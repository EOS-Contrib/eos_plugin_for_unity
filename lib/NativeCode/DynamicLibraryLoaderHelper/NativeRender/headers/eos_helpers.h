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
#include "headers/PEW_EOS_Defines.h"

namespace std
{
    namespace filesystem
    {
        class path;
    }
}

namespace pew::eos
{
    namespace config_legacy
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
    PEW_EOS_API_FUNC(void*) EOS_GetPlatformInterface();

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
    void eos_init(const config_legacy::EOSConfig eos_config);

    /**
     * @brief Creates an EOS platform using the specified configuration.
     *
     * Configures and creates an EOS platform instance. This includes setting up RTC options,
     * integrated platform options, and other settings defined in the configuration.
     *
     * @param eos_config The configuration object containing EOS platform settings.
     */
    void eos_create(config_legacy::EOSConfig eos_config);
}
#endif
