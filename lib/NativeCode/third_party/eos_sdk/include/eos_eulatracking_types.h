// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

/** Handle to the EULA Tracking interface */
EOS_EXTERN_C typedef struct EOS_EULATrackingHandle* EOS_HEULATracking;

/** Max length of an AgreementKey string */
#define EOS_EULATRACKING_MAX_AGREEMENT_KEY_LENGTH 64

/** The most recent version of the EOS_EULATracking_QueryUserAgreementStatus API. */
#define EOS_EULATRACKING_QUERYUSERAGREEMENTSTATUS_API_LATEST 1

/**
 * Input parameters for the EOS_EULATracking_QueryUserAgreementStatus function.
 */
EOS_STRUCT(EOS_EULATracking_QueryUserAgreementStatusOptions, (
	/** API Version: Set this to EOS_EULATRACKING_QUERYUSERAGREEMENTSTATUS_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the user whose response status being queried. */
	EOS_EpicAccountId LocalUserId;
	/** The Key for the agreement to retrieve the status for. */
	const char* AgreementKey;
));

/**
 * Output parameters for the EOS_EULATracking_QueryUserAgreementStatus function.
 */
EOS_STRUCT(EOS_EULATracking_QueryUserAgreementStatusCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_EULATracking_QueryUserAgreementStatus */
	void* ClientData;
	/** The Epic Account ID of the user whose agreement status was queried. */
	EOS_EpicAccountId LocalUserId;
	/** True if the request was successful and the user has accepted the latest version of the specified agreement. */
	EOS_Bool bIsAccepted;
	/** True if the request was successful and user has responded in any manner to the latest version of the specified agreement. */
	EOS_Bool bHasResponse;
));

/**
 * Function prototype definition for callbacks passed to EOS_EULATracking_QueryUserAgreementStatus
 * @param Data A EOS_EULATracking_QueryUserAgreementStatusCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_EULATracking_OnQueryUserAgreementStatusCallback, const EOS_EULATracking_QueryUserAgreementStatusCallbackInfo* Data);


 /** The most recent version of the EOS_EULATracking_AddNotifyUserAgreementStatusUpdate API. */
#define EOS_EULATRACKING_ADDNOTIFYUSERAGREEMENTSTATUSUPDATE_API_LATEST 1

/** Input parameters for the EOS_EULATracking_AddNotifyUserAgreementStatusUpdate function. */
EOS_STRUCT(EOS_EULATracking_AddNotifyUserAgreementStatusUpdateOptions, (
	/** API Version: Set this to EOS_EULATRACKING_ADDNOTIFYUSERAGREEMENTSTATUSUPDATE_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Structure containing information about an updated agreement status.
 */
EOS_STRUCT(EOS_EULATracking_OnUserAgreementStatusUpdateCallbackInfo, (
	/** Client-specified data passed into EOS_EULATracking_AddNotifyUserAgreementStatusUpdate */
	void* ClientData;
	/** The Epic Account ID of the local user who the agreement status update is for */
	EOS_EpicAccountId LocalUserId;
	/** The key for the agreement the status update correlates to. */
	const char* AgreementKey;
	/** The latest version of the agreement that this status correlates to. */
	int32_t AgreementVersion;
	/** The status of whether the user has accepted the agreement or not. */
	EOS_Bool bHasAccepted;
));

/**
 * Callback for information related to an agreement status update.
 */
EOS_DECLARE_CALLBACK(EOS_EULATracking_OnUserAgreementStatusUpdateCallback, const EOS_EULATracking_OnUserAgreementStatusUpdateCallbackInfo* Data);

#pragma pack(pop)
