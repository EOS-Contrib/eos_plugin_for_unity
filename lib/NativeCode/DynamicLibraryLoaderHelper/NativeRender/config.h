#pragma once
#include "io_helpers.h"
#include "json.h"
#include "json_helpers.h"
#include "logging.h"
#include "temp.h"


namespace playeveryware::eos::config
{
    /**
     * \brief
     * Maps string values to values defined by the EOS SDK regarding platform
     * creation.
     */
    static const std::map<std::string, int> PLATFORM_CREATION_FLAGS_STRINGS_TO_ENUM = {
        {"EOS_PF_LOADING_IN_EDITOR",                          EOS_PF_LOADING_IN_EDITOR},
        {"LoadingInEditor",                                   EOS_PF_LOADING_IN_EDITOR},

        {"EOS_PF_DISABLE_OVERLAY",                            EOS_PF_DISABLE_OVERLAY},
        {"DisableOverlay",                                    EOS_PF_DISABLE_OVERLAY},

        {"EOS_PF_DISABLE_SOCIAL_OVERLAY",                     EOS_PF_DISABLE_SOCIAL_OVERLAY},
        {"DisableSocialOverlay",                              EOS_PF_DISABLE_SOCIAL_OVERLAY},

        {"EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D9",                EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D9},
        {"WindowsEnableOverlayD3D9",                          EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D9},

        {"EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D10",               EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D10},
        {"WindowsEnableOverlayD3D10",                         EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D10},

        {"EOS_PF_WINDOWS_ENABLE_OVERLAY_OPENGL",              EOS_PF_WINDOWS_ENABLE_OVERLAY_OPENGL},
        {"WindowsEnableOverlayOpengl",                        EOS_PF_WINDOWS_ENABLE_OVERLAY_OPENGL},

        {"EOS_PF_CONSOLE_ENABLE_OVERLAY_AUTOMATIC_UNLOADING", EOS_PF_CONSOLE_ENABLE_OVERLAY_AUTOMATIC_UNLOADING},
        {"ConsoleEnableOverlayAutomaticUnloading",            EOS_PF_CONSOLE_ENABLE_OVERLAY_AUTOMATIC_UNLOADING},

        {"EOS_PF_RESERVED1",                                  EOS_PF_RESERVED1},
        {"Reserved1",                                         EOS_PF_RESERVED1}
    };

    /**
     * \brief Maps string values to values within the
     * EOS_EIntegratedPlatformManagementFlags enum.
     */
    static const std::map<std::string, EOS_EIntegratedPlatformManagementFlags> INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_STRINGS_TO_ENUM = {
        {"EOS_IPMF_Disabled",                        EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_Disabled },
        {"Disabled",                                 EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_Disabled },

        {"EOS_IPMF_LibraryManagedByApplication",     EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedByApplication },
        {"EOS_IPMF_ManagedByApplication",            EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedByApplication},
        {"ManagedByApplication",                     EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedByApplication},
        {"LibraryManagedByApplication",              EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedByApplication},

        {"ManagedBySDK",                             EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedBySDK },
        {"EOS_IPMF_ManagedBySDK",                    EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedBySDK },
        {"EOS_IPMF_LibraryManagedBySDK",             EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedBySDK },
        {"LibraryManagedBySDK",                      EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedBySDK },

        {"DisableSharedPresence",                    EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisablePresenceMirroring },
        {"EOS_IPMF_DisableSharedPresence",           EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisablePresenceMirroring },
        {"EOS_IPMF_DisablePresenceMirroring",        EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisablePresenceMirroring },
        {"DisablePresenceMirroring",                 EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisablePresenceMirroring},

        {"DisableSessions",                          EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisableSDKManagedSessions },
        {"EOS_IPMF_DisableSessions",                 EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisableSDKManagedSessions },
        {"EOS_IPMF_DisableSDKManagedSessions",       EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisableSDKManagedSessions },
        {"DisableSDKManagedSessions",                EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisableSDKManagedSessions },

        {"PreferEOS",                                EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferEOSIdentity },
        {"EOS_IPMF_PreferEOS",                       EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferEOSIdentity },
        {"EOS_IPMF_PreferEOSIdentity",               EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferEOSIdentity },
        {"PreferEOSIdentity",                        EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferEOSIdentity},

        {"PreferIntegrated",                         EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferIntegratedIdentity },
        {"EOS_IPMF_PreferIntegrated",                EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferIntegratedIdentity },
        {"EOS_IPMF_PreferIntegratedIdentity",        EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferIntegratedIdentity },
        {"PreferIntegratedIdentity",                 EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferIntegratedIdentity},

        {"EOS_IPMF_ApplicationManagedIdentityLogin", EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_ApplicationManagedIdentityLogin },
        {"ApplicationManagedIdentityLogin",          EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_ApplicationManagedIdentityLogin}
    };

