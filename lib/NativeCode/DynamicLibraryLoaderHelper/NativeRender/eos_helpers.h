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
#include <string>

namespace std::filesystem
{
    class path;
}

namespace pew::eos
{
    namespace config
    {
        struct EOSConfig;
    }

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
    void eos_set_loglevel_via_config();

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
     * @brief Initializes the EOS SDK with the provided configuration.
     *
     * Sets up and initializes the EOS SDK using the provided configuration. Sets log levels and
     * a logging callback if configured. If initialization fails, an error is logged.
     *
     * @param eos_config The EOS configuration settings.
     */
    void eos_init(const config::EOSConfig& eos_config);

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
    void eos_call_steam_init(const std::filesystem::path& steam_dll_path);

    /**
     * @brief Loads and initializes the Steam API DLL using a string path.
     *
     * Attempts to load the Steam API DLL from the specified path. If the DLL is not already
     * loaded, this function tries to load it and then calls `SteamAPI_Init`.
     *
     * @param steam_dll_path The string path to the Steam API DLL.
     */
    void eos_call_steam_init(const std::string& steam_dll_path);

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
     * @brief Creates an EOS platform using the specified configuration.
     *
     * Configures and creates an EOS platform instance. This includes setting up RTC options,
     * integrated platform options, and other settings defined in the configuration.
     *
     * @param eosConfig The configuration object containing EOS platform settings.
     */
    void eos_create(config::EOSConfig& eosConfig);
}
#endif
