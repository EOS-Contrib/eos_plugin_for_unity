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
#include "headers/Config/EOSWrapper.h"

#include "headers/Config/ProductConfig.h"
#include "headers/Config/WindowsConfig.h"
#include "headers/Config/SteamConfig.h"

namespace pew::eos
{
    using namespace common;

    EOS_HPlatform EOSWrapper::start_eos() const
    {
        // Get the configuration values
        const auto product_config = config::ConfigBase::get<config::ProductConfig>();
        const auto windows_config = config::ConfigBase::get<config::WindowsConfig>();

        // Apply any command line arguments that there might be
        apply_cli_arguments(*windows_config, *product_config);

        // Initialize the sdk
        init(*windows_config, *product_config);

        // Create the platform and return a pointer to the platform interface.
        return create(*windows_config, *product_config);
    }

    void EOSWrapper::init(const config::PlatformConfig& platform_config, const config::ProductConfig& product_config) const
    {
        EOS_InitializeOptions sdk_options{};
        set_initialize_options(sdk_options, platform_config, product_config);

        const auto init_result = call_library_function<EOS_Initialize_t>(&sdk_options);
        if (init_result != EOS_EResult::EOS_Success)
        {
            logging::log_error("Unable to do eos init");
        }

        // Set the log level and the log callback
        call_library_function<EOS_Logging_SetLogLevel_t>(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, EOS_ELogLevel::EOS_LOG_VeryVerbose);
        call_library_function<EOS_Logging_SetCallback_t>(&logging::eos_log_callback);
    }

    void EOSWrapper::set_platform_options(EOS_Platform_Options& platform_options, const config::PlatformConfig& platform_config, const config::ProductConfig& product_config) const
    {
        
        platform_options.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
        platform_options.bIsServer = platform_config.is_server;
        platform_options.Flags = platform_config.platform_options_flags;
        platform_options.CacheDirectory = get_cache_directory();
        platform_options.OverrideCountryCode = nullptr;
        platform_options.OverrideLocaleCode = platform_config.overrideLocaleCode.length() > 0 ? platform_config.overrideLocaleCode.c_str() : nullptr;
        platform_options.ProductId = product_config.product_id.c_str();
        platform_options.SandboxId = platform_config.deployment.sandbox.id.c_str();
        platform_options.DeploymentId = platform_config.deployment.id.c_str();

        // Set the Client Credentials and associated values.
        static auto encryption_key = platform_config.client_credentials.encryption_key;
        static auto client_id = platform_config.client_credentials.client_id;
        static auto client_secret = platform_config.client_credentials.client_secret;

        platform_options.EncryptionKey = encryption_key.c_str();
        platform_options.ClientCredentials.ClientId = client_id.c_str();
        platform_options.ClientCredentials.ClientSecret = client_secret.c_str();
        platform_options.TickBudgetInMilliseconds = platform_config.tick_budget_in_milliseconds;

        // Because input parameter is const, we need to make a copy of the value
        static double task_network_timeout_seconds = platform_config.task_network_timeout_seconds;
        platform_options.TaskNetworkTimeoutSeconds = &task_network_timeout_seconds;

        static EOS_Platform_RTCOptions rtc_options = { 0 };
        rtc_options.ApiVersion = EOS_PLATFORM_RTCOPTIONS_API_LATEST;

        logging::log_inform("setting up rtc");
        static std::filesystem::path xaudio2_dll_path = get_path_relative_to_current_module(XAUDIO2_DLL_NAME);
        static std::string xaudio2_dll_path_as_string = to_utf8_str(xaudio2_dll_path);
        static EOS_Windows_RTCOptions windows_rtc_options;
        windows_rtc_options.ApiVersion = EOS_WINDOWS_RTCOPTIONS_API_LATEST;
        windows_rtc_options.XAudio29DllPath = xaudio2_dll_path_as_string.c_str();
        logging::log_warn(xaudio2_dll_path_as_string.c_str());

        if (!exists(xaudio2_dll_path))
        {
            logging::log_warn("Missing XAudio dll!");
        }
        rtc_options.PlatformSpecificOptions = &windows_rtc_options;
        platform_options.RTCOptions = &rtc_options;

        static EOS_HIntegratedPlatformOptionsContainer integrated_platform_options_container = nullptr;

        logging::log_inform("run EOS_Platform_Create");

        static EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions options = {
            EOS_INTEGRATEDPLATFORM_CREATEINTEGRATEDPLATFORMOPTIONSCONTAINER_API_LATEST
        };

        call_library_function<EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t>(&options, &integrated_platform_options_container);

        platform_options.IntegratedPlatformOptionsContainerHandle = integrated_platform_options_container;


    }

    void EOSWrapper::set_initialize_options(EOS_InitializeOptions& intialize_options,
        const config::PlatformConfig& platform_config, const config::ProductConfig& product_config) const
    {
        intialize_options.ApiVersion = EOS_INITIALIZE_API_LATEST;
        intialize_options.AllocateMemoryFunction = nullptr;
        intialize_options.ReallocateMemoryFunction = nullptr;
        intialize_options.ReleaseMemoryFunction = nullptr;
        intialize_options.ProductName = product_config.product_name.c_str();
        intialize_options.ProductVersion = product_config.product_version.c_str();
        intialize_options.Reserved = new int[2] {1, 1};
        intialize_options.SystemInitializeOptions = nullptr;

        // Because the parameter passed is const, the value needs to be copied.
        static EOS_Initialize_ThreadAffinity affinity = platform_config.thread_affinity;
        intialize_options.OverrideThreadAffinity = &affinity;
    }

    EOS_HPlatform EOSWrapper::create(const config::PlatformConfig& platform_config,
                                     const config::ProductConfig& product_config) const
    {
        EOS_Platform_Options platform_options = { 0 };
        set_platform_options(platform_options, platform_config, product_config);

        


        
        const auto eos_platform_handle = call_library_function<EOS_Platform_Create_t>(&platform_options);

        if (!eos_platform_handle)
        {
            logging::log_error("failed to create the platform");
        }

        if (platform_options.IntegratedPlatformOptionsContainerHandle)
        {
            call_library_function<EOS_IntegratedPlatformOptionsContainer_Release_t>(platform_options.IntegratedPlatformOptionsContainerHandle);
        }

        return eos_platform_handle;
    }
}

