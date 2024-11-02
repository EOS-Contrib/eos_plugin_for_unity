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

// dllmain.cpp : Defines the entry point for the DLL application.
// This file does some *magick* to load the EOS Overlay DLL.
// This is apparently needed so that the Overlay can render properly
#include "pch.h"
#include <config.h>
#include <string_helpers.h>
#include <logging.h>
#include <json_helpers.h>

#include "temp.h"

using namespace playeveryware::eos::string_helpers;
using namespace playeveryware::eos::logging;
using namespace playeveryware::eos::config;
using namespace playeveryware::eos::json_helpers;

namespace fs = std::filesystem;
typedef HKEY__* HKEY;

using FSig_ApplicationWillShutdown = void (__stdcall *)(void);
FSig_ApplicationWillShutdown FuncApplicationWillShutdown = nullptr;



//-------------------------------------------------------------------------
// Fetched out of DLLs




extern "C"
{
    void __declspec(dllexport) __stdcall UnityPluginLoad(void* unityInterfaces);
    void __declspec(dllexport) __stdcall UnityPluginUnload();
}

typedef void (*log_flush_function_t)(const char* str);
DLL_EXPORT(void) global_log_flush_with_function(log_flush_function_t log_flush_function)
{
    if (buffered_output.size() > 0)
    {
        for (const std::string& str : buffered_output)
        {
            log_flush_function(str.c_str());
        }
        buffered_output.clear();
    }
}

//-------------------------------------------------------------------------
EXTERN_C void EOS_CALL eos_log_callback(const EOS_LogMessage* message)
{
    constexpr size_t final_timestamp_len = 32;
    char final_timestamp[final_timestamp_len] = {0};

    if (create_timestamp_str(final_timestamp, final_timestamp_len))
    {
        global_logf("%s %s (%s): %s", final_timestamp, message->Category, eos_loglevel_to_print_str(message->Level), message->Message);
    }
    else
    {
        global_logf("%s (%s): %s", message->Category, eos_loglevel_to_print_str(message->Level), message->Message);
    }

}


//-------------------------------------------------------------------------
void unload_library(void* library_handle)
{
    FreeLibrary((HMODULE)library_handle);
}

//-------------------------------------------------------------------------
void eos_init(const EOSConfig& eos_config)
{
    static int reserved[2] = {1, 1};
    EOS_InitializeOptions SDKOptions = { 0 };
    SDKOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
    SDKOptions.AllocateMemoryFunction = nullptr;
    SDKOptions.ReallocateMemoryFunction = nullptr;
    SDKOptions.ReleaseMemoryFunction = nullptr;
    SDKOptions.ProductName = eos_config.productName.c_str();
    SDKOptions.ProductVersion = eos_config.productVersion.c_str();
    SDKOptions.Reserved = reserved;
    SDKOptions.SystemInitializeOptions = nullptr;

    EOS_Initialize_ThreadAffinity overrideThreadAffinity = {0};

    overrideThreadAffinity.ApiVersion = EOS_INITIALIZE_THREADAFFINITY_API_LATEST;

    overrideThreadAffinity.HttpRequestIo = eos_config.ThreadAffinity_HTTPRequestIO;
    overrideThreadAffinity.NetworkWork = eos_config.ThreadAffinity_networkWork;
    overrideThreadAffinity.P2PIo = eos_config.ThreadAffinity_P2PIO;
    overrideThreadAffinity.RTCIo = eos_config.ThreadAffinity_RTCIO;
    overrideThreadAffinity.StorageIo = eos_config.ThreadAffinity_storageIO;
    overrideThreadAffinity.WebSocketIo = eos_config.ThreadAffinity_webSocketIO;


    SDKOptions.OverrideThreadAffinity = &overrideThreadAffinity;

    log_inform("call EOS_Initialize");
    EOS_EResult InitResult = playeveryware::eos::temp::EOS_Initialize_ptr(&SDKOptions);
    if (InitResult != EOS_EResult::EOS_Success)
    {
        log_error("Unable to do eos init");
    }
    if (playeveryware::eos::temp::EOS_Logging_SetLogLevel_ptr != nullptr)
    {
        playeveryware::eos::temp::EOS_Logging_SetLogLevel_ptr(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, EOS_ELogLevel::EOS_LOG_VeryVerbose);
    }

    if (playeveryware::eos::temp::EOS_Logging_SetCallback_ptr != nullptr)
    {
        playeveryware::eos::temp::EOS_Logging_SetCallback_ptr(&eos_log_callback);
    }
}

