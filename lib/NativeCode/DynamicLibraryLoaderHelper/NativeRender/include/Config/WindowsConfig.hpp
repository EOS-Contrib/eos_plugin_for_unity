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
        ~WindowsConfig() override = default;

        const char* get_cache_directory() const override
        {
            if (s_cache_directory.empty())
            {
                WCHAR tmp_buffer = 0;
                DWORD buffer_size = GetTempPathW(1, &tmp_buffer) + 1;
                WCHAR* lpTempPathBuffer = (TCHAR*)malloc(buffer_size * sizeof(TCHAR));
                GetTempPathW(buffer_size, lpTempPathBuffer);

                s_cache_directory = string_helpers::create_utf8_str_from_wide_str(lpTempPathBuffer);
                free(lpTempPathBuffer);
            }

            return s_cache_directory.c_str();
        }
    private:
        explicit WindowsConfig() : PlatformConfig("eos_windows_config.json") 
        {
            get_cache_directory();
        }
        // Makes the WindowsConfig constructor accessible to the Config class.
        friend struct Config;
    };
}

#endif
