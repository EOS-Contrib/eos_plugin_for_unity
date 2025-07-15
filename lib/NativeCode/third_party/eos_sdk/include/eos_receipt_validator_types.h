// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_ReceiptValidatorHandle* EOS_HReceiptValidator;

/**
 * A unique identifier for a receipt.
 */
EOS_EXTERN_C typedef const char* EOS_ReceiptValidator_ReceiptId;

/**
 * A unique identifier for a token.
 */
EOS_EXTERN_C typedef const char* EOS_ReceiptValidator_Token;

/**
 * A unique identifier for an entitlement owned by an account.
 */
EOS_EXTERN_C typedef const char* EOS_ReceiptValidator_EntitlementId;

/**
 * An enumeration of the different ownership statuses.
 */
EOS_ENUM(EOS_EReceiptValidatorVendorType,
	/** Vendor is Epic Games Store */
	EOS_RVVT_EGS = 0,
	/** Vendor is Steam */
	EOS_RVVT_STEAM = 1,
	/** Vendor is PlayStation(TM)Network */
	EOS_RVVT_PSN = 2,
	/** Vendor is Xbox Live */
	EOS_RVVT_XBL = 3,
	/** Vendor is Nintendo */
	EOS_RVVT_NINTENDO = 4,
	/** Vendor is Apple */
	EOS_RVVT_APPLE = 5,
	/** Vendor is Google */
	EOS_RVVT_GOOGLE = 6
);

/**
 * An enumeration defining an entitlement type.
 */
EOS_ENUM(EOS_EReceiptValidatorEntitlementType,
	/** This entitlement is an in-app purchase. */
	EOS_RVET_IN_APP_PURCHASE = 0,
	/** This entitlement is a downloadable content purchase. */
	EOS_RVET_DLC = 1
);

/** The most recent version of the EOS_ReceiptValidator_VerifyPurchase API */
#define EOS_RECEIPTVALIDATOR_VERIFYPURCHASE_API_LATEST 1

/**
 * Input parameters for the EOS_ReceiptValidator_VerifyPurchase function.
 */
EOS_STRUCT(EOS_ReceiptValidator_VerifyPurchaseOptions, (
	/** API Version: Set this to EOS_RECEIPTVALIDATOR_VERIFYPURCHASE_API_LATEST. */
	int32_t ApiVersion;
	/** Product User ID for local user who is verifying the purchase. */
	EOS_ProductUserId LocalUserId;
	/** Product User ID for the user whose purchase is being verified. */
	EOS_ProductUserId TargetUserId;
	/** Vendor Type. */
	EOS_EReceiptValidatorVendorType VendorType;
	/** Entitlement Type. */
	EOS_EReceiptValidatorEntitlementType EntitlementType;
	/**
	 * Platform specific data.
	 *
	 * For Console platforms, the structure will be located in <System>/eos_<System>.h.
	 * The structure will be named EOS_<System>_<EntitlementType>_VerifyPurchasePlatformOptions.
	 *
	 * @see EOS_EGS_IAP_VerifyPurchasePlatformOptions
	 * @see EOS_EGS_DLC_VerifyPurchasePlatformOptions
	 * @see EOS_Steam_IAP_VerifyPurchasePlatformOptions
	 * @see EOS_Steam_DLC_VerifyPurchasePlatformOptions
	 */
	void* PlatformSpecificOptions;
));

/**
 * Data containing the result information for a verify purchase request.
 */
EOS_STRUCT(EOS_ReceiptValidator_VerifyPurchaseCompleteCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_ReceiptValidator_VerifyPurchase. */
	void* ClientData;
));

/**
 * Function prototype definition for callbacks passed to EOS_ReceiptValidator_VerifyPurchase
 * @param Data An EOS_ReceiptValidator_VerifyPurchaseCompleteCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_ReceiptValidator_OnVerifyPurchaseCompleteCallback, const EOS_ReceiptValidator_VerifyPurchaseCompleteCallbackInfo* Data);

/** The most recent version of the EOS_Steam_IAP_VerifyPurchasePlatformOptions structure */
#define EOS_STEAM_IAP_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST 1

/**
 * Platform specific options for EOS_ReceiptValidator_VerifyPurchase for the Steam VendorType using the in-app purchase EntitlementType.
 *
 * @see EOS_ReceiptValidator_VerifyPurchaseOptions
 */
EOS_STRUCT(EOS_Steam_IAP_VerifyPurchasePlatformOptions, (
	/** API Version: Set this to EOS_STEAM_IAP_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** The Steam Transaction ID */
	const char* TransId;
));

/** The most recent version of the EOS_Steam_DLC_VerifyPurchasePlatformOptions structure */
#define EOS_STEAM_DLC_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST 1

/**
 * Platform specific options for EOS_ReceiptValidator_VerifyPurchase for the Steam VendorType using the downloadable content EntitlementType.
 *
 * @see EOS_ReceiptValidator_VerifyPurchaseOptions
 */
EOS_STRUCT(EOS_Steam_DLC_VerifyPurchasePlatformOptions, (
	/** API Version: Set this to EOS_STEAM_DLC_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** The Steam Application ID */
	const char* AppId;
	/** SteamID of user */
	const char* SteamId;
));

/** The most recent version of the EOS_EGS_IAP_VerifyPurchasePlatformOptions structure */
#define EOS_EGS_IAP_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST 1

/**
 * Platform specific options for EOS_ReceiptValidator_VerifyPurchase for the Epic Games Store VendorType using the in-app purchase EntitlementType.
 *
 * @see EOS_ReceiptValidator_VerifyPurchaseOptions
 */
EOS_STRUCT(EOS_EGS_IAP_VerifyPurchasePlatformOptions, (
	/** API Version: Set this to EOS_EGS_IAP_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** Entitlement Name */
	const char* EntitlementName;
	/** Entitlement ID */
	const char* EntitlementId;
));

/** The most recent version of the EOS_EGS_DLC_VerifyPurchasePlatformOptions structure */
#define EOS_EGS_DLC_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST 1

/**
 * Platform specific options for EOS_ReceiptValidator_VerifyPurchase for the Epic Games Store VendorType using the downloadable content EntitlementType.
 *
 * @see EOS_ReceiptValidator_VerifyPurchaseOptions
 */
EOS_STRUCT(EOS_EGS_DLC_VerifyPurchasePlatformOptions, (
	/** API Version: Set this to EOS_EGS_DLC_VERIFYPURCHASEPLATFORMOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** Catalog Item ID */
	const char* CatalogItemId;
	/** Entitlement ID */
	const char* EntitlementId;
));

#pragma pack(pop)

