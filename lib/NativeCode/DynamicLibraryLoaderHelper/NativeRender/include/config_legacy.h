#ifndef CONFIG_H
#define CONFIG_H

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

#include <filesystem>
#include <map>
#include <vector>
#include <optional>

#include "json.h"
#include "eos_sdk.h"

struct json_value_s;

#define CONFIG_EXPORTS

#ifdef CONFIG_EXPORTS
#define CONFIG_API __declspec(dllexport)
#else
#define CONFIG_API __declspec(dllimport)
#endif

namespace pew::eos::config_legacy
{
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

    /**
     * \brief Maps string values to values for platform options flags. Note that
     * there are multiple keys that can be mapped to the same value. This is to
     * provide backwards-compatibility for versions where the member names of
     * the enum have changed or were being serialized differently.
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
        {"Reserved1",                                         EOS_PF_RESERVED1},

        {"EOS_PF_NONE",                                       0},
        {"None",                                              0},
    };

    /**
     * \brief Maps string values to specific auth scope flags defined within the
     * EOS SDK.
     */
    const std::map<std::string, EOS_EAuthScopeFlags> AUTH_SCOPE_FLAGS_STRINGS_TO_ENUM = {
        { "NoFlags",           EOS_EAuthScopeFlags::EOS_AS_NoFlags           },
        { "BasicProfile",      EOS_EAuthScopeFlags::EOS_AS_BasicProfile      },
        { "FriendsList",       EOS_EAuthScopeFlags::EOS_AS_FriendsList       },
        { "Presence",          EOS_EAuthScopeFlags::EOS_AS_Presence          },
        { "FriendsManagement", EOS_EAuthScopeFlags::EOS_AS_FriendsManagement },
        { "Email",             EOS_EAuthScopeFlags::EOS_AS_Email             },
        { "Country",           EOS_EAuthScopeFlags::EOS_AS_Country           },
    };

    /**
    * \brief Maps string values to values within the EOS_UI_EInputStateButtonFlags enum. Note that there are multiple
    * keys that can be mapped to the same value. This is to provide backwards-compatibility for versions where the member names of the
    * enum have changed or were being serialized differently.
    */
    const std::map<std::string, EOS_UI_EInputStateButtonFlags> INPUT_STATE_BUTTON_FLAGS_STRINGS_TO_ENUM = {
        { "None",              EOS_UI_EInputStateButtonFlags::EOS_UISBF_None              },

        { "DPad_Left",         EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Left         },
        { "DPadLeft",          EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Left         },

        { "DPad_Right",        EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Right        },
        { "DPadRight",         EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Right        },

        { "DPad_Down",         EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Down         },
        { "DPadDown",          EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Down         },

        { "DPad_Up",           EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Up           },
        { "DPadUp",            EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Up           },

        { "FaceButton_Left",   EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Left   },
        { "FaceButtonLeft",    EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Left   },

        { "FaceButton_Right",  EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Right  },
        { "FaceButtonRight",   EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Right  },

        { "FaceButton_Bottom", EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Bottom },
        { "FaceButtonBottom",  EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Bottom },

        { "FaceButton_Top",    EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Top    },
        { "FaceButtonTop",     EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Top    },

        { "LeftShoulder",      EOS_UI_EInputStateButtonFlags::EOS_UISBF_LeftShoulder      },

        { "RightShoulder",     EOS_UI_EInputStateButtonFlags::EOS_UISBF_RightShoulder     },

        { "LeftTrigger",       EOS_UI_EInputStateButtonFlags::EOS_UISBF_LeftTrigger       },

        { "RightTrigger",      EOS_UI_EInputStateButtonFlags::EOS_UISBF_RightTrigger      },

        { "Special_Left",      EOS_UI_EInputStateButtonFlags::EOS_UISBF_Special_Left      },
        { "SpecialLeft",       EOS_UI_EInputStateButtonFlags::EOS_UISBF_Special_Left      },

        { "Special_Right",     EOS_UI_EInputStateButtonFlags::EOS_UISBF_Special_Right     },
        { "SpecialRight",      EOS_UI_EInputStateButtonFlags::EOS_UISBF_Special_Right     },

        { "LeftThumbstick",    EOS_UI_EInputStateButtonFlags::EOS_UISBF_LeftThumbstick    },
        { "RightThumbstick",   EOS_UI_EInputStateButtonFlags::EOS_UISBF_RightThumbstick   },
    };

