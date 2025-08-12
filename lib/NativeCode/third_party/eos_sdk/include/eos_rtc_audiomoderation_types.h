// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_RTCAudioModerationHandle* EOS_HRTCAudioModeration;

/** Handle to an existing created buffer (0 is an invalid handle) */
EOS_EXTERN_C typedef uint64_t EOS_RTCAudioModeration_BufferId;

/** An invalid buffer ID */
#define EOS_RTCAUDIOMODERATION_INVALID_BUFFERID ((EOS_RTCAudioModeration_BufferId)0)

/** Handle to an existing created record (0 is an invalid handle) */
EOS_EXTERN_C typedef uint64_t EOS_RTCAudioModeration_RecordId;

/** An invalid record ID */
#define EOS_RTCAUDIOMODERATION_INVALID_RECORDID ((EOS_RTCAudioModeration_RecordId)0)

/** The max available number of the created records at the same time. */
#define EOS_RTCAUDIOMODERATION_MAX_RECORDS 16

/** The most recent version of the EOS_RTCAudioModeration_CreateBuffer API. */
#define EOS_RTCAUDIOMODERATION_CREATEBUFFER_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_CreateBuffer
 */
EOS_STRUCT(EOS_RTCAudioModeration_CreateBufferOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_CREATEBUFFER_API_LATEST. */
	int32_t ApiVersion;
	/**
	* Recommended size 1Mb for phones and consoles, 10Mb for PC.
	* It's enough for nearly 1-5 minutes of audio. It can be related to the number of participants in the room and their activity.
	* Anyway, maximum duration of audio is 5 minutes. It can be changed by SetSetting
	*/
	uint64_t BufferSize;
));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_OnCreateBufferCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_CreateBufferCallbackInfo, (
	/** This returns:
	 * EOS_Success indicates that the operation succeeded.
	 * EOS_RTC_AllocationFailed if failed to allocate memory.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudioModeration_Create. */
	void* ClientData;
	/** The Buffer ID associated with the create audio buffer request. */
	EOS_RTCAudioModeration_BufferId BufferId;
	/** The size of create audio buffer. */
	uint64_t BufferSize;
));

/**
 * Callback for completion of create audio buffer request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnCreateBufferCallback, const EOS_RTCAudioModeration_CreateBufferCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudioModeration_DestroyBuffer API. */
#define EOS_RTCAUDIOMODERATION_DESTROYBUFFER_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_DestroyBuffer
 */
EOS_STRUCT(EOS_RTCAudioModeration_DestroyBufferOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_DESTROYBUFFER_API_LATEST. */
	int32_t ApiVersion;
	/** The Buffer ID that should be destroyed. */
	EOS_RTCAudioModeration_BufferId BufferId;
));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_OnDestroyBufferCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_DestroyBufferCallbackInfo, (
	/** This returns:
	 * EOS_Success indicates that the operation succeeded.
	 * EOS_NotFound if the Buffer with provided id is not exists.
	 * EOS_InvalidState if the Buffer assigned for existing record.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudioModeration_DestroyBuffer. */
	void* ClientData;
	/** The Buffer ID that is destroyed. */
	EOS_RTCAudioModeration_BufferId BufferId;
));

/**
 * Callback for completion of destroy buffer request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnDestroyBufferCallback, const EOS_RTCAudioModeration_DestroyBufferCallbackInfo* Data);


/** The most recent version of the EOS_RTCAudioModeration_Create API. */
#define EOS_RTCAUDIOMODERATION_CREATE_API_LATEST 2

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_Create
 */
EOS_STRUCT(EOS_RTCAudioModeration_CreateOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_CREATE_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this record should be attached on or null to attach for each room. */
	const char* RoomName;
	/** The Buffer ID that should be assigned to the record. */
	EOS_RTCAudioModeration_BufferId BufferId;
));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_OnCreateCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_CreateCallbackInfo, (
	/** This returns:
	 * EOS_Success indicates that the operation succeeded.
	 * EOS_NotFound if the Buffer isn't found by BufferId.
	 * EOS_InvalidState if the Buffer is already assigned to another record.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudioModeration_Create. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room this record should be attached on or null to attach for each room. */
	const char* RoomName;
	/** The Record ID associated with the create audio record request. */
	EOS_RTCAudioModeration_RecordId RecordId;
	/** The Buffer ID that is be assigned to the record. */
	EOS_RTCAudioModeration_BufferId BufferId;
));

