// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_types.h"

#pragma pack(push, 8)

/** The most recent version of the EOS_IOS_IAP_VerifyPurchasePlatformOptions structure */
#define EOS_IOS_IAP_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST 1

/**
 * Platform specific options for EOS_ReceiptValidator_VerifyPurchase for the Apple VendorType using the in-app purchase EntitlementType.
 *
 * @see EOS_ReceiptValidator_VerifyPurchaseOptions
 */
EOS_STRUCT(EOS_IOS_IAP_VerifyPurchasePlatformOptions, (
	/** API Version: Set this to EOS_IOS_IAP_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** Transaction ID */
	const char* TransactionId;
	/** Receipt */
	const char* Receipt;
));

#pragma pack(pop)
