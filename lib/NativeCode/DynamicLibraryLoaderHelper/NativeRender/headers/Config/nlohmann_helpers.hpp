#ifndef NHLOHMANN_HELPERS_HPP
#define NHLOHMANN_HELPERS_HPP

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

#include <map>
#include <vector>

#include "headers/Config/common.hpp"
#include <nlohmann/json.hpp>

#include "headers/Config/Sandbox.hpp"
#include "headers/Config/Deployment.hpp"

/**
 * Because C++20 supports the "contains" method on the map collection, but older
 * versions of C++ do not, this MACRO is here to make it easier.
 */
#if __cplusplus >= 202002L  // Check if C++20 or later
#define CONTAINS(map, key) ((map).contains(key))
#else
#define CONTAINS(map, key) ((map).find(key) != (map).end())
#endif

 /**
  * \brief Function that instructs the nlohmann library how to parse an
  * EOS_Initialize_ThreadAffinity struct.
  *
  * Note that this is defined outside the pew::eos::config namespace. This is
  * because from_json functions must be defined within the namespace that the
  * object they parse to is defined, and since EOS_Initialize_ThreadAffinity is
  * defined in an anonymous namespace being as how it's included from,
  * eos_init.h.
  *
  * \param json The JSON object to read the values from.
  * \param initialize_thread_affinity The value to set from the JSON.
  */
inline void from_json(const nlohmann::json& json, EOS_Initialize_ThreadAffinity& initialize_thread_affinity)
{
    initialize_thread_affinity.ApiVersion = json["ApiVersion"].get<int>();
    initialize_thread_affinity.NetworkWork = json["NetworkWork"].get<uint64_t>();
    initialize_thread_affinity.StorageIo = json["StorageIo"].get<uint64_t>();
    initialize_thread_affinity.WebSocketIo = json["WebSocketIo"].get<uint64_t>();
    initialize_thread_affinity.P2PIo = json["P2PIo"].get<uint64_t>();
    initialize_thread_affinity.HttpRequestIo = json["HttpRequestIo"].get<uint64_t>();
    initialize_thread_affinity.RTCIo = json["RTCIo"].get<uint64_t>();
    initialize_thread_affinity.EmbeddedOverlayMainThread = json["EmbeddedOverlayMainThread"].get<uint64_t>();
    initialize_thread_affinity.EmbeddedOverlayWorkerThreads = json["EmbeddedOverlayWorkerThreads"].get<uint64_t>();
}

/**
     * \brief Helper constraint for allowing template functions to require the
     * template parameter be either an enum or an int.
     * \tparam T The type (either an enum class or an int).
     */
template <typename T>
constexpr bool is_enum_or_int_v = std::is_enum_v<T> || std::is_same_v<T, int>;

/**
 * \brief Converts a string value to an enum or integer value given a custom
 * mapping of a set of one to the other.
 * \tparam T Either an enum class or an integer.
 * \param flag_enums_string A string representation of flag enum values, in
 * the form of a comma-delimited list of values.
 * \param strings_to_flags A map that associates a string value with either
 * an enum or int value.
 * \param flags_enum The variable to assign the determined value to.
 */
template<typename T, std::enable_if_t<is_enum_or_int_v<T>, int> = 0>
void flags_enum_from_string(const std::string& flag_enums_string, const std::map<std::string, T>& strings_to_flags, T& flags_enum)
{
    // Get the comma-delimited list of strings
    const auto string_values = pew::eos::common::split_and_trim(flag_enums_string);

    // Iterate through them and apply to the auth scope flags.
    for (const auto& str : string_values)
    {
        // If the string isn't in the map of string values to flag values
        if (!CONTAINS(strings_to_flags, str))
        {
            continue;
        }

        // Perform bitwise-or operation to add flag value.
        flags_enum |= strings_to_flags.at(str);
    }
}

/**
 * \brief Converts the contents of a JSON object to an enum or integer value
 * given a custom mapping of a set of one to the other.
 * \tparam T Either an enum class or an integer.
 * \param json The JSON object to read the values from.
 * \param strings_to_flags A map that associates a string value with either
 * an enum or int value.
 * \param flags_enum The variable to assign the determined value to.
 */
template <typename T, std::enable_if_t<is_enum_or_int_v<T>, int> = 0>
void flags_enum_from_json(const nlohmann::json& json, const std::map<std::string, T>& strings_to_flags, T& flags_enum)
{
    std::string str_enum_values;
    json.get_to(str_enum_values);

    flags_enum_from_string(str_enum_values, strings_to_flags, flags_enum);
}

