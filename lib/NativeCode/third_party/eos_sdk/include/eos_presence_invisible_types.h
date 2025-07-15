// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_platform_prereqs.h"
#include "eos_common.h"
#include "eos_presence_types.h"

#pragma pack(push, 8)

#define EOS_PRESENCE_GETINVISIBLE_API_LATEST 1

/**
 * Data for the EOS_Presence_GetInvisible function.
 */
EOS_STRUCT(EOS_Presence_GetInvisibleOptions, (
	/** API Version: Set this to EOS_PRESENCE_GETINVISIBLE_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the local, logged-in user making the request */
	EOS_EpicAccountId LocalUserId;
));

#pragma pack(pop)
