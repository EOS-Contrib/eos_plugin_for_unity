/*
 * Copyright (c) 2021 PlayEveryWare
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
#include "eos_helpers.h"
#include <filesystem>
#include <sstream>
#include "io_helpers.h"
#include "json_helpers.h"
#include "logging.h"
#include "eos_init.h"
#include <codecvt>

#include "EOSWrapper.h"
#include "ProductConfig.h"
#include "WindowsConfig.h"
#include "headers/config_DEPRECATED.h"

namespace pew::eos
{
    DLL_EXPORT(void*) EOS_GetPlatformInterface()
    {
        const auto eos_sdk = new EOSWrapper();

        return eos_sdk->start_eos();
    }

    void set_eos_loglevel(const EOSWrapper& eos_sdk)
    {
        auto path_to_log_config_json = config::get_path_for_eos_service_config(EOS_LOGLEVEL_CONFIG_FILENAME);

        if (!exists(path_to_log_config_json))
        {
            logging::log_inform("Log level config not found, using default log levels");
            return;
        }

        json_value_s* log_config_as_json = json_helpers::read_config_json_as_json_from_path(path_to_log_config_json);
        config::LogLevelConfig log_config = config::log_config_from_json_value(log_config_as_json);
        free(log_config_as_json);

        // Validation to prevent out of range exception
        if (log_config.category.size() != log_config.level.size())
        {
            logging::log_warn("Log level config entries out of range");
            return;
        }

        // Last in the vector is AllCategories, and will not be set
        size_t individual_category_size = log_config.category.size() > 0 ? log_config.category.size() - 1 : 0;
        if (individual_category_size == 0)
        {
            logging::log_warn("Log level config entries empty");
            return;
        }

        for (size_t i = 0; i < individual_category_size; i++)
        {
            eos_sdk.call_library_function<EOS_Logging_SetLogLevel_t>(
                static_cast<EOS_ELogCategory>(i), 
                logging::eos_loglevel_str_to_enum(log_config.level[i])
            );
        }

        logging::log_inform("Log levels set according to config");
    }

    void EOS_Platform_Options_debug_log(const EOS_Platform_Options& platform_options)
    {
        std::stringstream output;
        output << platform_options.ApiVersion << "\n";
        output << platform_options.bIsServer << "\n";
        output << platform_options.Flags << "\n";
        output << platform_options.CacheDirectory << "\n";

        output << platform_options.EncryptionKey << "\n";
        if (platform_options.OverrideCountryCode)
        {
            output << platform_options.OverrideCountryCode << "\n";
        }

        if (platform_options.OverrideLocaleCode)
        {
            output << platform_options.OverrideLocaleCode << "\n";
        }
        output << platform_options.ProductId << "\n";
        output << platform_options.SandboxId << "\n";
        output << platform_options.DeploymentId << "\n";
        output << platform_options.ClientCredentials.ClientId << "\n";
        output << platform_options.ClientCredentials.ClientSecret << "\n";

        auto* rtc_options = platform_options.RTCOptions;
        auto* windows_rtc_options = (EOS_Windows_RTCOptions*)rtc_options->PlatformSpecificOptions;

        output << windows_rtc_options->ApiVersion << "\n";
        output << windows_rtc_options->XAudio29DllPath << "\n";

        logging::log_inform(output.str().c_str());
    }

    CONFIG_API EOS_HPlatform load_eos(const config::PlatformConfig& platform_config, const config::ProductConfig& product_config)
    {
        const EOSWrapper* eos_sdk = new EOSWrapper();
        eos_init(*eos_sdk, platform_config, product_config);
        const auto platform_handle = eos_create(*eos_sdk, platform_config, product_config);
        set_eos_loglevel(*eos_sdk);
        return platform_handle;
    }

    void eos_init(const EOSWrapper& eos_sdk, const config::PlatformConfig& platform_config, const config::ProductConfig& product_config)
    {
        static int reserved[2] = { 1, 1 };
        EOS_InitializeOptions sdk_options{};
        sdk_options.ApiVersion = EOS_INITIALIZE_API_LATEST;
        sdk_options.AllocateMemoryFunction = nullptr;
        sdk_options.ReallocateMemoryFunction = nullptr;
        sdk_options.ReleaseMemoryFunction = nullptr;
        sdk_options.ProductName = product_config.product_name.c_str();
        sdk_options.ProductVersion = product_config.product_version.c_str();
        sdk_options.Reserved = reserved;
        sdk_options.SystemInitializeOptions = nullptr;

        // Because the parameter passed is const, the value needs to be copied.
        EOS_Initialize_ThreadAffinity affinity = platform_config.thread_affinity;
        sdk_options.OverrideThreadAffinity = &affinity;

        const auto init_result = eos_sdk.call_library_function<EOS_Initialize_t>(&sdk_options);
        //const auto init_result = call_library_function<EOS_Initialize_t>("EOS_Initialize", s_eos_sdk_lib_handle, &sdk_options);
        
        if (init_result != EOS_EResult::EOS_Success)
        {
            logging::log_error("Unable to do eos init");
        }

        eos_sdk.call_library_function<EOS_Logging_SetLogLevel_t>(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, EOS_ELogLevel::EOS_LOG_VeryVerbose);
        eos_sdk.call_library_function<EOS_Logging_SetCallback_t>(&logging::eos_log_callback);
    }

    void* load_library_at_path(const std::filesystem::path& library_path)
    {
        void* to_return = nullptr;

#if PLATFORM_WINDOWS
        logging::log_inform(("Loading path at " + string_helpers::to_utf8_str(library_path)).c_str());
        HMODULE handle = LoadLibrary(library_path.c_str());
        to_return = (void*)handle;
#endif

        return to_return;
    }

    bool QueryRegKey(const HKEY InKey, const TCHAR* InSubKey, const TCHAR* InValueName, std::wstring& OutData)
    {
        bool bSuccess = false;
#if PLATFORM_WINDOWS
        // Redirect key depending on system
        for (uint32_t RegistryIndex = 0; RegistryIndex < 2 && !bSuccess; ++RegistryIndex)
        {
            HKEY Key = 0;
            const uint32_t RegFlags = (RegistryIndex == 0) ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;
            if (RegOpenKeyEx(InKey, InSubKey, 0, KEY_READ | RegFlags, &Key) == ERROR_SUCCESS)
            {
                DWORD Size = 0;
                // First, we'll call RegQueryValueEx to find out how large of a buffer we need
                if ((RegQueryValueEx(Key, InValueName, NULL, NULL, NULL, &Size) == ERROR_SUCCESS) && Size)
                {
                    // Allocate a buffer to hold the value and call the function again to get the data
                    char* Buffer = new char[Size];
                    if (RegQueryValueEx(Key, InValueName, NULL, NULL, (LPBYTE)Buffer, &Size) == ERROR_SUCCESS)
                    {
                        const uint32_t Length = (Size / sizeof(TCHAR)) - 1;
                        OutData = (TCHAR*)Buffer;
                        bSuccess = true;
                    }
                    delete[] Buffer;
                }
                RegCloseKey(Key);
            }
        }
#endif
        return bSuccess;
    }

    static bool get_overlay_dll_path(std::filesystem::path* OutDllPath)
    {
#if PLATFORM_WINDOWS
        const TCHAR* RegKey = TEXT(R"(SOFTWARE\Epic Games\EOS)");
        const TCHAR* RegValue = TEXT("OverlayPath");
        std::wstring OverlayDllDirectory;

        if (!QueryRegKey(HKEY_CURRENT_USER, RegKey, RegValue, OverlayDllDirectory))
        {
            if (!QueryRegKey(HKEY_LOCAL_MACHINE, RegKey, RegValue, OverlayDllDirectory))
            {
                return false;
            }
        }

        *OutDllPath = std::filesystem::path(OverlayDllDirectory) / OVERLAY_DLL_NAME;
        return exists(*OutDllPath) && is_regular_file(*OutDllPath);
#else
        log_inform("Trying to get a DLL path on a platform without DLL paths searching");
        return false;
#endif
    }

    static void eos_call_steam_init(const std::filesystem::path& steam_dll_path)
    {
        std::string steam_dll_path_as_string = steam_dll_path.string();
        eos_call_steam_init(steam_dll_path_as_string);
    }

    static void eos_call_steam_init(const std::string& steam_dll_path)
    {
        auto steam_dll_path_string = io_helpers::get_basename(steam_dll_path);
        HANDLE steam_dll_handle = GetModuleHandleA(steam_dll_path_string.c_str());

        // Check the default name for the steam_api.dll
        if (!steam_dll_handle)
        {
            steam_dll_handle = GetModuleHandleA("steam_api.dll");
        }

        // in the case that it's not loaded, try to load it from the user provided path
        if (!steam_dll_handle)
        {
            steam_dll_handle = load_library_at_path(steam_dll_path);
        }

        if (steam_dll_handle != nullptr)
        {
            typedef bool(__cdecl* SteamAPI_Init_t)();
            SteamAPI_Init_t SteamAPI_Init;
            // TODO: Create SteamAPIWrapper class
            /*if (try_load_function(steam_dll_handle, "SteamAPI_Init", SteamAPI_Init) && SteamAPI_Init())
            {
                logging::log_inform("Called SteamAPI_Init with success!");
            }*/
        }
    }

    EOS_HPlatform eos_create(const EOSWrapper& eos_sdk, const config::PlatformConfig& platform_config, const config::ProductConfig& product_config)
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
#if PLATFORM_WINDOWS
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
#endif

