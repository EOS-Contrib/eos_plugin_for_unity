// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"

/** The most recent version of the EOS_Moderation_GenerateKeysOptions API. */
#define EOS_MODERATION_GENERATEKEYS_API_LATEST 1

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_ModerationHandle* EOS_HModeration;

/**
 * The options that should be passed to EOS_Moderation_GenerateKeys function.
 */
EOS_STRUCT(EOS_Moderation_GenerateKeysOptions, (
	/** API version - set to EOS_MODERATION_GENERATEKEYS_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the local user. */
	EOS_ProductUserId LocalUserId;
));

/**
 * Async result of the EOS_Moderation_GenerateKeys function.
 */
EOS_STRUCT(EOS_Moderation_GenerateKeysCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Moderation_GenerateKeys. */
	void* ClientData;
	/** The Product User ID of the local user. */
	EOS_ProductUserId LocalUserId;
	/** The private key data. */
	uint8_t* PrivateKeyData;
	/** The length of private key data. */
	uint32_t PrivateKeyDataLengthBytes;
	/** The null-terminated string with signed public key (JWT). */
	const char* SignedPublicKey;
));

EOS_DECLARE_CALLBACK(EOS_Moderation_OnGenerateKeysCallback, const EOS_Moderation_GenerateKeysCallbackInfo* Data);

#pragma pack(pop)
