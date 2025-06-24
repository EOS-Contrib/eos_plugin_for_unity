// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#pragma pack(push, 8)

/** The most recent version of the EOS_Android_IAP_VerifyPurchasePlatformOptions structure */
#define EOS_ANDROID_IAP_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST 1

/**
 * Platform specific options for EOS_ReceiptValidator_VerifyPurchase for the Google VendorType using the in-app purchase EntitlementType.
 *
 * @see EOS_ReceiptValidator_VerifyPurchaseOptions
 */
EOS_STRUCT(EOS_Android_IAP_VerifyPurchasePlatformOptions, (
	/** API Version: Set this to EOS_ANDROID_IAP_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** Product ID */
	const char* ProductId;
	/** Token */
	const char* Token;
));

#pragma pack(pop)
