/*
 * Copyright (c) 2024 PlayEveryWare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <pch.h>
#include "EOSWrapper.h"

#include <codecvt>

#include "ProductConfig.h"
#include "WindowsConfig.h"
#include "config.h"
#include "config_DEPRECATED.h"
#include "eos_helpers.h"
#include "json_helpers.h"
#include "SteamConfig.h"
#include "string_helpers.h"

namespace pew::eos
{
    EOS_HPlatform EOSWrapper::start_eos() const
    {
        // Get the configuration values
        const auto product_config = config::Config::get<config::ProductConfig>();
        auto windows_config = config::Config::get<config::WindowsConfig>();

        // Apply any command line arguments that there might be
        apply_cli_arguments(*windows_config, *product_config);

        // Initialize the sdk
        init(*windows_config, *product_config);

        const auto platform_interface = create(*windows_config, *product_config);

        return platform_interface;
    }

    void EOSWrapper::init(const config::PlatformConfig& platform_config, const config::ProductConfig& product_config) const
    {
        EOS_InitializeOptions sdk_options{};
        sdk_options.ApiVersion = EOS_INITIALIZE_API_LATEST;
        sdk_options.AllocateMemoryFunction = nullptr;
        sdk_options.ReallocateMemoryFunction = nullptr;
        sdk_options.ReleaseMemoryFunction = nullptr;
        sdk_options.ProductName = product_config.product_name.c_str();
        sdk_options.ProductVersion = product_config.product_version.c_str();
        sdk_options.Reserved = new int[] {1, 1};
        sdk_options.SystemInitializeOptions = nullptr;

        // Because the parameter passed is const, the value needs to be copied.
        EOS_Initialize_ThreadAffinity affinity = platform_config.thread_affinity;
        sdk_options.OverrideThreadAffinity = &affinity;

        const auto init_result = call_library_function<EOS_Initialize_t>(&sdk_options);
        if (init_result != EOS_EResult::EOS_Success)
        {
            logging::log_error("Unable to do eos init");
        }

        // Set the log level and the log callback
        call_library_function<EOS_Logging_SetLogLevel_t>(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, EOS_ELogLevel::EOS_LOG_VeryVerbose);
        call_library_function<EOS_Logging_SetCallback_t>(&logging::eos_log_callback);
    }

    EOS_HPlatform EOSWrapper::create(const config::PlatformConfig& platform_config,
                                     const config::ProductConfig& product_config) const
    {
        auto platform_options = create_platform_options(platform_config, product_config);

        EOS_HIntegratedPlatformOptionsContainer integrated_platform_options_container = nullptr;

        configure_steam_options(platform_options, integrated_platform_options_container);

        logging::log_inform("run EOS_Platform_Create");

        const auto eos_platform_handle = call_library_function<EOS_Platform_Create_t>(&platform_options);

        if (integrated_platform_options_container)
        {
            call_library_function<EOS_IntegratedPlatformOptionsContainer_Release_t>(integrated_platform_options_container);
        }

        if (!eos_platform_handle)
        {
            logging::log_error("failed to create the platform");
        }

        return eos_platform_handle;
    }

    EOS_Platform_Options EOSWrapper::create_platform_options(const config::PlatformConfig& platform_config,
        const config::ProductConfig& product_config) const
    {
        EOS_Platform_Options platform_options = { 0 };
        platform_options.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
        platform_options.bIsServer = platform_config.is_server;
        platform_options.Flags = platform_config.platform_options_flags;
        platform_options.CacheDirectory = io_helpers::get_cache_directory();
        platform_options.OverrideCountryCode = nullptr;
        platform_options.OverrideLocaleCode = platform_config.overrideLocaleCode.length() > 0 ? platform_config.overrideLocaleCode.c_str() : nullptr;
        platform_options.ProductId = product_config.product_id.c_str();
        platform_options.SandboxId = platform_config.deployment.sandbox.id.c_str();
        platform_options.DeploymentId = platform_config.deployment.id.c_str();

        // Set the Client Credentials and associated values.
        auto [client_id, client_secret, encryption_key] = platform_config.client_credentials;
        platform_options.EncryptionKey = encryption_key.c_str();
        platform_options.ClientCredentials.ClientId = client_id.c_str();
        platform_options.ClientCredentials.ClientSecret = client_secret.c_str();
        platform_options.TickBudgetInMilliseconds = platform_config.tick_budget_in_milliseconds;

        // Because input parameter is const, we need to make a copy of the value
        double task_network_timeout_seconds = platform_config.task_network_timeout_seconds;
        platform_options.TaskNetworkTimeoutSeconds = &task_network_timeout_seconds;

        EOS_Platform_RTCOptions rtc_options = { 0 };

        rtc_options.ApiVersion = EOS_PLATFORM_RTCOPTIONS_API_LATEST;

        logging::log_inform("setting up rtc");
        std::filesystem::path xaudio2_dll_path = io_helpers::get_path_relative_to_current_module(XAUDIO2_DLL_NAME);
        std::string xaudio2_dll_path_as_string = string_helpers::to_utf8_str(xaudio2_dll_path);
        EOS_Windows_RTCOptions windows_rtc_options = { 0 };
        windows_rtc_options.ApiVersion = EOS_WINDOWS_RTCOPTIONS_API_LATEST;
        windows_rtc_options.XAudio29DllPath = xaudio2_dll_path_as_string.c_str();
        logging::log_warn(xaudio2_dll_path_as_string.c_str());

        if (!std::filesystem::exists(xaudio2_dll_path))
        {
            logging::log_warn("Missing XAudio dll!");
        }
        rtc_options.PlatformSpecificOptions = &windows_rtc_options;
        platform_options.RTCOptions = &rtc_options;

        return platform_options;
    }

    void EOSWrapper::configure_steam_options(EOS_Platform_Options& platform_options, EOS_HIntegratedPlatformOptionsContainer& integrated_platform_options_container) const
    {
        const auto steam_config = config::Config::get<config::SteamConfig>();
        auto path_to_steam_config_json = config::get_path_for_eos_service_config(EOS_STEAM_CONFIG_FILENAME);


        if (exists(path_to_steam_config_json))
        {
            
            
            config::EOSSteamConfig eos_steam_config;
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            json_value_s* eos_steam_config_as_json = nullptr;
            eos_steam_config_as_json = json_helpers::read_config_json_as_json_from_path(path_to_steam_config_json);
            eos_steam_config = config::eos_steam_config_from_json_value(eos_steam_config_as_json);
            free(eos_steam_config_as_json);

            if (eos_steam_config.OverrideLibraryPath.has_value())
            {
                if (!std::filesystem::exists(eos_steam_config.OverrideLibraryPath.value()))
                {
                    auto override_lib_path_as_str = io_helpers::get_basename(eos_steam_config.OverrideLibraryPath.value());
                    auto found_steam_path = io_helpers::get_path_relative_to_current_module(override_lib_path_as_str);

                    // Fall back and use the steam dll name based on the
                    // type of binary the GfxPluginNativeRender
                    if (!std::filesystem::exists(found_steam_path) || eos_steam_config.OverrideLibraryPath.value().empty())
                    {
                        found_steam_path = io_helpers::get_path_relative_to_current_module(STEAM_API_DLL);
                    }

                    if (std::filesystem::exists(found_steam_path))
                    {
                        eos_steam_config.OverrideLibraryPath = converter.to_bytes(found_steam_path.wstring());
                    }
                }
            }
            else
            {
                auto found_steam_path = io_helpers::get_path_relative_to_current_module(STEAM_API_DLL);
                if (exists(found_steam_path))
                {
                    eos_steam_config.OverrideLibraryPath = converter.to_bytes(found_steam_path.wstring());
                }
            }

            EOS_IntegratedPlatform_Steam_Options steam_platform = { 0 };
            if (eos_steam_config.is_managed_by_application())
            {
                eos_call_steam_init(eos_steam_config.OverrideLibraryPath.value());
                eos_steam_config.OverrideLibraryPath.reset();
            }

            if (eos_steam_config.OverrideLibraryPath.has_value())
            {
                steam_platform.OverrideLibraryPath = eos_steam_config.OverrideLibraryPath.value().c_str();
            }

            steam_platform.SteamMajorVersion = eos_steam_config.steamSDKMajorVersion;
            steam_platform.SteamMinorVersion = eos_steam_config.steamSDKMinorVersion;

            // For each element in the array (each of which is a string of an api version information)
            // iterate across each character, and at the end of a string add a null terminator \0
            // then add one more null terminator at the end of the array
            std::vector<char> steamApiInterfaceVersionsAsCharArray;

            for (const auto& currentFullValue : eos_steam_config.steamApiInterfaceVersionsArray)
            {
                for (char currentCharacter : currentFullValue)
                {
                    steamApiInterfaceVersionsAsCharArray.push_back(currentCharacter);
                }

                steamApiInterfaceVersionsAsCharArray.push_back('\0');
            }
            steamApiInterfaceVersionsAsCharArray.push_back('\0');

            steam_platform.SteamApiInterfaceVersionsArray = reinterpret_cast<char*>(steamApiInterfaceVersionsAsCharArray.data());

            auto size = steamApiInterfaceVersionsAsCharArray.size();

            if (size > EOS_INTEGRATEDPLATFORM_STEAM_MAX_STEAMAPIINTERFACEVERSIONSARRAY_SIZE)
            {
                logging::log_error("Size given for SteamApiInterfaceVersionsAsCharArray exceeds the maximum value.");
            }
            else
            {
                // steam_platform needs to have a count of how many bytes the "array" is, stored in SteamApiInterfaceVersionsArrayBytes
                // This has some fuzzy behavior; if you set it to 0 or count it up properly, there won't be a logged problem
                // if you put a non-zero amount that is insufficient, there will be an unclear logged error message
                steam_platform.SteamApiInterfaceVersionsArrayBytes = static_cast<uint32_t>(size);
            }

            EOS_IntegratedPlatform_Options steam_integrated_platform_option = { 0 };
            steam_integrated_platform_option.ApiVersion = EOS_INTEGRATEDPLATFORM_OPTIONS_API_LATEST;
            steam_integrated_platform_option.Type = EOS_IPT_Steam;
            steam_integrated_platform_option.Flags = eos_steam_config.flags;
            steam_integrated_platform_option.InitOptions = &steam_platform;

            steam_platform.ApiVersion = EOS_INTEGRATEDPLATFORM_STEAM_OPTIONS_API_LATEST;

            EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions options = { EOS_INTEGRATEDPLATFORM_CREATEINTEGRATEDPLATFORMOPTIONSCONTAINER_API_LATEST };
            call_library_function<EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t>(&options, &integrated_platform_options_container);

            platform_options.IntegratedPlatformOptionsContainerHandle = integrated_platform_options_container;

            EOS_IntegratedPlatformOptionsContainer_AddOptions addOptions = { EOS_INTEGRATEDPLATFORMOPTIONSCONTAINER_ADD_API_LATEST };
            addOptions.Options = &steam_integrated_platform_option;

            call_library_function<EOS_IntegratedPlatformOptionsContainer_Add_t>(integrated_platform_options_container, &addOptions);
        }

    }
}