//-------------------------------------------------------------------------
static char* GetCacheDirectory()
{
    static char* s_tempPathBuffer = NULL;

    if (s_tempPathBuffer == NULL)
    {
        WCHAR tmp_buffer = 0;
        DWORD buffer_size = GetTempPathW(1, &tmp_buffer) + 1;
        WCHAR* lpTempPathBuffer = (TCHAR*)malloc(buffer_size * sizeof(TCHAR));
        GetTempPathW(buffer_size, lpTempPathBuffer);

        s_tempPathBuffer = create_utf8_str_from_wide_str(lpTempPathBuffer);
        free(lpTempPathBuffer);
    }

    return s_tempPathBuffer;
}


//-------------------------------------------------------------------------
static EOSSteamConfig eos_steam_config_from_json_value(json_value_s *config_json)
{
    struct json_object_s* config_json_object = json_value_as_object(config_json);
    struct json_object_element_s* iter = config_json_object->start;
    EOSSteamConfig eos_config;
    eos_config.flags;

    while (iter != nullptr)
    {
        if (!strcmp("flags", iter->name->string))
        {
            eos_config.flags = eos_collect_integrated_platform_managment_flags(iter);

        }
        else if (!strcmp("overrideLibraryPath", iter->name->string))
        {
            const char *override_library_path = json_value_as_string(iter->value)->string;

            if (strcmp("NULL", override_library_path)
                && strcmp("null", override_library_path)
                )
            {
                eos_config.OverrideLibraryPath = override_library_path;
            }

        }
        else if (!strcmp("steamSDKMajorVersion", iter->name->string))
        {
            eos_config.steamSDKMajorVersion = json_value_as_uint32(iter->value);
        }
        else if (!strcmp("steamSDKMinorVersion", iter->name->string))
        {
            eos_config.steamSDKMinorVersion = json_value_as_uint32(iter->value);
        }
        else if (!strcmp("steamApiInterfaceVersionsArray", iter->name->string))
        {
            json_array_s* apiVersions = json_value_as_array(iter->value);

            for (auto e = apiVersions->start; e != nullptr; e = e->next)
            {
                eos_config.steamApiInterfaceVersionsArray.push_back(json_value_as_string(e->value)->string);
            }
        }

        iter = iter->next;
    }

    return eos_config;
}


//-------------------------------------------------------------------------
json_value_s* read_eos_config_as_json_value_from_file(std::string config_filename)
{
    std::filesystem::path path_to_config_json = get_path_for_eos_service_config(config_filename);

    return read_config_json_as_json_from_path(path_to_config_json);
}

//-------------------------------------------------------------------------
static void EOS_Platform_Options_debug_log(const EOS_Platform_Options& platform_options)
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

    auto *rtc_options = platform_options.RTCOptions;
    auto *windows_rtc_options = (EOS_Windows_RTCOptions*)rtc_options->PlatformSpecificOptions;

    output << windows_rtc_options->ApiVersion << "\n";
    output << windows_rtc_options->XAudio29DllPath << "\n";

    log_inform(output.str().c_str());
}

//-------------------------------------------------------------------------
static std::string basename(const std::string& path)
{
    std::string filename;
    filename.resize(path.length() + 1);
    _splitpath_s(path.c_str(), NULL, 0, NULL, 0, filename.data(), filename.size(), NULL, 0);

    return filename;
}

//-------------------------------------------------------------------------
static void eos_call_steam_init(const std::filesystem::path& steam_dll_path)
{
    std::string steam_dll_path_as_string = steam_dll_path.string();
    eos_call_steam_init(steam_dll_path_as_string);
}

