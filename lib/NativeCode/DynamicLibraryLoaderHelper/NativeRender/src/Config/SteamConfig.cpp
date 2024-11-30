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
#include "include/Config/SteamConfig.h"
#include <filesystem>

#include "io_helpers.h"

namespace pew::eos::config
{
    bool SteamConfig::try_get_library_path(std::filesystem::path& library_path) const
    {
        if(exists(_override_library_path))
        {
            library_path = absolute(_override_library_path);
            return true;
        }

        if (exists(_library_path))
        {
            library_path = absolute(_library_path);
            return true;
        }

        return false;
    }

    const char* SteamConfig::get_steam_api_interface_versions_array() const
    {
        // For each element in the array (each of which is a string of an api version information)
        // iterate across each character, and at the end of a string add a null terminator \0
        // then add one more null terminator at the end of the array
        std::vector<char> steamApiInterfaceVersionsAsCharArray;

        for (const auto& currentFullValue : _steam_api_interface_versions_array)
        {
            for (char currentCharacter : currentFullValue)
            {
                steamApiInterfaceVersionsAsCharArray.push_back(currentCharacter);
            }

            steamApiInterfaceVersionsAsCharArray.push_back('\0');
        }
        steamApiInterfaceVersionsAsCharArray.push_back('\0');

        return steamApiInterfaceVersionsAsCharArray.data();
    }
}
