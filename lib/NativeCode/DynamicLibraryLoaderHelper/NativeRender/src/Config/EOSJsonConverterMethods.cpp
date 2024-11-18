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
#include "EOSJsonConverterMethods.h"
#include "string_helpers.h"
#include <iostream>

namespace nlohmann
{
    using namespace pew::eos::string_helpers;
    /*
    // Helper macro to strip a prefix (e.g., EOS_AS_)
    #define STRIP_PREFIX(Value, Prefix) #Value + sizeof(#Prefix) - 1

    // Macro to simplify individual key-value pairs
    #define FLAG_PAIR(EnumValue, Prefix) { EnumValue, STRIP_PREFIX(EnumValue, Prefix) }

    // Macro to define the flag-to-string map
    #define DEFINE_FLAG_STRING_MAP(EnumType, MapName, Prefix, ...) \
    const std::map<EnumType, std::string> MapName = { __VA_ARGS__ }

    // AUTH_SCOPE_FLAGS_TO_STRINGS
    DEFINE_FLAG_STRING_MAP(
        EOS_EAuthScopeFlags,
        AUTH_SCOPE_FLAGS_TO_STRINGS,
        EOS_AS_,
        FLAG_PAIR(EOS_EAuthScopeFlags::EOS_AS_NoFlags, EOS_AS_),
        FLAG_PAIR(EOS_EAuthScopeFlags::EOS_AS_BasicProfile, EOS_AS_),
        FLAG_PAIR(EOS_EAuthScopeFlags::EOS_AS_FriendsList, EOS_AS_),
        FLAG_PAIR(EOS_EAuthScopeFlags::EOS_AS_Presence, EOS_AS_),
        FLAG_PAIR(EOS_EAuthScopeFlags::EOS_AS_FriendsManagement, EOS_AS_),
        FLAG_PAIR(EOS_EAuthScopeFlags::EOS_AS_Email, EOS_AS_),
        FLAG_PAIR(EOS_EAuthScopeFlags::EOS_AS_Country, EOS_AS_)
    );

    // INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_TO_STRINGS
    DEFINE_FLAG_STRING_MAP(
        EOS_EIntegratedPlatformManagementFlags,
        INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_TO_STRINGS,
        EOS_IPMF_,
        FLAG_PAIR(EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_Disabled, EOS_IPMF_),
        FLAG_PAIR(EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedByApplication, EOS_IPMF_),
        FLAG_PAIR(EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedBySDK, EOS_IPMF_),
        FLAG_PAIR(EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisablePresenceMirroring, EOS_IPMF_),
        FLAG_PAIR(EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisableSDKManagedSessions, EOS_IPMF_),
        FLAG_PAIR(EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferEOSIdentity, EOS_IPMF_),
        FLAG_PAIR(EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferIntegratedIdentity, EOS_IPMF_),
        FLAG_PAIR(EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_ApplicationManagedIdentityLogin, EOS_IPMF_)
    );

    // INPUT_STATE_BUTTON_FLAGS_TO_STRINGS
    DEFINE_FLAG_STRING_MAP(
        EOS_UI_EInputStateButtonFlags,
        INPUT_STATE_BUTTON_FLAGS_TO_STRINGS,
        EOS_UISBF_,
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_None, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Left, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Right, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Down, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Up, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Left, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Right, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Bottom, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Top, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_LeftShoulder, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_RightShoulder, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_LeftTrigger, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_RightTrigger, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_Special_Left, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_Special_Right, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_LeftThumbstick, EOS_UISBF_),
        FLAG_PAIR(EOS_UI_EInputStateButtonFlags::EOS_UISBF_RightThumbstick, EOS_UISBF_)
    );
    */
    const std::map<EOS_EAuthScopeFlags, std::string> AUTH_SCOPE_FLAGS_TO_STRINGS = {
        { EOS_EAuthScopeFlags::EOS_AS_NoFlags,           "NoFlags"           },
        { EOS_EAuthScopeFlags::EOS_AS_BasicProfile,      "BasicProfile"      },
        { EOS_EAuthScopeFlags::EOS_AS_FriendsList,       "FriendsList"       },
        { EOS_EAuthScopeFlags::EOS_AS_Presence,          "Presence"          },
        { EOS_EAuthScopeFlags::EOS_AS_FriendsManagement, "FriendsManagement" },
        { EOS_EAuthScopeFlags::EOS_AS_Email,             "Email"             },
        { EOS_EAuthScopeFlags::EOS_AS_Country,           "Country"           },
    };