//-------------------------------------------------------------------------
// This function assumes that if the caller has already loaded the steam DLL,
// that SteamAPI_Init doesn't need to be called
static void eos_call_steam_init(const std::string& steam_dll_path)
{
    auto steam_dll_path_string = basename(steam_dll_path);
    HANDLE steam_dll_handle = GetModuleHandleA(steam_dll_path_string.c_str());

    // Check the default name for the steam_api.dll
    if (!steam_dll_handle)
    {
        steam_dll_handle = GetModuleHandleA("steam_api.dll");
    }

    // in the case that it's not loaded, try to load it from the user provided path
    if (!steam_dll_handle)
    {
        steam_dll_handle = playeveryware::eos::temp::load_library_at_path(steam_dll_path);
    }

    if (steam_dll_handle != nullptr)
    {
        typedef bool(__cdecl* SteamAPI_Init_t)();
        SteamAPI_Init_t SteamAPI_Init = playeveryware::eos::temp::load_function_with_name<SteamAPI_Init_t>(steam_dll_handle, "SteamAPI_Init");

        if (SteamAPI_Init())
        {
            log_inform("Called SteamAPI_Init with success!");
        }
    }
}

//-------------------------------------------------------------------------
void eos_set_loglevel_via_config()
{
    if (playeveryware::eos::temp::EOS_Logging_SetLogLevel_ptr == nullptr)
    {
        return;
    }

    auto path_to_log_config_json = get_path_for_eos_service_config(EOS_LOGLEVEL_CONFIG_FILENAME);

    if (!std::filesystem::exists(path_to_log_config_json))
    {
        log_inform("Log level config not found, using default log levels");
        return;
    }

    json_value_s* log_config_as_json = read_config_json_as_json_from_path(path_to_log_config_json);
    LogLevelConfig log_config = log_config_from_json_value(log_config_as_json);
    free(log_config_as_json);

    // Validation to prevent out of range exception
    if (log_config.category.size() != log_config.level.size())
    {
        log_warn("Log level config entries out of range");
        return;
    }

    // Last in the vector is AllCategories, and will not be set
    size_t individual_category_size = log_config.category.size() > 0 ? log_config.category.size() - 1 : 0;
    if (individual_category_size == 0)
    {
        log_warn("Log level config entries empty");
        return;
    }

    for (size_t i = 0; i < individual_category_size; i++)
    {
        playeveryware::eos::temp::EOS_Logging_SetLogLevel_ptr((EOS_ELogCategory)i, eos_loglevel_str_to_enum(log_config.level[i]));
    }

    log_inform("Log levels set according to config");
}

