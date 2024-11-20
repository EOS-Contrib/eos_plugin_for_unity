#ifndef EOS_HELPERS_H
#define EOS_HELPERS_H
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

#pragma once
#include <iostream>
#include <sstream>
#include <vector>

#include "Config/PlatformConfig.h"
#include "Config/ProductConfig.h"
#include "logging.h"
#include "io_helpers.h"

namespace std
{
    namespace filesystem
    {
        class path;
    }
}

namespace pew::eos
{
    class EOSWrapper;

    namespace config
    {
        class ProductConfig;
        struct EOSConfig;
    }

    /**
     * @brief Loads a dynamic library from the specified file path.
     *
     * Attempts to load the library at the specified path and returns a handle to it.
     * On Windows, it uses `LoadLibrary` to perform the loading.
     *
     * @param library_path The file path to the library to load.
     * @return A handle to the loaded library, or `nullptr` if loading fails.
     */
    void* load_library_at_path(const std::filesystem::path& library_path);

    /**
     * @brief Queries a registry key for a specific value on Windows.
     *
     * Attempts to try_read a value from the specified registry key and subkey. It supports both
     * 32-bit and 64-bit registry views.
     *
     * @param InKey The registry key handle.
     * @param InSubKey The name of the subkey to query.
     * @param InValueName The name of the value to retrieve.
     * @param OutData The output parameter to store the retrieved data.
     * @return `true` if the value was successfully retrieved, `false` otherwise.
     */
    bool QueryRegKey(const HKEY InKey, const TCHAR* InSubKey, const TCHAR* InValueName, std::wstring& OutData);

    /**
     * @brief Retrieves the path to the overlay DLL.
     *
     * Attempts to retrieve the overlay DLL path from the system registry on Windows.
     *
     * @param[out] OutDllPath The output parameter where the overlay DLL path is stored.
     * @return `true` if the DLL path was found and exists; otherwise, `false`.
     */
    static bool get_overlay_dll_path(std::filesystem::path* OutDllPath);

    /**
     * @brief Retrieves the EOS platform interface handle.
     *
     * Provides access to the EOS platform interface handle managed by the library.
     *
     * @return A pointer to the EOS platform interface handle.
     */
    DLL_EXPORT(void*) EOS_GetPlatformInterface();

    /**
     * @brief Sets the log level for the EOS SDK based on the configuration file.
     *
     * Reads the log level configuration file and applies specified log levels to each
     * log category in the EOS SDK. If the configuration file is missing or the entries are
     * invalid, default log levels are used.
     */
    void set_eos_loglevel(const EOSWrapper& eos_sdk);

    /**
     * @brief Logs EOS platform options for debugging purposes.
     *
     * Logs various properties of the EOS platform options structure, including version,
     * server mode, flags, cache directory, and client credentials, for debugging.
     *
     * @param platform_options The EOS platform options structure to log.
     */
    void EOS_Platform_Options_debug_log(const EOS_Platform_Options& platform_options);

    /**
     * @brief Initializes the EOS SDK and creates an EOS platform using the specified configurations.
     *
     * Sets up and initializes the EOS SDK using the provided configuration. Sets log levels and
     * a logging callback if configured. If initialization fails, an error is logged.
     *
     * @param platform_config The configuration for Windows.
     * @param product_config The configuration for the product.
     */
    CONFIG_API EOS_HPlatform load_eos(const config::PlatformConfig& platform_config, const config::ProductConfig& product_config);

    /**
     * @brief Initializes the EOS SDK with the provided configuration.
     *
     * Sets up and initializes the EOS SDK using the provided configuration. Sets log levels and
     * a logging callback if configured. If initialization fails, an error is logged.
     *
     * @param eos_sdk Reference to the eos sdk.
     * @param platform_config The configuration for Windows.
     * @param product_config The configuration for the product.
     */
    void eos_init(const EOSWrapper& eos_sdk, const config::PlatformConfig& platform_config, const config::ProductConfig& product_config);

    /**
     * @brief Creates an EOS platform using the specified configuration.
     *
     * Configures and creates an EOS platform instance. This includes setting up RTC options,
     * integrated platform options, and other settings defined in the configuration.
     *
     * @param eos_sdk Reference to the eos sdk.
     * @param platform_config The configuration object containing EOS platform settings.
     * @param product_config The configuration object containing product settings.
     */
    EOS_HPlatform eos_create(const EOSWrapper& eos_sdk, const config::PlatformConfig& platform_config, const config::ProductConfig& product_config);

    /**
     * \brief Applies any command line arguments that may have been provided.
     * \param platform_config The platform config whose values may need to be
     * overridden by command line arguments.
     * \param product_config The product config. This is used to warn the user if
     * the provided sandbox id or deployment id is not defined in the product
     * config. If they are not defined, they will still be applied.
     */
    static void apply_cli_arguments(config::PlatformConfig& platform_config, const config::ProductConfig& product_config)
    {
        //support sandbox and deployment id override via command line arguments
        std::stringstream argument_stream = std::stringstream(GetCommandLineA());
        const std::istream_iterator<std::string> argument_stream_begin(argument_stream);
        const std::istream_iterator<std::string> argument_stream_end;
        const std::vector argument_strings(argument_stream_begin, argument_stream_end);

        std::string sandbox_id_override;
        if (io_helpers::try_get_command_line_argument(argument_strings, sandbox_id_override, "epicsandboxid", "eossandboxid"))
        {
            if (!product_config.environments.is_sandbox_defined(sandbox_id_override))
            {
                std::cerr << "Sandbox Id \"" << sandbox_id_override << "\" was provided on the command line, but is not found in the product config. Attempting to use it regardless." << std::endl;
            }
            platform_config.deployment.sandbox.id = sandbox_id_override;
        }

        std::string deployment_id_override;
        if (io_helpers::try_get_command_line_argument(argument_strings, deployment_id_override, "eosdeploymentid", "epicdeploymentid"))
        {
            if (!product_config.environments.is_deployment_defined(deployment_id_override))
            {
                std::cerr << "Deployment Id \"" << deployment_id_override << "\" was provided on the command line, but is not found in the product config. Attempting to use it regardless." << std::endl;
            }
            platform_config.deployment.id = deployment_id_override;
        }
    }

    /**
     * @brief Loads and initializes the Steam API DLL using a string path.
     *
     * Attempts to load the Steam API DLL from the specified path. If the DLL is not already
     * loaded, this function tries to load it and then calls `SteamAPI_Init`.
     *
     * @param steam_dll_path The string path to the Steam API DLL.
     */
    inline static void eos_call_steam_init(const std::string& steam_dll_path)
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
           /* if (try_load_function(steam_dll_handle, "SteamAPI_Init", SteamAPI_Init) && SteamAPI_Init())
            {
                logging::log_inform("Called SteamAPI_Init with success!");
            }*/
        }
    }

    /**
     * @brief Loads the Steam API DLL from a specified path.
     *
     * Loads the Steam API DLL and initializes it if necessary. Attempts to load the DLL from
     * the specified path, or defaults to `steam_api.dll` if no path is specified.
     *
     * This function assumes that if the caller has already loaded the steam
     * DLL, that SteamAPI_Init doesn't need to be called
     *
     * @param steam_dll_path The path to the Steam API DLL.
     */
    inline static void eos_call_steam_init(const std::filesystem::path& steam_dll_path)
    {
        const std::string steam_dll_path_as_string = steam_dll_path.string();
        eos_call_steam_init(steam_dll_path_as_string);
    }
}
#endif