    typedef const char* (*GetConfigAsJSONString_t)();
    static GetConfigAsJSONString_t GetConfigAsJSONString;

    struct SandboxDeploymentOverride
    {
        std::string sandboxID;
        std::string deploymentID;
    };

    struct EOSConfig
    {
        std::string productName;
        std::string productVersion;

        std::string productID;
        std::string sandboxID;
        std::string deploymentID;
        std::vector<SandboxDeploymentOverride> sandboxDeploymentOverrides;

        std::string clientSecret;
        std::string clientID;
        std::string encryptionKey;

        std::string overrideCountryCode;
        std::string overrideLocaleCode;

        // this is called platformOptionsFlags in C#
        uint64_t flags = 0;

        uint32_t tickBudgetInMilliseconds = 0;
        double taskNetworkTimeoutSeconds = 0.0;

        uint64_t ThreadAffinity_networkWork = 0;
        uint64_t ThreadAffinity_storageIO = 0;
        uint64_t ThreadAffinity_webSocketIO = 0;
        uint64_t ThreadAffinity_P2PIO = 0;
        uint64_t ThreadAffinity_HTTPRequestIO = 0;
        uint64_t ThreadAffinity_RTCIO = 0;

        bool isServer = false;

    };

    struct LogLevelConfig
    {
        std::vector<std::string> category;
        std::vector<std::string> level;
    };

    struct EOSSteamConfig
    {
        EOS_EIntegratedPlatformManagementFlags flags;
        uint32_t steamSDKMajorVersion;
        uint32_t steamSDKMinorVersion;
        std::optional<std::string> OverrideLibraryPath;
        std::vector<std::string> steamApiInterfaceVersionsArray;

        EOSSteamConfig()
        {
            flags = static_cast<EOS_EIntegratedPlatformManagementFlags>(0);
        }

        bool isManagedByApplication()
        {
            return std::underlying_type<EOS_EIntegratedPlatformManagementFlags>::type(flags & EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedByApplication);
        }
        bool isManagedBySDK()
        {
            return std::underlying_type<EOS_EIntegratedPlatformManagementFlags>::type(flags & EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedBySDK);
        }

    };

    inline static LogLevelConfig log_config_from_json_value(json_value_s* config_json)
    {
        struct json_object_s* config_json_object = json_value_as_object(config_json);
        struct json_object_element_s* iter = config_json_object->start;
        LogLevelConfig log_config;

        while (iter != nullptr)
        {
            if (!strcmp("LogCategoryLevelPairs", iter->name->string))
            {
                json_array_s* pairs = json_value_as_array(iter->value);
                for (auto e = pairs->start; e != nullptr; e = e->next)
                {
                    struct json_object_s* pairs_json_object = json_value_as_object(e->value);
                    struct json_object_element_s* pairs_iter = pairs_json_object->start;
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

    //-------------------------------------------------------------------------
    static std::filesystem::path get_path_for_eos_service_config(std::string config_filename)
    {
        //return get_path_relative_to_current_module(std::filesystem::path("../..") / "StreamingAssets" / "EOS" / "EpicOnlineServicesConfig.json");
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

    //-------------------------------------------------------------------------
    inline static json_value_s* read_config_json_from_dll()
    {
        struct json_value_s* config_json = nullptr;

#if ENABLE_DLL_BASED_EOS_CONFIG
        logging::log_inform("Trying to load eos config via dll");
        static void* eos_generated_library_handle = temp::load_library_at_path(io_helpers::get_path_relative_to_current_module("EOSGenerated.dll"));

        if (!eos_generated_library_handle)
        {
            logging::log_warn("No Generated DLL found (Might not be an error)");
            return NULL;
        }

        GetConfigAsJSONString = temp::load_function_with_name<GetConfigAsJSONString_t>(eos_generated_library_handle, "GetConfigAsJSONString");

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
    inline static EOSConfig eos_config_from_json_value(json_value_s* config_json)
    {
        // Create platform instance
        struct json_object_s* config_json_object = json_value_as_object(config_json);
        struct json_object_element_s* iter = config_json_object->start;
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
                    struct json_object_s* override_json_object = json_value_as_object(e->value);
                    struct json_object_element_s* ov_iter = override_json_object->start;
                    struct SandboxDeploymentOverride override_item = SandboxDeploymentOverride();
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


    //-------------------------------------------------------------------------
    static EOS_EIntegratedPlatformManagementFlags eos_collect_integrated_platform_managment_flags(json_object_element_s* iter)
    {

        return json_helpers::collect_flags<EOS_EIntegratedPlatformManagementFlags>(
            &(INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_STRINGS_TO_ENUM),
            EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_Disabled,
            iter);
    }

}
