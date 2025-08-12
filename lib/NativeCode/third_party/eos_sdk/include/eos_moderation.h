// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_moderation_types.h"

/**
 * The component contains a set of APIs to support voice and text chat moderation.
 *
 * @see EOS_Moderation_GenerateKeys
 */

/**
 * Generates and registers a pair of crypto keys for specified user (required for moderation features).
 * The completion delegate should be fired when the registering operation completes, either successfully or in error.
 *
 * @param Options The structure with the necessary parameters
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate
 * @param CompletionDelegate A callback that is fired when the registering operation completes, either successfully or in error
 */
EOS_DECLARE_FUNC(void) EOS_Moderation_GenerateKeys(EOS_HModeration Handle, const EOS_Moderation_GenerateKeysOptions* Options, void* ClientData, const EOS_Moderation_OnGenerateKeysCallback CompletionDelegate);