namespace pew::eos::config
{
    /**
     * \brief Maps string values to values for platform options flags. Note that
     * there are multiple keys that can be mapped to the same value. This is to
     * provide backwards-compatibility for versions where the member names of
     * the enum have changed or were being serialized differently.
     */
    static const std::map<std::string, int> PLATFORM_CREATION_FLAGS_STRING_TO_ENUM = {
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
     * \brief Maps string values to specific auth scope flags defined within the EOS SDK.
     */
    const std::map<std::string, EOS_EAuthScopeFlags> STRINGS_TO_AUTH_SCOPE_FLAGS = {
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
    const std::map<std::string, EOS_UI_EInputStateButtonFlags> STRINGS_TO_INPUT_STATE_BUTTON_FLAGS = {
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
    * \brief Maps string values to values within the EOS_EIntegratedPlatformManagementFlags enum. Note that there are multiple
    * keys that can be mapped to the same value. This is to provide backwards-compatibility for versions where the member names of the
    * enum have changed or were being serialized differently.
    */
    static const std::map<std::string, EOS_EIntegratedPlatformManagementFlags> INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_STRING_TO_ENUM = {
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
}


/**
 * \brief Function that instructs the nlohmann library how to parse auth
 * scope flags.
 * \param json The JSON object to read the values from.
 * \param auth_scope_flags The value to set from the JSON.
 */
inline void from_json(const nlohmann::json& json, EOS_EAuthScopeFlags& auth_scope_flags)
{
    flags_enum_from_json<EOS_EAuthScopeFlags>(
        json,
        pew::eos::config::STRINGS_TO_AUTH_SCOPE_FLAGS,
        auth_scope_flags);
}

/**
 * \brief Function that instructs the nlohmann library how to parse
 * integrated platform management flags.
 * \param json The JSON object to read the values from.
 * \param integrated_platform_management_flags The value to set from the
 * JSON.
 */
inline void from_json(const nlohmann::json& json, EOS_EIntegratedPlatformManagementFlags& integrated_platform_management_flags)
{
    flags_enum_from_json<EOS_EIntegratedPlatformManagementFlags>(
        json,
        pew::eos::config::INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_STRING_TO_ENUM,
        integrated_platform_management_flags);
}

/**
 * \brief Function that instructs the nlohmann library how to parse input
 * state button flags.
 * \param json The JSON object to read the values from.
 * \param input_state_button_flags The value to set from the JSON.
 */
inline void from_json(const nlohmann::json& json, EOS_UI_EInputStateButtonFlags& input_state_button_flags)
{
    flags_enum_from_json<EOS_UI_EInputStateButtonFlags>(
        json,
        pew::eos::config::STRINGS_TO_INPUT_STATE_BUTTON_FLAGS,
        input_state_button_flags
    );
}

namespace pew::eos::config
{
    inline void from_json(const nlohmann::json& json, ClientCredentials& credentials)
    {
        nlohmann::json temp_json = json;
        if (temp_json.contains("Value"))
        {
            temp_json = temp_json["Value"];
        }

        temp_json["ClientId"].get_to(credentials.client_id);
        temp_json["ClientSecret"].get_to(credentials.client_secret);
        temp_json["EncryptionKey"].get_to(credentials.encryption_key);
    }

    inline void from_json(const nlohmann::json& json, Deployment& deployment)
    {
        nlohmann::json temp_json = json;
        if (temp_json.contains("Value"))
        {
            temp_json = temp_json["Value"];
        }

        Sandbox sandbox;
        temp_json["SandboxId"]["Value"].get_to(sandbox.id);
        deployment.sandbox = sandbox;
        temp_json["DeploymentId"].get_to(deployment.id);
    }

    inline void from_json(const nlohmann::json& json, Sandbox& sandbox)
    {
        json["Value"]["Value"].get_to(sandbox.id);
    }

    inline void from_json(const nlohmann::json& json, ProductionEnvironments& environments)
    {
        environments.sandboxes = json["Sandboxes"].get_to(environments.sandboxes);
        environments.deployments = json["Deployments"].get <std::vector<Deployment>>();
    }

    inline void from_json(const nlohmann::json& json, PlatformConfig& platform_config)
    {
        json["deployment"].get_to(platform_config.deployment);
        json["clientCredentials"].get_to(platform_config.client_credentials);
        json["isServer"].get_to(platform_config.is_server);

        // "platformOptionsFlags" is special, because it's value is a uint64_t,
        // but the C# code translates it to a custom flag enum type and saves it
        // into the json as a comma-delimited list of values. Attempting to
        // parse that directly into an integer fails, so custom logic is
        // required. A "from_json" function cannot be written for this scenario
        // because the template parameter would be uint64_t, and parsing would
        // fail.
        std::string platform_options_flags_str;
        platform_config.platform_options_flags = 0;
        json["platformOptionsFlags"].get_to(platform_options_flags_str);
        flags_enum_from_string(platform_options_flags_str, PLATFORM_CREATION_FLAGS_STRING_TO_ENUM, platform_config.platform_options_flags);

        json["authScopeOptionsFlags"].get_to(platform_config.auth_scope_flags);
        json["integratedPlatformManagementFlags"].get_to(platform_config.integrated_platform_management_flags);
        json["tickBudgetInMilliseconds"].get_to(platform_config.tick_budget_in_milliseconds);
        json["taskNetworkTimeoutSeconds"].get_to(platform_config.task_network_timeout_seconds);
        json["threadAffinity"].get_to(platform_config.thread_affinity);
        json["alwaysSendInputToOverlay"].get_to(platform_config.always_send_input_to_overlay);
        json["alwaysSendInputToOverlay"].get_to(platform_config.always_send_input_to_overlay);
        json["initialButtonDelayForOverlay"].get_to(platform_config.initial_button_delay_for_overlay);
        json["repeatButtonDelayForOverlay"].get_to(platform_config.repeat_button_delay_for_overlay);
        json["toggleFriendsButtonCombination"].get_to(platform_config.toggle_friends_button_combination);
    }
}
#endif
