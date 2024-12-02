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
#include "config_legacy.h"
#include "eos_library_helpers.h"
#include "io_helpers.h"
#include "json_helpers.h"
#include "logging.h"
#include <codecvt>
#include <eos_types.h>

#include "Config/PlatformConfig.hpp"
#include "Config/ProductConfig.hpp"

/**
  * @brief Retrieves the system cache directory.
  *
  * Retrieves the system's temporary directory and converts it to a UTF-8 encoded string.
  *
  * @return A pointer to a UTF-8 encoded string containing the system cache directory.
  *         This pointer is statically allocated, so it should not be freed by the caller.
  */
char* GetCacheDirectory();

/**
 * @brief Loads and initializes the Steam API DLL using a string path.
 *
 * Attempts to load the Steam API DLL from the specified path. If the DLL is not already
 * loaded, this function tries to load it and then calls `SteamAPI_Init`.
 *
 * @param steam_dll_filename The string path to the Steam API DLL.
 */
void eos_call_steam_init(const std::string& steam_dll_filename);

namespace pew::eos
{
    PEW_EOS_API_FUNC(void*) EOS_GetPlatformInterface()
    {
        return eos_library_helpers::eos_platform_handle;
    }

    void eos_set_loglevel_via_config()
    {
        if (eos_library_helpers::EOS_Logging_SetLogLevel_ptr == nullptr)
        {
            return;
        }

        auto path_to_log_config_json = config_legacy::get_path_for_eos_service_config(EOS_LOGLEVEL_CONFIG_FILENAME);

        if (!exists(path_to_log_config_json))
        {
            logging::log_inform("Log level config not found, using default log levels");
            return;
        }

        json_value_s* log_config_as_json = json_helpers::read_config_json_as_json_from_path(path_to_log_config_json);
        config_legacy::LogLevelConfig log_config = config_legacy::log_config_from_json_value(log_config_as_json);
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
            eos_library_helpers::EOS_Logging_SetLogLevel_ptr((EOS_ELogCategory)i, logging::eos_loglevel_str_to_enum(log_config.level[i]));
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

    void eos_init(const pew::eos::config::PlatformConfig& platform_config, const pew::eos::config::ProductConfig& product_config)
    {
        static int reserved[2] = { 1, 1 };
        EOS_InitializeOptions SDKOptions = { 0 };
        SDKOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
        SDKOptions.AllocateMemoryFunction = nullptr;
        SDKOptions.ReallocateMemoryFunction = nullptr;
        SDKOptions.ReleaseMemoryFunction = nullptr;
        SDKOptions.ProductName = product_config.product_name.c_str();
        SDKOptions.ProductVersion = product_config.product_version.c_str();
        SDKOptions.Reserved = reserved;
        SDKOptions.SystemInitializeOptions = nullptr;

        EOS_Initialize_ThreadAffinity overrideThreadAffinity = platform_config.thread_affinity;
        overrideThreadAffinity.ApiVersion = EOS_INITIALIZE_THREADAFFINITY_API_LATEST;

        SDKOptions.OverrideThreadAffinity = &overrideThreadAffinity;

        logging::log_inform("call EOS_Initialize");
        EOS_EResult InitResult = eos_library_helpers::EOS_Initialize_ptr(&SDKOptions);
        if (InitResult != EOS_EResult::EOS_Success)
        {
            logging::log_error("Unable to do eos init");
        }
        if (eos_library_helpers::EOS_Logging_SetLogLevel_ptr != nullptr)
        {
            eos_library_helpers::EOS_Logging_SetLogLevel_ptr(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, EOS_ELogLevel::EOS_LOG_VeryVerbose);
        }

        if (eos_library_helpers::EOS_Logging_SetCallback_ptr != nullptr)
        {
            eos_library_helpers::EOS_Logging_SetCallback_ptr(&logging::eos_log_callback);
        }
    }

    void eos_call_steam_init(const std::string& steam_dll_filename)
    {
        // Default (fallback) name of the steam dll to load.
        constexpr const char* DEFAULT_STEAM_DLL_NAME = "steam_api.dll";

        const auto steam_dll_path = std::filesystem::path(steam_dll_filename);

        // Check to see if the path exists, stop and log a warning if it does not.
        if (!exists(steam_dll_path))
        {
            logging::log_warn("Steam DLL provided (\""
                + steam_dll_path.string() + "\") does not exist; cannot "
                                            "initialize steam.");
            return;
        }

        // Get a handle to the steam dll
        HANDLE steam_dll_handle = GetModuleHandleA(steam_dll_path.filename().string().c_str());

        // If getting a handle to the steam dll was not successful, log a
        // warning, and if the filename given as a parameter is something other
        // than "steam_api.dll", try using that to get a library handle.
        if (steam_dll_handle == nullptr)
        {
            logging::log_warn("Steam DLL exists, but a handle to the module was not able to be loaded.");
            if (steam_dll_filename != DEFAULT_STEAM_DLL_NAME)
            {
                logging::log_inform(
                    "Because Steam DLL could not be loaded, and a "
                    "different dll file that the default was indicated, trying "
                    "again with the default dll file name of "
                    "\"" + std::string(DEFAULT_STEAM_DLL_NAME) + "\".");

                // TODO: Isn't it supposed to be the basename of the dll, like
                // it is in the first call to GetModuleHandleA?
                steam_dll_handle = GetModuleHandleA(DEFAULT_STEAM_DLL_NAME);
            }

            // If the steam dll handle is *STILL* not loaded, then attempt to
            // use load_library_at_path to load the dll - because _it_ uses the
            // LoadLibrary method, different from the GetModuleHandleA functions
            // used above.
            //
            // TODO: This seems very confusing and incorrect - it seems that all
            //       functionality above this line should be included into the
            //       "load_library_at_path" method - that way every time a
            //       library load is attempted, both methods (GetModuleHandleA
            //       and LoadLibrary) can be used if one of them fails.
            if (steam_dll_handle == nullptr)
            {
                steam_dll_handle = eos_library_helpers::load_library_at_path(steam_dll_path);
            }

            // If a handle to the steam dll has still not been obtained, then
            // log an error and stop.
            if (steam_dll_handle == nullptr)
            {
                logging::log_error(
                    "Could not obtain a handle to the steam library. "
                    "Steam initialization has failed.");
                return;
            }
        }

        logging::log_inform(
            "Handle to the Steam library has been retrieved. Next "
            "attempting to load a pointer to the SteamAPI_Init function from "
            "within it.");

        // TODO: Typedef should be moved outside the scope of the function?
        typedef bool(__cdecl* SteamAPI_Init_t)();

        // Get a pointer to the SteamAPI_Init function within the steam
        // library.
        const auto SteamAPI_Init = pew::eos::eos_library_helpers::load_function_with_name<SteamAPI_Init_t>(steam_dll_handle, "SteamAPI_Init");

        // If the SteamAPI_Init function pointer is null, then it was not
        // correctly retrieved from the steam library. Log an error and stop
        if (SteamAPI_Init == nullptr)
        {
            logging::log_error(
                "Could not load a pointer to the SteamAPI_Init "
                "function within the loaded steam library handle.");
            return;
        }

        if (SteamAPI_Init())
        {
            logging::log_inform("SteamAPI_Init returned true. "
                                "Steam has been initialized.");
        }
        else
        {
            logging::log_error(
                "SteamAPI_Init returned false. Steam was not able "
                "to be initialized.");
        }
    }

    char* GetCacheDirectory()
    {
        static char* s_tempPathBuffer = NULL;

        if (s_tempPathBuffer == NULL)
        {
            WCHAR tmp_buffer = 0;
            DWORD buffer_size = GetTempPathW(1, &tmp_buffer) + 1;
            WCHAR* lpTempPathBuffer = (TCHAR*)malloc(buffer_size * sizeof(TCHAR));
            GetTempPathW(buffer_size, lpTempPathBuffer);

            s_tempPathBuffer = string_helpers::create_utf8_str_from_wide_str(lpTempPathBuffer);
            free(lpTempPathBuffer);
        }

        return s_tempPathBuffer;
    }

    void eos_create(const pew::eos::config::PlatformConfig& platform_config, const pew::eos::config::ProductConfig& product_config)
    {
        EOS_Platform_Options platform_options = { 0 };
        platform_options.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
        platform_options.bIsServer = platform_config.is_server;
        platform_options.Flags = platform_config.platform_options_flags;
        platform_options.CacheDirectory = GetCacheDirectory();

        platform_options.EncryptionKey = platform_config.client_credentials.encryption_key.c_str();

        platform_options.OverrideCountryCode = platform_config.overrideCountryCode.c_str();
        platform_options.OverrideLocaleCode = platform_config.overrideLocaleCode.c_str();

        platform_options.ProductId = product_config.product_id.c_str();
        platform_options.SandboxId = platform_config.deployment.sandbox.id.c_str();
        platform_options.DeploymentId = platform_config.deployment.id.c_str();
        platform_options.ClientCredentials.ClientId = platform_config.client_credentials.client_id.c_str();
        platform_options.ClientCredentials.ClientSecret = platform_config.client_credentials.client_secret.c_str();

        platform_options.TickBudgetInMilliseconds = platform_config.tick_budget_in_milliseconds;

        if (platform_config.task_network_timeout_seconds > 0)
        {
            double task_network_timeout_seconds_dbl = platform_config.task_network_timeout_seconds;
            platform_options.TaskNetworkTimeoutSeconds = &task_network_timeout_seconds_dbl;
        }

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
        auto path_to_steam_config_json = config_legacy::get_path_for_eos_service_config(EOS_STEAM_CONFIG_FILENAME);

        // Defined here so that the override path lives long enough to be referenced by the create option
        config_legacy::EOSSteamConfig eos_steam_config;
        EOS_IntegratedPlatform_Options steam_integrated_platform_option = { 0 };
        EOS_IntegratedPlatform_Steam_Options steam_platform = { 0 };
        EOS_HIntegratedPlatformOptionsContainer integrated_platform_options_container = nullptr;
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

        if (exists(path_to_steam_config_json))
        {
            json_value_s* eos_steam_config_as_json = nullptr;
            eos_steam_config_as_json = json_helpers::read_config_json_as_json_from_path(path_to_steam_config_json);
            eos_steam_config = config_legacy::eos_steam_config_from_json_value(eos_steam_config_as_json);
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
            eos_library_helpers::EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_ptr(&options, &integrated_platform_options_container);
            platform_options.IntegratedPlatformOptionsContainerHandle = integrated_platform_options_container;

            EOS_IntegratedPlatformOptionsContainer_AddOptions addOptions = { EOS_INTEGRATEDPLATFORMOPTIONSCONTAINER_ADD_API_LATEST };
            addOptions.Options = &steam_integrated_platform_option;
            eos_library_helpers::EOS_IntegratedPlatformOptionsContainer_Add_ptr(integrated_platform_options_container, &addOptions);
        }
#endif

        //EOS_Platform_Options_debug_log(platform_options);
        logging::log_inform("run EOS_Platform_Create");
        eos_library_helpers::eos_platform_handle = eos_library_helpers::EOS_Platform_Create_ptr(&platform_options);
        if (integrated_platform_options_container)
        {
            eos_library_helpers::EOS_IntegratedPlatformOptionsContainer_Release_ptr(integrated_platform_options_container);
        }

        if (!eos_library_helpers::eos_platform_handle)
        {
            logging::log_error("failed to create the platform");
        }
    }
}