/**
 * Callback for completion of create room record request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnCreateCallback, const EOS_RTCAudioModeration_CreateCallbackInfo* Data);


/** The most recent version of the EOS_RTCAudioModeration_Resume API. */
#define EOS_RTCAUDIOMODERATION_RESUME_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_Resume
 */
EOS_STRUCT(EOS_RTCAudioModeration_ResumeOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_RESUME_API_LATEST. */
	int32_t ApiVersion;
	/** The Record ID this settings should be applied on. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_OnResumeCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_ResumeCallbackInfo, (
	/** This returns:
	 * EOS_Success indicates that the operation succeeded.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudioModeration_Resume. */
	void* ClientData;
	/** The Record ID this settings should be applied on. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * Callback for completion of resume room record request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnResumeCallback, const EOS_RTCAudioModeration_ResumeCallbackInfo* Data);


/** The most recent version of the EOS_RTCAudioModeration_Suspend API. */
#define EOS_RTCAUDIOMODERATION_SUSPEND_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_Suspend
 */
EOS_STRUCT(EOS_RTCAudioModeration_SuspendOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_SUSPEND_API_LATEST. */
	int32_t ApiVersion;
	/** The Record ID this settings should be applied on. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_OnSuspendCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_SuspendCallbackInfo, (
	/** This returns:
	 * EOS_Success indicates that the operation succeeded.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudioModeration_Suspend. */
	void* ClientData;
	/** The Record ID this settings should be applied on. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * Callback for completion of suspend room record request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnSuspendCallback, const EOS_RTCAudioModeration_SuspendCallbackInfo* Data);


/** The most recent version of the EOS_RTCAudioModeration_Destroy API. */
#define EOS_RTCAUDIOMODERATION_DESTROY_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_Destroy
 */
EOS_STRUCT(EOS_RTCAudioModeration_DestroyOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_DESTROY_API_LATEST. */
	int32_t ApiVersion;
	/** The Record ID this settings should be applied on. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_OnDestroyCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_DestroyCallbackInfo, (
	/** This returns:
	 * EOS_Success indicates that the operation succeeded.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudioModeration_Destroy. */
	void* ClientData;
	/** The Record ID this settings should be applied on. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * Callback for completion of destroy record request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnDestroyCallback, const EOS_RTCAudioModeration_DestroyCallbackInfo* Data);

/** The most recent version of the EOS_RTCAudioModeration_StartUpload API. */
#define EOS_RTCAUDIOMODERATION_STARTUPLOAD_API_LATEST 2

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_UploadingProgressCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_UploadingProgressInfo, (
	/** The Record ID that is uploading. */
	EOS_RTCAudioModeration_RecordId RecordId;
	/** Client-specified data passed into EOS_RTCAudioModeration_StartUpload. */
	void* ClientData;
	/** Upload progress - 0.0f ~ 1.0f */
	float UploadProgress;
));

/**
 * Callback for progress of upload room record request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_UploadingProgressCallback, const EOS_RTCAudioModeration_UploadingProgressInfo* Data);

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_StartUpload
 */
EOS_STRUCT(EOS_RTCAudioModeration_StartUploadOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_STARTUPLOAD_API_LATEST. */
	int32_t ApiVersion;
	/** The Record ID this settings should be applied on. */
	EOS_RTCAudioModeration_RecordId RecordId;
	/** (Optional) The custom destination URL where the record will be uploaded. Set to NULL when needed upload to EOS service. */
	const char* DestinationUrl;
	/** (Optional) The callback to track uploading progress. */
	EOS_RTCAudioModeration_UploadingProgressCallback UploadingProgressCallback;
));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_OnStartUploadCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_StartUploadCallbackInfo, (
	/** This returns:
	 * EOS_Success indicates that the operation succeeded.
	 * EOS_Canceled: The request was canceled.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudioModeration_StartUpload. */
	void* ClientData;
	/** The Record ID this settings should be applied on. */
	EOS_RTCAudioModeration_RecordId RecordId;
	/** The destination URL where the record will be uploaded. */
	const char* DestinationUrl;
	/** The null-terminated string that is a token of uploaded report. Will be NULL if requested upload to custom destination URL. */
	const char* Token;
));

