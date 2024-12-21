#ifndef ENUM_MAPPINGS_H
#define ENUM_MAPPINGS_H

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

namespace pew::eos
{
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
}

#endif
