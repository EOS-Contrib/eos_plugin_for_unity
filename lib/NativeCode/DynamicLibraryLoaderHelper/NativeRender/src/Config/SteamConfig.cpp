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
#include "PlatformConfig.h"
#include "SteamConfig.h"
#include <iostream>
#include <filesystem>
#include "io_helpers.h"
#include "EOSJsonConverterMethods.h"

namespace pew::eos::config
{
    void SteamConfig::from_json(const json& json)
    {
        const auto json_string = json.dump();

        int version_major;
        json["steamSDKMajorVersion"].get_to(version_major);
        steam_sdk_major_version = static_cast<uint32_t>(version_major);

        int version_minor;
        json["steamSDKMinorVersion"].get_to(version_minor);
        steam_sdk_minor_version = static_cast<uint32_t>(version_minor);

        
        if (!json["overrideLibraryPath"].is_null())
        {
            std::string library_path = json["overrideLibraryPath"].get<std::string>();
            override_library_path = std::filesystem::path(library_path);
        }

        json["integratedPlatformManagementFlags"].get_to(integrated_platform_management_flags);

        json["steamApiInterfaceVersionsArray"].get_to(steam_api_interface_versions_array);
        std::cout << json_string << std::endl;
    }

    void SteamConfig::migrate()
    {
    }

    std::filesystem::path SteamConfig::get_config_path(const char* file_name)
    {
        return absolute(io_helpers::get_path_relative_to_current_module(std::filesystem::path(
#ifdef _DEBUG
            "../../../../../../etc/config/"
#endif
#ifdef NDEBUG
            "../../../etc/config/"
#endif
        ) / file_name));
    }
}