//-------------------------------------------------------------------------
void eos_create(EOSConfig& eosConfig)
{
    EOS_Platform_Options platform_options = {0};
    platform_options.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
    platform_options.bIsServer = eosConfig.isServer;
    platform_options.Flags = eosConfig.flags;
    platform_options.CacheDirectory = GetCacheDirectory();

    platform_options.EncryptionKey = eosConfig.encryptionKey.length() > 0 ? eosConfig.encryptionKey.c_str() : nullptr;
    platform_options.OverrideCountryCode = eosConfig.overrideCountryCode.length() > 0 ? eosConfig.overrideCountryCode.c_str() : nullptr;
    platform_options.OverrideLocaleCode = eosConfig.overrideLocaleCode.length() > 0 ? eosConfig.overrideLocaleCode.c_str() : nullptr;
    platform_options.ProductId = eosConfig.productID.c_str();
    platform_options.SandboxId = eosConfig.sandboxID.c_str();
    platform_options.DeploymentId = eosConfig.deploymentID.c_str();
    platform_options.ClientCredentials.ClientId = eosConfig.clientID.c_str();
    platform_options.ClientCredentials.ClientSecret = eosConfig.clientSecret.c_str();

    platform_options.TickBudgetInMilliseconds = eosConfig.tickBudgetInMilliseconds;

    if (eosConfig.taskNetworkTimeoutSeconds > 0)
    {
        platform_options.TaskNetworkTimeoutSeconds = &eosConfig.taskNetworkTimeoutSeconds;
    }

    EOS_Platform_RTCOptions rtc_options = { 0 };

    rtc_options.ApiVersion = EOS_PLATFORM_RTCOPTIONS_API_LATEST;
#if PLATFORM_WINDOWS
    log_inform("setting up rtc");
    fs::path xaudio2_dll_path = playeveryware::eos::io_helpers::get_path_relative_to_current_module(XAUDIO2_DLL_NAME);
    std::string xaudio2_dll_path_as_string = to_utf8_str(xaudio2_dll_path);
    EOS_Windows_RTCOptions windows_rtc_options = { 0 };
    windows_rtc_options.ApiVersion = EOS_WINDOWS_RTCOPTIONS_API_LATEST;
    windows_rtc_options.XAudio29DllPath = xaudio2_dll_path_as_string.c_str();
    log_warn(xaudio2_dll_path_as_string.c_str());

    if (!fs::exists(xaudio2_dll_path))
    {
        log_warn("Missing XAudio dll!");
    }
    rtc_options.PlatformSpecificOptions = &windows_rtc_options;
    platform_options.RTCOptions = &rtc_options;
#endif

#if PLATFORM_WINDOWS
    auto path_to_steam_config_json = get_path_for_eos_service_config(EOS_STEAM_CONFIG_FILENAME);

    // Defined here so that the override path lives long enough to be referenced by the create option
    EOSSteamConfig eos_steam_config;
    EOS_IntegratedPlatform_Options steam_integrated_platform_option = { 0 };
    EOS_IntegratedPlatform_Steam_Options steam_platform = { 0 };
    EOS_HIntegratedPlatformOptionsContainer integrated_platform_options_container = nullptr;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    if (std::filesystem::exists(path_to_steam_config_json))
    {
        json_value_s* eos_steam_config_as_json = nullptr;
        eos_steam_config_as_json = read_config_json_as_json_from_path(path_to_steam_config_json);
        eos_steam_config = eos_steam_config_from_json_value(eos_steam_config_as_json);
        free(eos_steam_config_as_json);

        if (eos_steam_config.OverrideLibraryPath.has_value())
        {
            if (!std::filesystem::exists(eos_steam_config.OverrideLibraryPath.value()))
            {
                auto override_lib_path_as_str = basename(eos_steam_config.OverrideLibraryPath.value());
                auto found_steam_path = playeveryware::eos::io_helpers::get_path_relative_to_current_module(override_lib_path_as_str);

                // Fall back and use the steam dll name based on the
                // type of binary the GfxPluginNativeRender
                if (!std::filesystem::exists(found_steam_path) || eos_steam_config.OverrideLibraryPath.value().empty())
                {
                    found_steam_path = playeveryware::eos::io_helpers::get_path_relative_to_current_module(STEAM_DLL_NAME);
                }

                if (std::filesystem::exists(found_steam_path))
                {
                    eos_steam_config.OverrideLibraryPath = converter.to_bytes(found_steam_path.wstring());
                }
            }
        }
        else
        {
            auto found_steam_path = playeveryware::eos::io_helpers::get_path_relative_to_current_module(STEAM_DLL_NAME);
            if (std::filesystem::exists(found_steam_path))
            {
                eos_steam_config.OverrideLibraryPath = converter.to_bytes(found_steam_path.wstring());
            }
        }

        if (eos_steam_config.isManagedByApplication())
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
            log_error("Size given for SteamApiInterfaceVersionsAsCharArray exceeds the maximum value.");
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
        playeveryware::eos::temp::EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_ptr(&options, &integrated_platform_options_container);
        platform_options.IntegratedPlatformOptionsContainerHandle = integrated_platform_options_container;

        EOS_IntegratedPlatformOptionsContainer_AddOptions addOptions = { EOS_INTEGRATEDPLATFORMOPTIONSCONTAINER_ADD_API_LATEST };
        addOptions.Options = &steam_integrated_platform_option;
        playeveryware::eos::temp::EOS_IntegratedPlatformOptionsContainer_Add_ptr(integrated_platform_options_container, &addOptions);
    }
#endif

    //EOS_Platform_Options_debug_log(platform_options);
    log_inform("run EOS_Platform_Create");
    playeveryware::eos::temp::eos_platform_handle = playeveryware::eos::temp::EOS_Platform_Create_ptr(&platform_options);
    if (integrated_platform_options_container)
    {
        playeveryware::eos::temp::EOS_IntegratedPlatformOptionsContainer_Release_ptr(integrated_platform_options_container);
    }

    if (!playeveryware::eos::temp::eos_platform_handle)
    {
        log_error("failed to create the platform");
    }
}

