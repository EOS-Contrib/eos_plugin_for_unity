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
#include "config_legacy.h"
#include "eos_library_helpers.h"
#include "io_helpers.h"
#include "json_helpers.h"
#include "logging.h"

using namespace pew::eos::config_legacy;
using namespace pew::eos::json_helpers;

namespace pew::eos::config_legacy
{
    /**
     * \brief Function that gets the config as a JSON string.
     */
    static GetConfigAsJSONString_t GetConfigAsJSONString;

    bool EOSSteamConfig::is_managed_by_application() const
    {
        return static_cast<std::underlying_type_t<EOS_EIntegratedPlatformManagementFlags>>(flags &
            EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedByApplication);
    }

    bool EOSSteamConfig::is_managed_by_sdk() const
    {
        return static_cast<std::underlying_type_t<EOS_EIntegratedPlatformManagementFlags>>(flags &
            EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedBySDK);
    }

    LogLevelConfig log_config_from_json_value(json_value_s* config_json)
    {
        json_object_s* config_json_object = json_value_as_object(config_json);
        json_object_element_s* iter = config_json_object->start;
        LogLevelConfig log_config;

        while (iter != nullptr)
        {
            if (!strcmp("LogCategoryLevelPairs", iter->name->string))
            {
                json_array_s* pairs = json_value_as_array(iter->value);
                for (auto e = pairs->start; e != nullptr; e = e->next)
                {
                    json_object_s* pairs_json_object = json_value_as_object(e->value);
                    json_object_element_s* pairs_iter = pairs_json_object->start;
                    while (pairs_iter != nullptr)
                    {
                        if (!strcmp("Category", pairs_iter->name->string))
                        {
                            log_config.category.push_back(json_value_as_string(pairs_iter->value)->string);
                        }
                        else if (!strcmp("Level", pairs_iter->name->string))
                        {
                            log_config.level.push_back(json_value_as_string(pairs_iter->value)->string);
                        }
                        pairs_iter = pairs_iter->next;
                    }
                }
            }
            iter = iter->next;
        }

        return log_config;
    }

    /**
     * \brief Gets the fully-qualified path to a config file. Uses
     * CONFIG_DIRECTORY in concert with the determined path of the current
     * module to determine the fully qualified path.
     * \param config_filename The file name to get the path for.
     * \return A fully qualified path to the config file indicated.
     */
    std::filesystem::path get_path_for_eos_service_config(const std::string& config_filename)
    {
        // Get the config file path using CONFIG_DIRECTORY
        const auto relative_config_file_path = std::filesystem::path(CONFIG_DIRECTORY) / config_filename;

        // Get the path of the file relative to the current module
        const auto config_file_path_relative_to_module = io_helpers::get_path_relative_to_current_module(relative_config_file_path);

        // Return the absolute path of the relative path determined above.
        return absolute(config_file_path_relative_to_module);
    }

    json_value_s* read_config_json_from_dll()
    {
        json_value_s* config_json = nullptr;

#if ENABLE_DLL_BASED_EOS_CONFIG
        logging::log_inform("Trying to load eos config via dll");
        static void* eos_generated_library_handle = eos_library_helpers::load_library_at_path(io_helpers::get_path_relative_to_current_module("EOSGenerated.dll"));

        if (!eos_generated_library_handle)
        {
            logging::log_warn("No Generated DLL found (Might not be an error)");
            return NULL;
        }

        GetConfigAsJSONString = eos_library_helpers::load_function_with_name<GetConfigAsJSONString_t>(eos_generated_library_handle, "GetConfigAsJSONString");

        if (GetConfigAsJSONString)
        {
            const char* config_as_json_string = GetConfigAsJSONString();
            if (config_as_json_string != nullptr)
            {
                size_t config_as_json_string_length = strlen(config_as_json_string);
                config_json = json_parse(config_as_json_string, config_as_json_string_length);
            }
        }
        else
        {
            logging::log_warn("No function found");
        }
#endif

        return config_json;
    }

    EOS_EIntegratedPlatformManagementFlags eos_collect_integrated_platform_management_flags(json_object_element_s* iter)
    {
        return json_helpers::collect_flags<EOS_EIntegratedPlatformManagementFlags>(
            &(INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_STRINGS_TO_ENUM),
            EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_Disabled,
            iter);
    }

    EOSSteamConfig eos_steam_config_from_json_value(json_value_s* config_json)
    {
        json_object_s* config_json_object = json_value_as_object(config_json);
        json_object_element_s* iter = config_json_object->start;
        EOSSteamConfig eos_config;

        while (iter != nullptr)
        {
            if (!strcmp("flags", iter->name->string))
            {
                eos_config.flags = eos_collect_integrated_platform_management_flags(iter);

            }
            else if (!strcmp("overrideLibraryPath", iter->name->string))
            {
                const char* override_library_path = json_value_as_string(iter->value)->string;

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
}