/**
 * Callback for completion of start upload room record request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnStartUploadCallback, const EOS_RTCAudioModeration_StartUploadCallbackInfo* Data);


/** The most recent version of the EOS_RTCAudioModeration_StopUpload API. */
#define EOS_RTCAUDIOMODERATION_STOPUPLOAD_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_StopUpload
 */
EOS_STRUCT(EOS_RTCAudioModeration_StopUploadOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_STOPUPLOAD_API_LATEST. */
	int32_t ApiVersion;
	/** The Record ID this settings should be applied on. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_OnStopUploadCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_StopUploadCallbackInfo, (
	/** This returns:
	 * EOS_Success indicates that the operation succeeded.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudioModeration_StopUpload. */
	void* ClientData;
	/** The Record ID this settings should be applied on. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * Callback for completion of stop upload room record request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnStopUploadCallback, const EOS_RTCAudioModeration_StopUploadCallbackInfo* Data);


/** The most recent version of the EOS_RTCAudioModeration_UpdateSendingSignature API. */
#define EOS_RTCAUDIOMODERATION_UPDATESENDINGSIGNATURE_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_UpdateSendingSignature
 */
EOS_STRUCT(EOS_RTCAudioModeration_UpdateSendingSignatureOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_UPDATESENDINGSIGNATURE_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/** 
	  * The signed public key which will be associated with the request. Receive from PKI service
	  */
	const char* SignedPublicKey;
	/**
	  * The private key which will be associated with the request.
	  */
	const uint8_t* PrivateKeyData;
	/**
	  * The private key's length which will be associated with the request.
	*/
	uint32_t PrivateKeyLength;

));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_OnUpdateSendingSignatureCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_UpdateSendingSignatureCallbackInfo, (
	/** This returns:
	 * EOS_Success indicates that the operation succeeded.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudioModeration_UpdateSendingSignature. */
	void* ClientData;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this settings should be applied on. */
	const char* RoomName;
	/**
	  * The signed public key which will be associated with the request.
	  */
	const char* SignedPublicKey;
	/**
	  * The private key which will be associated with the request.
	  */
	const uint8_t* PrivateKeyData;
	/**
	  * The private key's length will be associated with the request.
	  */
	uint32_t PrivateKeyLength;
	));

/**
 * Callback for completion of update sending signature request.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnUpdateSendingSignatureCallback, const EOS_RTCAudioModeration_UpdateSendingSignatureCallbackInfo* Data);


/** The most recent version of the EOS_RTCAudioModeration_QueryRecordInformation API. */
#define EOS_RTCAUDIOMODERATION_QUERYRECORDINFORMATION_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_QueryRecordInformation.
 */
EOS_STRUCT(EOS_RTCAudioModeration_QueryRecordInformationOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_QUERYRECORDINFORMATION_API_LATEST. */
	int32_t ApiVersion;
	/** The ID of a record which information should be queried. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_QueryRecordInformationCallback.
 */
EOS_STRUCT(EOS_RTCAudioModeration_QueryRecordInformationCallbackInfo, (
	/** This returns:
	 * EOS_Success indicates that the operation succeeded.
	 * EOS_NotFound if the Record isn't found by RecordId.
	 * EOS_UnexpectedError otherwise.
	 */
	EOS_EResult ResultCode;
	/** Client-specified data passed into EOS_RTCAudioModeration_QueryRecordInformationCallback. */
	void* ClientData;
	/** The ID of a record which information is queried. */
	EOS_RTCAudioModeration_RecordId RecordId;
));

/**
 * Callback for completion of a record information query.
 */
EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnQueryRecordInformationCallback, const EOS_RTCAudioModeration_QueryRecordInformationCallbackInfo* Data);


/** The most recent version of the EOS_RTCAudioModeration_RecordedParticipantInformation API. */
#define EOS_RTCAUDIOMODERATION_RECORDEDPARTICIPANTINFORMATION_API_LATEST 1

/**
 * This struct contains information about recorded participant.
 */
EOS_STRUCT(EOS_RTCAudioModeration_RecordedParticipantInformation, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_RECORDEDPARTICIPANTINFORMATION_API_LATEST. */
	int32_t ApiVersion;
	/** ID of recorded participant. */
	EOS_ProductUserId ParticipantId;
));

