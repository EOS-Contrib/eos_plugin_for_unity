#ifndef STEAM_CONFIG_H
#define STEAM_CONFIG_H
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

#include "include/Config/ConfigBase.h"
#include "include/Config/common.hpp"

namespace pew::eos::config
{
    struct SteamConfig final : public ConfigBase
    {
    private:
        std::filesystem::path _library_path;
        std::filesystem::path _override_library_path;
        std::vector<std::string> _steam_api_interface_versions_array;

        explicit SteamConfig() : ConfigBase(get_config_path("eos_plugin_steam_config.json")),
            steam_sdk_major_version(0),
            steam_sdk_minor_version(0)
        {
            // TODO: Restore STEAM_SDK_DLL_NAME
            //_library_path = common::get_path_relative_to_current_module(STEAM_SDK_DLL_NAME);
        }

        void from_json(const nlohmann::json& json) override;
        std::filesystem::path get_config_path(const char* file_name) override;

        friend struct ConfigBase;

    public:
        
        bool try_get_library_path(std::filesystem::path& library_path) const;
        uint32_t steam_sdk_major_version;
        uint32_t steam_sdk_minor_version;
        const char* get_steam_api_interface_versions_array() const;
        EOS_EIntegratedPlatformManagementFlags integrated_platform_management_flags = EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_Disabled;

    };
}

#endif