    /**
     * @brief Typedef for a function pointer that retrieves the configuration as a JSON string.
     *
     * This function pointer type represents a function that, when called,
     * returns the configuration data as a JSON-formatted string.
     */
    typedef const char* (*GetConfigAsJSONString_t)();

    /**
     * @brief Represents a sandbox deployment override configuration.
     *
     * Contains the sandbox ID and the corresponding deployment ID to override the default sandbox
     * deployment configuration.
     */
    struct SandboxDeploymentOverride
    {
        std::string sandboxID;
        std::string deploymentID;
    };


    /**
     * @brief Holds EOS platform configuration settings.
     *
     * This structure defines various configuration parameters for the EOS platform, including
     * product details, sandbox information, client credentials, thread affinities, and platform
     * options.
     */
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

    /**
     * @brief Holds configuration for log levels and categories.
     *
     * This structure defines log level settings for specific log categories.
     */
    struct LogLevelConfig
    {
        std::vector<std::string> category;
        std::vector<std::string> level;
    };

    /**
     * @brief Configuration settings specific to the EOS Steam platform integration.
     *
     * This structure defines settings for integrating with the Steam platform, including SDK versions,
     * platform flags, and API versions.
     */
    struct EOSSteamConfig
    {
        EOS_EIntegratedPlatformManagementFlags flags = static_cast<EOS_EIntegratedPlatformManagementFlags>(0);
        uint32_t steamSDKMajorVersion = 0;
        uint32_t steamSDKMinorVersion = 0;
        std::optional<std::string> OverrideLibraryPath;
        std::vector<std::string> steamApiInterfaceVersionsArray;

        /**
         * @brief Checks if the library is managed by the application.
         *
         * @return `true` if the library is managed by the application, `false` otherwise.
         */
        bool is_managed_by_application() const;

        /**
         * @brief Checks if the library is managed by the EOS SDK.
         *
         * @return `true` if the library is managed by the SDK, `false` otherwise.
         */
        bool is_managed_by_sdk() const;
    };

    /**
     * @brief Parses a JSON configuration object to create a log level configuration.
     *
     * Extracts log category and level pairs from a JSON structure, storing them in a `LogLevelConfig` object.
     *
     * @param config_json The JSON value representing the configuration.
     * @return A `LogLevelConfig` object populated with log categories and their levels.
     */
    LogLevelConfig log_config_from_json_value(json_value_s* config_json);

    /**
     * @brief Retrieves the path to the EOS service configuration file.
     *
     * Attempts to locate the configuration file by navigating up directories.
     * If the path is not found, it defaults to a predefined directory.
     *
     * @param config_filename The name of the configuration file.
     * @return The path to the EOS service configuration file.
     */
    std::filesystem::path get_path_for_eos_service_config(const std::string& config_filename);

    /**
     * @brief Reads a JSON configuration from a DLL.
     *
     * TODO: This is no longer supported, it should be removed.
     *
     * Attempts to load configuration data from a DLL. The function retrieves the
     * JSON configuration string from the DLL, parses it, and returns the parsed JSON object.
     *
     * @return A pointer to a `json_value_s` representing the configuration data, or `nullptr` if loading fails.
     */
    json_value_s* read_config_json_from_dll();

    /**
     * \brief Parses an EOS configuration object from a JSON structure.
     * \return An `EOSConfig` object populated with settings from the JSON structure.
     */
    //CONFIG_API bool try_get_eos_config(EOSConfig& config);

    /**
     * @brief Collects integrated platform management flags from a JSON element.
     *
     * Parses platform management flags from a JSON element and combines them into a single flag value.
     *
     * @param iter The JSON object element representing the flags.
     * @return The combined `EOS_EIntegratedPlatformManagementFlags` value.
     */
    EOS_EIntegratedPlatformManagementFlags eos_collect_integrated_platform_management_flags(json_object_element_s* iter);

    /**
     * @brief Parses an EOS Steam configuration from a JSON structure.
     *
     * Reads Steam-specific settings, such as flags, library path, SDK versions, and API versions,
     * from the provided JSON configuration, and stores them in an `EOSSteamConfig` object.
     *
     * @param config_json The JSON value representing the Steam configuration.
     * @return An `EOSSteamConfig` object populated with settings from the JSON structure.
     */
    EOSSteamConfig eos_steam_config_from_json_value(json_value_s* config_json);
}
#endif
