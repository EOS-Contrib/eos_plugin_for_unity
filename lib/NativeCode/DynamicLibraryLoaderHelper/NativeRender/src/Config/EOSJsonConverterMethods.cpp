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

namespace nlohmann
{
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

    
    void from_json(const json& json, EOS_EAuthScopeFlags& auth_scope_flags)
    {
        flags_enum_from_json<EOS_EAuthScopeFlags>(
            json, 
            STRINGS_TO_AUTH_SCOPE_FLAGS, 
            auth_scope_flags);
    }

    
    void from_json(const json& json, EOS_EIntegratedPlatformManagementFlags& integrated_platform_management_flags)
    {
        flags_enum_from_json<EOS_EIntegratedPlatformManagementFlags>(
            json, 
            INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_STRING_TO_ENUM,
            integrated_platform_management_flags);
    }

    
    void from_json(const json& json, EOS_UI_EInputStateButtonFlags& input_state_button_flags)
    {
        flags_enum_from_json<EOS_UI_EInputStateButtonFlags>(
            json,
            STRINGS_TO_INPUT_STATE_BUTTON_FLAGS,
            input_state_button_flags
        );
    }

    

    
}
