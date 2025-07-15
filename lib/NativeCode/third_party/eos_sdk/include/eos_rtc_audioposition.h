// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_rtc_audio_types.h"
#include "eos_rtc_audioposition_types.h"

/**
 * Use this function to change user's position.
 *
 * @param Options structure containing the parameters for the operation.
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidState if the room doesn't support positional audio
 *         EOS_NotFound if either the local user or specified participant are not in the room
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudio_SetPosition(EOS_HRTCAudio Handle, const EOS_RTCAudio_SetPositionOptions* Options);
