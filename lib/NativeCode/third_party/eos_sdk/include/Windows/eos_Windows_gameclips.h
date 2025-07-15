// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_Windows_base.h"
#include "eos_types.h"

#pragma pack(push, 8)

/**
 * Supported rendering API's for video frame recording
 */
EOS_ENUM(EOS_EWindowsVideoFrameAPI,
	/** Windows Direct3D 11 */
	EOS_WVF_Direct3D11 = 0,
	/** Windows Direct3D 12 */
	EOS_WVF_Direct3D12 = 1,
);

#define EOS_GAMECLIPS_WINDOWS_VIDEOFRAME_API_LATEST 1

EOS_STRUCT(EOS_GameClips_Windows_VideoFrame, (
	/** API Version: Set this to EOS_GAMECLIPS_SWITCH_VIDEOFRAME_API_LATEST. */
	int32_t ApiVersion;
	/** Frame format/rendering api */
	EOS_EWindowsVideoFrameAPI RenderAPI;
	/** pointer to device used to create texture (matching the RenderAPI setting) */
	void* Device;
	/** pointer to texture to be encoded into video (matching the RenderAPI setting) */ 
	void* Texture;
));

#pragma pack(pop)