/** The most recent version of the EOS_RTCAudioModeration_RecordedRoomInformation API. */
#define EOS_RTCAUDIOMODERATION_RECORDEDROOMINFORMATION_API_LATEST 1

/**
 * This struct contains information about recorded room.
 */
EOS_STRUCT(EOS_RTCAudioModeration_RecordedRoomInformation, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_RECORDEDROOMINFORMATION_API_LATEST. */
	int32_t ApiVersion;
	/** The recorded room name */
	const char* RoomName;
	/** A number of participants whose audio is recorded in the room. */
	uint32_t ParticipantsCount;
	/** An array of participants whose audio is recorded in the room. */
	EOS_RTCAudioModeration_RecordedParticipantInformation* Participants;
));

/** The most recent version of the EOS_RTCAudioModeration_RecordInformation API. */
#define EOS_RTCAUDIOMODERATION_RECORDINFORMATION_API_LATEST 1

/**
 * This struct contains information about an audio record.
 */
EOS_STRUCT(EOS_RTCAudioModeration_RecordInformation, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_RECORDINFORMATION_API_LATEST. */
	int32_t ApiVersion;
	/** The target record ID */
	EOS_RTCAudioModeration_RecordId RecordId;
	/** A number of rooms that represented in the record. */
	uint32_t RoomsCount;
	/** An array of rooms that represented in the record. */
	EOS_RTCAudioModeration_RecordedRoomInformation* Rooms;
));

EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_RecordInformation_Release(EOS_RTCAudioModeration_RecordInformation* RecordInfo);


/** The most recent version of the EOS_RTCAudioModeration_CopyRecordInformation API. */
#define EOS_RTCAUDIOMODERATION_COPYRECORDINFORMATION_API_LATEST 1

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_CopyRecordInformation.
 */
EOS_STRUCT(EOS_RTCAudioModeration_CopyRecordInformationOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_COPYRECORDINFORMATION_API_LATEST. */
	int32_t ApiVersion;
	/** The Record ID that information is required. */
	EOS_RTCAudioModeration_RecordId RecordId;
));


/** Participant RTC's verification status change */
EOS_ENUM(EOS_ERTCAudioModerationParticipantVerificationStatus,
	/** Verification do not able for some reason */
	EOS_RTCAMPVS_Unverified = 0,
	/** Audio successfully verified */
	EOS_RTCAMPVS_Verified = 1,
	/** Audio failed verification */
	EOS_RTCAMPVS_VerificationFailed = 2
);

/** The most recent version of the EOS_RTCAudioModeration_AddNotifyParticipantStatusChanged API. */
#define EOS_RTCAUDIOMODERATION_ADDNOTIFYPARTICIPANTSTATUSCHANGED_API_LATEST 1

/**
 * This struct is used to call EOS_RTCAudioModeration_AddNotifyParticipantStatusChanged.
 */
EOS_STRUCT(EOS_RTCAudioModeration_AddNotifyParticipantStatusChangedOptions, (
	/** API Version: Set this to EOS_RTCAUDIOMODERATION_ADDNOTIFYPARTICIPANTSTATUSCHANGED_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID of the user trying to request this operation. */
	EOS_ProductUserId LocalUserId;
	/** The room this event is registered on. */
	const char* RoomName;
));

/**
 * This struct is passed in with a call to EOS_RTCAudioModeration_AddNotifyParticipantStatusChanged registered event.
 */
EOS_STRUCT(EOS_RTCAudioModeration_ParticipantStatusChangedCallbackInfo, (
	/** Client-specified data passed into EOS_RTCAudioModeration_AddNotifyParticipantStatusChanged. */
	void* ClientData;
	/** The Product User ID of the user who initiated this request. */
	EOS_ProductUserId LocalUserId;
	/** The room associated with this event. */
	const char* RoomName;
	/** The participant whose audio moderation status changed. */
	EOS_ProductUserId ParticipantId;
	/** Is audio moderation supported by participant. */
	EOS_Bool bIsSupported;
	/** What verification status change occurred. */
	EOS_ERTCAudioModerationParticipantVerificationStatus VerificationStatus;
));

EOS_DECLARE_CALLBACK(EOS_RTCAudioModeration_OnParticipantStatusChangedCallback, const EOS_RTCAudioModeration_ParticipantStatusChangedCallbackInfo* Data);

#pragma pack(pop)
