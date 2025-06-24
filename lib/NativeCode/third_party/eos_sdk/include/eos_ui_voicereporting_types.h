// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"
#include "eos_rtc_audiomoderation_types.h"

#pragma pack(push, 8)

/** The most recent version of the EOS_UI_PromptVoiceTerms API. */
#define EOS_UI_PROMPTVOICETERMS_API_LATEST 1

/**
 * Parameters for the EOS_UI_PromptVoiceTerms function.
 */
EOS_STRUCT(EOS_UI_PromptVoiceTermsOptions, (
	/** API Version: Set this to EOS_UI_PROMPTVOICETERMS_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Online Services Account ID of the user to prompt. */
	EOS_EpicAccountId LocalUserId;
	/** The Key for the agreement to prompt. */
	const char* AgreementKey;
));

/** The most recent version of the EOS_UI_ShowReportVoice API. */
#define EOS_UI_SHOWREPORTVOICE_API_LATEST 1

/**
 * Parameters for the EOS_UI_ShowReportVoice function.
 */
EOS_STRUCT(EOS_UI_ShowReportVoiceOptions, (
	/** API Version: Set this to EOS_UI_SHOWREPORTVOICE_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Online Services Account ID of the user who is requesting the Report. */
	EOS_EpicAccountId LocalUserId;
	/** The Record ID that should be reported. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * Output parameters for the EOS_UI_ShowReportVoice function.
 */
EOS_STRUCT(EOS_UI_ShowReportVoiceCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_UI_ShowReportVoice */
	void* ClientData;
	/** The Epic Online Services Account ID of the user who requested the Report. */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_UI_ShowReportVoice
 * @param Data A EOS_UI_ShowReportVoiceCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_UI_OnShowReportVoiceCallback, const EOS_UI_ShowReportVoiceCallbackInfo* Data);

#pragma pack(pop)
