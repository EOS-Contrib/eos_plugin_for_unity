#ifndef WINDOWS_CONFIG_HPP
#define WINDOWS_CONFIG_HPP

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

#pragma once

#include "include/Config/PlatformConfig.hpp"

namespace pew::eos::config
{
    struct WindowsConfig final : PlatformConfig
    {
        ~WindowsConfig()
        {
            delete static_cast<EOS_Windows_RTCOptions*>(platform_specific_rtc_options);
        }

        void set_cache_directory() override
        {
            if (cache_directory.empty())
            {
                WCHAR tmp_buffer = 0;
                DWORD buffer_size = GetTempPathW(1, &tmp_buffer) + 1;
                WCHAR* lpTempPathBuffer = (TCHAR*)malloc(buffer_size * sizeof(TCHAR));
                GetTempPathW(buffer_size, lpTempPathBuffer);

                cache_directory = string_helpers::create_utf8_str_from_wide_str(lpTempPathBuffer);
                free(lpTempPathBuffer);
            }
        }

        void set_platform_specific_rtc_options() override
        {
            if (rtc_options == nullptr)
            {
                logging::log_error("Attempting to set platform specific rtc options, but rtc options are null.");
                return;
            }

            if (rtc_options->PlatformSpecificOptions == nullptr)
            {
                platform_specific_rtc_options = new EOS_Windows_RTCOptions();

                const auto windows_rtc_options = static_cast<EOS_Windows_RTCOptions*>(platform_specific_rtc_options);

                windows_rtc_options->ApiVersion = EOS_WINDOWS_RTCOPTIONS_API_LATEST;

                const auto xaudio2_dll_path = absolute(io_helpers::get_path_relative_to_current_module(XAUDIO2_DLL_NAME));

                if (!exists(xaudio2_dll_path))
                {
                    logging::log_warn("Missing XAudio dll!");
                }

                size_t len = xaudio2_dll_path.string().size() + 1;
                _xaudio2_dll_path = std::shared_ptr<char[]>(new char[len]);

                strcpy_s(_xaudio2_dll_path.get(), len, xaudio2_dll_path.string().c_str());

                windows_rtc_options->XAudio29DllPath = _xaudio2_dll_path.get();

                rtc_options->PlatformSpecificOptions = platform_specific_rtc_options;
            }
        }

    private:

        std::shared_ptr<char[]> _xaudio2_dll_path;

        explicit WindowsConfig() : PlatformConfig("eos_windows_config.json") 
        {
            initialize();
        }

        // Makes the WindowsConfig constructor accessible to the Config class.
        friend struct Config;
    };
}

#endif