    const std::map<EOS_EIntegratedPlatformManagementFlags, std::string> INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_TO_STRINGS = {
        { EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_Disabled,                        "Disabled"                       },
        { EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedByApplication,     "LibraryManagedByApplication"    },
        { EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_LibraryManagedBySDK,             "LibraryManagedBySDK"            },
        { EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisablePresenceMirroring,        "DisablePresenceMirroring"       },
        { EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_DisableSDKManagedSessions,       "DisableSDKManagedSessions"      },
        { EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferEOSIdentity,               "PreferEOSIdentity"              },
        { EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_PreferIntegratedIdentity,        "PreferIntegratedIdentity"       },
        { EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_ApplicationManagedIdentityLogin, "ApplicationManagedIdentityLogin"},
    };

    const std::map< EOS_UI_EInputStateButtonFlags, std::string> INPUT_STATE_BUTTON_FLAGS_TO_STRINGS = {
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_None,              "None"              },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Left,         "DPad_Left"         },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Right,        "DPad_Right"        },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Down,         "DPad_Down"         },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_DPad_Up,           "DPad_Up"           },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Left,   "FaceButton_Left"   },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Right,  "FaceButton_Right"  },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Bottom, "FaceButton_Bottom" },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_FaceButton_Top,    "FaceButton_Top"    },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_LeftShoulder,      "LeftShoulder"      },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_RightShoulder,     "RightShoulder"     },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_LeftTrigger,       "LeftTrigger"       },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_RightTrigger,      "RightTrigger"      },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_Special_Left,      "Special_Left"      },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_Special_Right,     "Special_Right"     },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_LeftThumbstick,    "LeftThumbstick"    },
        { EOS_UI_EInputStateButtonFlags::EOS_UISBF_RightThumbstick,   "RightThumbstick"   },
    };
    

    void from_json(const json& json, EOS_EAuthScopeFlags& auth_scope_flags)
    {
        flags_enum_from_json<EOS_EAuthScopeFlags>(
            json, 
            AUTH_SCOPE_FLAGS_TO_STRINGS, 
            auth_scope_flags);
    }

    void from_json(const json& json, EOS_EIntegratedPlatformManagementFlags& integrated_platform_management_flags)
    {
        flags_enum_from_json<EOS_EIntegratedPlatformManagementFlags>(
            json, 
            INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_TO_STRINGS, 
            integrated_platform_management_flags);
    }
    
    void from_json(const json& json, EOS_UI_EInputStateButtonFlags& input_state_button_flags)
    {
        flags_enum_from_json<EOS_UI_EInputStateButtonFlags>(
            json,
            INPUT_STATE_BUTTON_FLAGS_TO_STRINGS,
            input_state_button_flags
        );
    }

    unsigned __int64 get_int64_value(const json& json, const char* key)
    {
        int temp;
        json[key].get_to(temp);
        return static_cast<unsigned __int64>(temp);
    }

    #define JSON_PARSE_THREAD_AFFINITY(json, key, affinity) affinity.key = get_int64_value(json, #key)

    void from_json(const json& json, EOS_Initialize_ThreadAffinity& initialize_thread_affinity)
    {
        JSON_PARSE_THREAD_AFFINITY(json, NetworkWork,                  initialize_thread_affinity);
        JSON_PARSE_THREAD_AFFINITY(json, StorageIo,                    initialize_thread_affinity);
        JSON_PARSE_THREAD_AFFINITY(json, WebSocketIo,                  initialize_thread_affinity);
        JSON_PARSE_THREAD_AFFINITY(json, P2PIo,                        initialize_thread_affinity);
        JSON_PARSE_THREAD_AFFINITY(json, HttpRequestIo,                initialize_thread_affinity);
        JSON_PARSE_THREAD_AFFINITY(json, RTCIo,                        initialize_thread_affinity);
        JSON_PARSE_THREAD_AFFINITY(json, EmbeddedOverlayMainThread,    initialize_thread_affinity);
        JSON_PARSE_THREAD_AFFINITY(json, EmbeddedOverlayWorkerThreads, initialize_thread_affinity);
    }
}