#if PLATFORM_WINDOWS
        auto path_to_steam_config_json = config::get_path_for_eos_service_config(EOS_STEAM_CONFIG_FILENAME);

        // Defined here so that the override path lives long enough to be referenced by the create option
        config::EOSSteamConfig eos_steam_config;
        EOS_IntegratedPlatform_Options steam_integrated_platform_option = { 0 };
        EOS_IntegratedPlatform_Steam_Options steam_platform = { 0 };
        EOS_HIntegratedPlatformOptionsContainer integrated_platform_options_container = nullptr;
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

        if (exists(path_to_steam_config_json))
        {
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

            steam_integrated_platform_option.ApiVersion = EOS_INTEGRATEDPLATFORM_OPTIONS_API_LATEST;
            steam_integrated_platform_option.Type = EOS_IPT_Steam;
            steam_integrated_platform_option.Flags = eos_steam_config.flags;
            steam_integrated_platform_option.InitOptions = &steam_platform;

            steam_platform.ApiVersion = EOS_INTEGRATEDPLATFORM_STEAM_OPTIONS_API_LATEST;

            EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions options = { EOS_INTEGRATEDPLATFORM_CREATEINTEGRATEDPLATFORMOPTIONSCONTAINER_API_LATEST };
            eos_sdk.call_library_function<EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t>(&options, &integrated_platform_options_container);
            
            platform_options.IntegratedPlatformOptionsContainerHandle = integrated_platform_options_container;

            EOS_IntegratedPlatformOptionsContainer_AddOptions addOptions = { EOS_INTEGRATEDPLATFORMOPTIONSCONTAINER_ADD_API_LATEST };
            addOptions.Options = &steam_integrated_platform_option;

            eos_sdk.call_library_function<EOS_IntegratedPlatformOptionsContainer_Add_t>(integrated_platform_options_container, &addOptions);
        }
#endif

        //EOS_Platform_Options_debug_log(platform_options);
        logging::log_inform("run EOS_Platform_Create");

        auto eos_platform_handle = eos_sdk.call_library_function<EOS_Platform_Create_t>(&platform_options);

        if (integrated_platform_options_container)
        {
            eos_sdk.call_library_function<EOS_IntegratedPlatformOptionsContainer_Release_t>(integrated_platform_options_container);
        }

        if (!eos_platform_handle)
        {
            logging::log_error("failed to create the platform");
        }

        return eos_platform_handle;
    }
}
