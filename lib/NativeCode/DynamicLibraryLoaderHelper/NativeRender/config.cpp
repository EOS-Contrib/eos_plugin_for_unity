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
#include "config.h"
#include "eos_library_helpers.h"
#include "io_helpers.h"
#include "json_helpers.h"
#include "logging.h"

namespace playeveryware::eos::config
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

    std::filesystem::path get_path_for_eos_service_config(std::string config_filename)
    {
        auto twoDirsUp = std::filesystem::path("../..");
        std::filesystem::path packaged_data_path = io_helpers::get_path_relative_to_current_module(twoDirsUp);
        std::error_code error_code;

        logging::log_inform("about to look with exists");
        if (!std::filesystem::exists(packaged_data_path, error_code))
        {
            logging::log_warn("Didn't find the path twoDirsUp");
            packaged_data_path = io_helpers::get_path_relative_to_current_module(std::filesystem::path("./Data/"));
        }

        return packaged_data_path / "StreamingAssets" / "EOS" / config_filename;
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

    //-------------------------------------------------------------------------
    EOSConfig eos_config_from_json_value(json_value_s* config_json)
    {
        // Create platform instance
        json_object_s* config_json_object = json_value_as_object(config_json);
        json_object_element_s* iter = config_json_object->start;
        EOSConfig eos_config;

        while (iter != nullptr)
        {
            if (!strcmp("productName", iter->name->string))
            {
                eos_config.productName = json_value_as_string(iter->value)->string;
            }
            else if (!strcmp("productVersion", iter->name->string))
            {
                eos_config.productVersion = json_value_as_string(iter->value)->string;
            }
            else if (!strcmp("productID", iter->name->string))
            {
                eos_config.productID = json_value_as_string(iter->value)->string;
            }
            else if (!strcmp("sandboxID", iter->name->string))
            {
                eos_config.sandboxID = json_value_as_string(iter->value)->string;
            }
            else if (!strcmp("deploymentID", iter->name->string))
            {
                eos_config.deploymentID = json_value_as_string(iter->value)->string;
            }
            else if (!strcmp("sandboxDeploymentOverrides", iter->name->string))
            {
                json_array_s* overrides = json_value_as_array(iter->value);
                eos_config.sandboxDeploymentOverrides = std::vector<SandboxDeploymentOverride>();
                for (auto e = overrides->start; e != nullptr; e = e->next)
                {
                    json_object_s* override_json_object = json_value_as_object(e->value);
                    json_object_element_s* ov_iter = override_json_object->start;
                    SandboxDeploymentOverride override_item = SandboxDeploymentOverride();
                    while (ov_iter != nullptr)
                    {
                        if (!strcmp("sandboxID", ov_iter->name->string))
                        {
                            override_item.sandboxID = json_value_as_string(ov_iter->value)->string;
                        }
                        else if (!strcmp("deploymentID", ov_iter->name->string))
                        {
                            override_item.deploymentID = json_value_as_string(ov_iter->value)->string;
                        }
                        ov_iter = ov_iter->next;
                    }
                    eos_config.sandboxDeploymentOverrides.push_back(override_item);
                }
            }
            else if (!strcmp("clientID", iter->name->string))
            {
                eos_config.clientID = json_value_as_string(iter->value)->string;
            }
            else if (!strcmp("clientSecret", iter->name->string))
            {
                eos_config.clientSecret = json_value_as_string(iter->value)->string;
            }
            if (!strcmp("encryptionKey", iter->name->string))
            {
                eos_config.encryptionKey = json_value_as_string(iter->value)->string;
            }
            else if (!strcmp("overrideCountryCode ", iter->name->string))
            {
                eos_config.overrideCountryCode = json_value_as_string(iter->value)->string;
            }
            else if (!strcmp("overrideLocaleCode", iter->name->string))
            {
                eos_config.overrideLocaleCode = json_value_as_string(iter->value)->string;
            }
            else if (!strcmp("platformOptionsFlags", iter->name->string))
            {
                eos_config.flags = static_cast<uint64_t>(json_helpers::collect_flags(&PLATFORM_CREATION_FLAGS_STRINGS_TO_ENUM, 0, iter));
            }
            else if (!strcmp("tickBudgetInMilliseconds", iter->name->string))
            {
                eos_config.tickBudgetInMilliseconds = json_helpers::json_value_as_uint32(iter->value);
            }
            else if (!strcmp("taskNetworkTimeoutSeconds", iter->name->string))
            {
                eos_config.taskNetworkTimeoutSeconds = json_helpers::json_value_as_double(iter->value);
            }
            else if (!strcmp("ThreadAffinity_networkWork", iter->name->string))
            {
                eos_config.ThreadAffinity_networkWork = json_helpers::json_value_as_uint64(iter->value);
            }
            else if (!strcmp("ThreadAffinity_storageIO", iter->name->string))
            {
                eos_config.ThreadAffinity_storageIO = json_helpers::json_value_as_uint64(iter->value);
            }
            else if (!strcmp("ThreadAffinity_webSocketIO", iter->name->string))
            {
                eos_config.ThreadAffinity_webSocketIO = json_helpers::json_value_as_uint64(iter->value);
            }
            else if (!strcmp("ThreadAffinity_P2PIO", iter->name->string))
            {
                eos_config.ThreadAffinity_P2PIO = json_helpers::json_value_as_uint64(iter->value);
            }
            else if (!strcmp("ThreadAffinity_HTTPRequestIO", iter->name->string))
            {
                eos_config.ThreadAffinity_HTTPRequestIO = json_helpers::json_value_as_uint64(iter->value);
            }
            else if (!strcmp("ThreadAffinity_RTCIO", iter->name->string))
            {
                eos_config.ThreadAffinity_RTCIO = json_helpers::json_value_as_uint64(iter->value);
            }
            else if (!strcmp("isServer", iter->name->string))
            {
                // In this JSON library, true and false are _technically_ different types. 
                if (json_value_is_true(iter->value))
                {
                    eos_config.isServer = true;
                }
                else if (json_value_is_false(iter->value))
                {
                    eos_config.isServer = false;
                }
            }

            iter = iter->next;
        }

        return eos_config;
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
                eos_config.steamSDKMajorVersion = json_helpers::json_value_as_uint32(iter->value);
            }
            else if (!strcmp("steamSDKMinorVersion", iter->name->string))
            {
                eos_config.steamSDKMinorVersion = json_helpers::json_value_as_uint32(iter->value);
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

    json_value_s* read_eos_config_as_json_value_from_file(std::string config_filename)
    {
        std::filesystem::path path_to_config_json = get_path_for_eos_service_config(config_filename);

        return json_helpers::read_config_json_as_json_from_path(path_to_config_json);
    }
}
