#pragma once
#include "json.h"

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
}
