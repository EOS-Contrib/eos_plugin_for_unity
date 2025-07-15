// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

/**
 * When a room with requested proximity is connected, and proximity is enabled, the following options will be passed in
 * the EOS_RTC_JoinRoomCallbackInfo structure via RoomOptions:
 * - ProximityRoom: Flag to notify the application that this room is a proximity room so active audio streams will be selected
 *	based on proximity to local participants first. With 'large room support' enabled, proximity is applied first
 *	so that most active streams are selected from the streams that are inside hearing radius.
 * - ProximityHearingRadius: The hearing radius around the local participant that is defined for the room. Type - float.
 * 
 * @see EOS_RTC_JoinRoomCallbackInfo
 */

/** The most recent version of the EOS_RTCAudio_SetPosition API. */
#define EOS_RTCAUDIO_SETPOSITION_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudio_SetPosition
 */
EOS_STRUCT(EOS_RTCAudio_SetPositionOptions, (
	/** API Version: Set this to EOS_RTCAUDIO_SETPOSITION_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** A new X position that should be applied the user. */
	float X;
	/** A new Y position that should be applied the user. */
	float Y;
	/** A new Z position that should be applied the user. */
	float Z;
));

#pragma pack(pop)