//-------------------------------------------------------------------------
static bool QueryRegKey(const HKEY InKey, const TCHAR* InSubKey, const TCHAR* InValueName, std::wstring& OutData)
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
            ::DWORD Size = 0;
            // First, we'll call RegQueryValueEx to find out how large of a buffer we need
            if ((RegQueryValueEx(Key, InValueName, NULL, NULL, NULL, &Size) == ERROR_SUCCESS) && Size)
            {
                // Allocate a buffer to hold the value and call the function again to get the data
                char *Buffer = new char[Size];
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

//-------------------------------------------------------------------------
// Currently this only works on windows
static bool get_overlay_dll_path(fs::path* OutDllPath)
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

    *OutDllPath = fs::path(OverlayDllDirectory) / OVERLAY_DLL_NAME;
    return fs::exists(*OutDllPath) && fs::is_regular_file(*OutDllPath);
#else
    log_inform("Trying to get a DLL path on a platform without DLL paths searching");
    return false;
#endif
}



//-------------------------------------------------------------------------
// Called by unity on load. It kicks off the work to load the DLL for Overlay
#if PLATFORM_32BITS
#pragma comment(linker, "/export:UnityPluginLoad=_UnityPluginLoad@4")
#endif
DLL_EXPORT(void) UnityPluginLoad(void*)
{
#if _DEBUG
    show_log_as_dialog("You may attach a debugger to the DLL");
#endif

    auto path_to_config_json = get_path_for_eos_service_config(EOS_SERVICE_CONFIG_FILENAME);
    json_value_s* eos_config_as_json = nullptr;

    eos_config_as_json = read_config_json_from_dll();

    if (!eos_config_as_json && std::filesystem::exists(path_to_config_json))
    {
        eos_config_as_json = read_config_json_as_json_from_path(path_to_config_json);
    }

    if (!eos_config_as_json)
    {
        log_warn("Failed to load a valid json config for EOS");
        return;
    }

    EOSConfig eos_config = eos_config_from_json_value(eos_config_as_json);
    free(eos_config_as_json);

#if PLATFORM_WINDOWS
    //support sandbox and deployment id override via command line arguments
    std::stringstream argStream = std::stringstream(GetCommandLineA());
    std::istream_iterator<std::string> argsBegin(argStream);
    std::istream_iterator<std::string> argsEnd;
    std::vector<std::string> argStrings(argsBegin, argsEnd);
    std::string egsArgName = "-epicsandboxid=";
    std::string sandboxArgName = "-eossandboxid=";
    for (unsigned i = 0; i < argStrings.size(); ++i)
    {
        std::string* match = nullptr;
        if (argStrings[i]._Starts_with(sandboxArgName))
        {
            match = &sandboxArgName;
        }
        else if(argStrings[i]._Starts_with(egsArgName))
        {
            match = &egsArgName;
        }
        if (match != nullptr)
        {
            std::string sandboxArg = argStrings[i].substr(match->length());
            if (!sandboxArg.empty())
            {
                log_inform(("Sandbox ID override specified: " + sandboxArg).c_str());
                eos_config.sandboxID = sandboxArg;
            }
        }
    }
#endif

    //check if a deployment id override exists for sandbox id
    for (unsigned i = 0; i < eos_config.sandboxDeploymentOverrides.size(); ++i)
    {
        if (eos_config.sandboxID == eos_config.sandboxDeploymentOverrides[i].sandboxID)
        {
            log_inform(("Sandbox Deployment ID override specified: " + eos_config.sandboxDeploymentOverrides[i].deploymentID).c_str());
            eos_config.deploymentID = eos_config.sandboxDeploymentOverrides[i].deploymentID;
        }
    }

#if PLATFORM_WINDOWS
    std::string deploymentArgName = "-eosdeploymentid=";
    std::string egsDeploymentArgName = "-epicdeploymentid=";
    for (unsigned i = 0; i < argStrings.size(); ++i)
    {
        std::string* match = nullptr;
        if (argStrings[i]._Starts_with(deploymentArgName))
        {
            match = &deploymentArgName;
        }
        else if (argStrings[i]._Starts_with(egsDeploymentArgName))
        {
            match = &egsDeploymentArgName;
        }
        if (match != nullptr)
        {
            std::string deploymentArg = argStrings[i].substr(match->length());
            if (!deploymentArg.empty())
            {
                log_inform(("Deployment ID override specified: " + deploymentArg).c_str());
                eos_config.deploymentID = deploymentArg;
            }
        }
    }
#endif

#if _DEBUG
    global_log_open("gfx_log.txt");
#endif

    fs::path DllPath;
    log_inform("On UnityPluginLoad");
    //if (!get_overlay_dll_path(&DllPath))
    //{
    //    show_log_as_dialog("Missing Overlay DLL!\n Overlay functionality will not work!");
    //}

    playeveryware::eos::temp::s_eos_sdk_lib_handle = playeveryware::eos::temp::load_library_at_path(playeveryware::eos::io_helpers::get_path_relative_to_current_module(SDK_DLL_NAME));

    //eos_sdk_overlay_lib_handle = load_library_at_path(DllPath);
    //if (eos_sdk_overlay_lib_handle)
    //{
    //    log_warn("loaded eos overlay sdk");
    //    FuncApplicationWillShutdown = load_function_with_name<FSig_ApplicationWillShutdown>(eos_sdk_overlay_lib_handle, "EOS_Overlay_Initilize");
    //    if(FuncApplicationWillShutdown == nullptr)
    //    {
    //        log_warn("Unable to find overlay function");
    //    }
    //}

    if (playeveryware::eos::temp::s_eos_sdk_lib_handle)
    {
        playeveryware::eos::temp::FetchEOSFunctionPointers();

        if (playeveryware::eos::temp::EOS_Initialize_ptr)
        {
            log_inform("start eos init");

            eos_init(eos_config);

            eos_set_loglevel_via_config();
            //log_warn("start eos create");
            eos_create(eos_config);

            // This code is commented out because the handle is now handed off to the C# code
            //EOS_Platform_Release(eos_platform_handle);
            //eos_platform_handle = NULL;
            //log_warn("start eos shutdown");
            //EOS_Shutdown();
            //log_warn("unload eos sdk");
            //unload_library(s_eos_sdk_lib_handle);

            playeveryware::eos::temp::s_eos_sdk_lib_handle = NULL;
            playeveryware::eos::temp::EOS_Initialize_ptr = NULL;
            playeveryware::eos::temp::EOS_Shutdown_ptr = NULL;
            playeveryware::eos::temp::EOS_Platform_Create_ptr = NULL;
        }
        else
        {
            log_warn("unable to find EOS_Initialize");
        }

    }
    else
    {
        log_warn("Couldn't find dll "  SDK_DLL_NAME);
    }

}

//-------------------------------------------------------------------------
#if PLATFORM_32BITS
#pragma comment(linker, "/export:_UnityPluginUnload=_UnityPluginUnload@0")
#endif
DLL_EXPORT(void) UnityPluginUnload()
{
    if (FuncApplicationWillShutdown != nullptr)
    {
        FuncApplicationWillShutdown();
    }
    unload_library(playeveryware::eos::temp::s_eos_sdk_overlay_lib_handle);
    playeveryware::eos::temp::s_eos_sdk_overlay_lib_handle = nullptr;

    global_log_close();
}

//-------------------------------------------------------------------------
DLL_EXPORT(void *) EOS_GetPlatformInterface()
{
    return playeveryware::eos::temp::eos_platform_handle;
}
