// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"
#include "eos_init.h"

#pragma pack(push, 8)

/** Handle to the game clips interface */
EOS_EXTERN_C typedef struct EOS_GameClipsHandle* EOS_HGameClips;

/** The most recent version of the EOS_GameClips_Activate API. */
#define EOS_GAMECLIPS_ACTIVATE_API_LATEST 2

/**
 * Options for initializing the Epic Online Services SDK.
 */
EOS_STRUCT(EOS_GameClips_ActivateOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_ACTIVATE_API_LATEST. */
	int32_t ApiVersion;
	/** Allocators are optional - if not provided allocations will fall back to the allocator provided to EOS_Initialize(). */
	/** A custom memory allocator, if desired. */
	EOS_AllocateMemoryFunc AllocateMemoryFunction;
	/** A corresponding memory reallocator. If the AllocateMemoryFunction is nulled, then this field must also be nulled. */
	EOS_ReallocateMemoryFunc ReallocateMemoryFunction;
	/** A corresponding memory releaser. If the AllocateMemoryFunction is nulled, then this field must also be nulled. */
	EOS_ReleaseMemoryFunc ReleaseMemoryFunction;
));

/** Game Clip feature availability */
EOS_ENUM(EOS_GameClips_EAvailability,
	/** GameClips feature availability has not yet been tested */
	EOS_GCA_Pending = 0,
	/** GameClips feature is available */
	EOS_GCA_Available = 1,
    /** Not supported on this platform */
    EOS_GCA_NotSupported = 2,
    /** Not configured for the current application */
    EOS_GCA_NotConfigured = 3,
	/** Not available */
	EOS_GCA_NotAvailable = 4,
	/** Recording was disabled by user in system settings */
    EOS_GCA_DisabledByUser = 5
);

/** GameClip recording status */
EOS_ENUM(EOS_GameClips_ERecording,
	/** GameClips are not currently recording in background */
	EOS_GCR_NotRecording = 0,
	/** GameClips are currently recording in background */
	EOS_GCR_Recording = 1,
	/** GameClips background recording is currently paused */
	EOS_GCR_Paused = 2
);

/** GameClip completion status */
EOS_ENUM(EOS_GameClips_ECompletion,
	/** Clip completion status not set */
	EOS_GCC_None = 0,
	/** Clip started recording */
	EOS_GCC_Started = 1,
	/** Clip started uploading */
	EOS_GCC_Uploading = 2,
	/** Clip successfully recorded and uploaded */
	EOS_GCC_Completed = 3,
	/** Clip failed to record */
	EOS_GCC_Error_Record = 4,
	/** Clip upload failed */
	EOS_GCC_Error_Upload = 5
);

/**
 * Current status of GameClip recording/upload.
 */
EOS_STRUCT(EOS_GameClips_Status, (
	/** GameClip feature availability */
	EOS_GameClips_EAvailability Availability;
	/** GameClip background recording status */
	EOS_GameClips_ERecording Recording;
	/** Available services targets for clip upload - if != 0 targets are listening. */
	uint32_t AvailableTargets;
    /** Number of clips that are pending upload */
    uint32_t PendingUpload;
    /** Overall progress of upload - 0.0f ~ 1.0f */
    float Progress;
	/** Clip that has started/completed/failed to record and upload */
	int32_t ClipID;
	/** Clip completion/failure status */
	EOS_GameClips_ECompletion CompletionStatus;

	uint32_t Reserved1;
	uint32_t Reserved2;
	));

/**
 * Output parameters for the EOS_GameClips_OnStatusChangedCallback Function.
 */
EOS_STRUCT(EOS_GameClips_StatusChangedInfo, (
	/** Context that was passed into EOS_GameClips_AddNotifyStatusChanged */
	void* ClientData;
    /** Updated status of game clip recording/upload. */
    EOS_GameClips_Status NewStatus;
));

/**
 * Callback that is issued when GameClips status changes.
 */
EOS_DECLARE_CALLBACK(EOS_GameClips_OnStatusChangedCallback, const EOS_GameClips_StatusChangedInfo* Data);

/**
 * Output parameters for the EOS_GameClips_OnUserStatusChangedCallback Function.
 */
EOS_STRUCT(EOS_GameClips_UserStatusChangedCallbackInfo, (
	/** Context that was passed into EOS_GameClips_AddNotifyUserStatusChanged */
	void* ClientData;
	/** The Epic Account ID of the local user who has logged in */
	EOS_EpicAccountId LocalUserId;
	/** Whether user is logged into epic account or not */
	EOS_Bool bIsLoggedIn;
	/** Whether user account is connected to PostParty app or not */
	EOS_Bool bIsConnectedToPostParty;
));

/**
 * Callback that is issued when user connection status changes.
 */
EOS_DECLARE_CALLBACK(EOS_GameClips_OnUserStatusChangedCallback, const EOS_GameClips_UserStatusChangedCallbackInfo* Data);

/**
 * Output parameters for the EOS_GameClips_OnNotifyLogCollectionRequestedCallback Function.
 */
EOS_STRUCT(EOS_GameClips_OnNotifyLogCollectionRequestedCallbackInfo, (
	/** Context that was passed into EOS_GameClips_AddNotifyLogCollectionRequested */
	void* ClientData;
	/** The ReportId that needs to be associated with the log collection. i.e: included in the filename */
	const char* ReportId;
));

/**
 * Callback that is issued when the game logs should be collected.
 */
EOS_DECLARE_CALLBACK(EOS_GameClips_OnNotifyLogCollectionRequestedCallback, const EOS_GameClips_OnNotifyLogCollectionRequestedCallbackInfo* Data);

/** The most recent version of the EOS_GameClips_EnableOptions struct. */
#define EOS_GAMECLIPS_ENABLE_API_LATEST 1

/**
 * Input parameters for the EOS_GameClips_Enable function.
 */
EOS_STRUCT(EOS_GameClips_EnableOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_ENABLE_API_LATEST. */
	int32_t ApiVersion;
    /** The duration (in seconds) of clips recording in the background. */
    uint32_t ClipDuration;
));

/** The most recent version of the EOS_GameClips_StatusOptions struct. */
#define EOS_GAMECLIPS_CAPTURE_API_LATEST 1

/**
 * Input parameters for the EOS_GameClips_Capture function.
 */
EOS_STRUCT(EOS_GameClips_CaptureOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_CAPTURE_API_LATEST. */
	int32_t ApiVersion;
	/** Optional event that caused this clip to be captured - set to NULL to not specify */
	const char* EventType;
));

/** The most recent version of the EOS_GameClips_StatusOptions struct. */
#define EOS_GAMECLIPS_CREATE_API_LATEST 1

/**
 * Input parameters for the EOS_GameClips_Capture function.
 */
EOS_STRUCT(EOS_GameClips_CreateOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_CREATE_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the local user for who to upload the clip - if not specified the first logged in user will be used */
	EOS_EpicAccountId LocalUserId;
	/** Optional event that caused this clip to be captured - set to NULL to not specify */
	const char* EventType;
));

/** GameClip creation progress status */
EOS_ENUM(EOS_GameClips_EClipStatus,
	/** Clip creation requested */
	EOS_GCS_Requested = 0,
	/** Clip is being created */
	EOS_GCS_Creating = 1,
	/** Clip is being pending upload time available */
	EOS_GCS_PendingUpload = 2,
	/** Clip is being uploaded - see upload progress */
	EOS_GCS_Uploading = 3,
	/** Clip was successfully created and uploaded */
	EOS_GCS_Complete = 4,
	/** Clip creation/upload resulted in error */
	EOS_GCS_Error = 5
);

/**
 * Output parameters for the EOS_GameClips_CreateProgressCallback Function.
 */
EOS_STRUCT(EOS_GameClips_CreateProgressCallbackInfo, (
	/**
	 * The EOS_EResult code for the operation. Use EOS_EResult_IsOperationComplete() to determine if create operation is complete.
	 * Once complete, EOS_Success indicates that the operation succeeded; other codes indicate errors.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_GameClips_Create */
	void* ClientData;
	/** ID for clip - matches return value of EOS_GameClips_Create */
	int32_t ClipID;
	/** Current status of clip */
	EOS_GameClips_EClipStatus Status;
	/** Upload progress - 0.0f ~ 1.0f */
	float UploadProgress;
));

/**
 * Function prototype definition for callbacks passed to EOS_GameClips_Create.
 * @param Data A EOS_GameClips_CreateProgressCallbackInfo containing the progress information and result
 */
EOS_DECLARE_CALLBACK(EOS_GameClips_CreateProgressCallback, const EOS_GameClips_CreateProgressCallbackInfo* Data);


/** The most recent version of the EOS_GameClips_StatusOptions API. */
#define EOS_GAMECLIPS_STATUS_API_LATEST 1

/**
 * Input parameters for the EOS_GameClips_Enable function.
 */
EOS_STRUCT(EOS_GameClips_StatusOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_STATUS_API_LATEST. */
	int32_t ApiVersion;
    /** The status structure to be filled with the current status */
    EOS_GameClips_Status Status;
));

/** The most recent version of the EOS_GameClips_SetSetting API. */
#define EOS_GAMECLIPS_SETSETTING_API_LATEST 1

/**
 * Input parameters for the EOS_GameClips_SetSetting function.
 *
 * Available values of SettingName:
 */
EOS_STRUCT(EOS_GameClips_SetSettingOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_SETSETTING_API_LATEST. */
	int32_t ApiVersion;
	/** Setting that should be set. */
	const char* SettingName;
	/** Value to set the setting to. */
	const char* SettingValue;
));

/** The most recent version of the EOS_GameClips_EnableCleanAudio API. */
#define EOS_GAMECLIPS_ENABLECLEANAUDIO_API_LATEST 1

/**
 * Input parameters for the EOS_GameClips_EnableCleanAudio function.
 */
EOS_STRUCT(EOS_GameClips_EnableCleanAudioOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_ENABLECLEANAUDIO_API_LATEST. */
	int32_t ApiVersion;
	/** Whether or not clean audio should be enabled */
	EOS_Bool bEnable;
));

/** The most recent version of the EOS_GameClips_SendCleanAudio API. */
#define EOS_GAMECLIPS_SENDCLEANAUDIO_API_LATEST 1

/**
 * Input parameters for the EOS_GameClips_SendCleanAudio function.
 */
EOS_STRUCT(EOS_GameClips_SendCleanAudioOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_SENDCLEANAUDIO_API_LATEST. */
	int32_t ApiVersion;
	/** Pointer to the data with the interleaved stereo audio frames in signed 16 bits format. */
	const int16_t* Frames;
	/** Number of frames (stereo samples) in the Frames buffer */
	uint32_t NumFrames;
	/** Sample rate for the samples in the Frames buffer. */
	uint32_t SampleRate;
));

/** Handle that is returned by EOS_GameClips_HideVideoArea() and can be used to call EOS_GameClips_UnHideVideoArea(). */
EOS_EXTERN_C typedef uint64_t EOS_GameClips_HideHandle;

/** An invalid GameClips hide handle. */
#define EOS_GAMECLIPS_HIDEHANDLE_INVALID 0

/** The most recent version of the EOS_GameClips_HideVideoArea API. */
#define EOS_GAMECLIPS_HIDEVIDEOAREA_API_LATEST 1

/**
 * Input parameters for the EOS_GameClips_HideVideoArea function.
 */
EOS_STRUCT(EOS_GameClips_HideVideoAreaOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_HIDEVIDEOAREA_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * Left area on video to hide/blur.
	 * 
	 * The coordinates are normalized from 0-1 with [0,0] being the top/left corner
	 * and [1,1] being the bottom/right corner.
	 */
	float AreaLeft;
	/**
	 * Top area on video to hide/blur.
	 */
	float AreaTop;
	/**
	 * Right area on video to hide/blur.
	 */
	float AreaRight;
	/**
	 * Bottom area on video to hide/blur.
	 */
	float AreaBottom;
));

/** The most recent version of the EOS_GameClips_RecordVideoFrame API. */
#define EOS_GAMECLIPS_RECORDVIDEOFRAMECOMPLETEDINFO_API_LATEST 1

/**
 * Output parameters for the EOS_GameClips_OnStatusChangedCallback Function.
 */
EOS_STRUCT(EOS_GameClips_RecordVideoFrameCompletedInfo, (
	/** API Version: Will be set to EOS_GAMECLIPS_RECORDVIDEOFRAMECOMPLETEDINFO_API_LATEST. */
	int32_t ApiVersion;
	/** Context that was passed into EOS_GameClips_RecordVideoFrame */
	void* ClientData;
));

/**
 * Callback that is issued when recording (encoding) a video frame was completed.
 */
EOS_DECLARE_CALLBACK(EOS_GameClips_OnRecordVideoFrameCompleted, const EOS_GameClips_RecordVideoFrameCompletedInfo* Data);

/** The most recent version of the EOS_GameClips_RecordVideoFrame API. */
#define EOS_GAMECLIPS_RECORDVIDEOFRAME_API_LATEST 1

/**
 * Input parameters for the EOS_GameClips_RecordVideoFrame function.
 */
EOS_STRUCT(EOS_GameClips_RecordVideoFrameOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_RECORDVIDEOFRAME_API_LATEST. */
	int32_t ApiVersion;
	/** Frame width - keep constant after recording the first frame. */ 
	uint16_t FrameWidth;
	/** Frame height - keep constant after recording the first frame. */
	uint16_t FrameHeight;
	/** 
	 * This field is for system specific initialization if any.
	 *
	 * If provided then the structure will be located in <System>/eos_<system>_gameclips.h.
	 * The structure will be named EOS_<System>_VideoFrame.
	 */
	const void* SystemVideoFrame;
));

/** Max length of a GameClips meta information key name. */
#define EOS_GAMECLIPS_META_MAX_KEY_LENGTH 128

/** Max length of a GameClips meta information value. */
#define EOS_GAMECLIPS_META_MAX_VALUE_LENGTH 128

/** The most recent version of the EOS_GameClips_SetMeta API. */
#define EOS_GAMECLIPS_SETMETA_API_LATEST 1

/**
 * Input parameters for the EOS_GameClips_SetMeta function.
 */
EOS_STRUCT(EOS_GameClips_SetMetaOptions, (
	/** API Version: Set this to EOS_GAMECLIPS_SETMETA_API_LATEST. */
	int32_t ApiVersion;
	/** Meta information key, key length is limited to EOS_GAMECLIPS_META_MAX_KEY_LENGTH characters */
	const char* MetaKey;
	/** Meta information value, value length is limited to EOS_GAMECLIPS_META_MAX_VALUE_LENGTH characters.
	 *  If passing NULL the meta information for MetaKey will be removed. */
	const char* MetaValue;
));

/** The attribute type for EOS_GameClips_MetricAttribute */
EOS_ENUM(EOS_GameClips_MetricAttributeValueType,
	/** Attribute is a string type */
	EOS_GCAT_String = 0,
	/** Attribute is a numeric type */
	EOS_GCAT_Numeric = 1,
	/** Attribute is a boolean type */
	EOS_GCAT_Bool = 2,
	/** Attribute is a fractional numeric type (float) */
	EOS_GCAT_Fractional = 3
);

EOS_STRUCT(EOS_GameClips_MetricAttribute, (
	/** Name of the attribute */
	const char* Key;
	/** Type of the represented attribute */
	const EOS_GameClips_MetricAttributeValueType AttributeType;
	/** String value of the attribute */
	const char* StringValue;
	/** int64 value of the attribute */
	const int64_t NumericValue;
	/** bool value of the attribute */
	const EOS_Bool BoolValue;
	/** double value of the attribute */
	const double FractionalValue;
));

/**
 * Output parameters for the EOS_GameClips_AddNotifyMetricPublished function.
 */
EOS_STRUCT(EOS_GameClips_OnMetricPublishedCallbackInfo, (
	/** Context that was passed into EOS_GameClips_AddNotifyMetricPublished */
	void* ClientData;
	/** Name of the analytics event being published. */
	const char* EventName;
	/** Number of attributes in associated with the analytics event being published. */
	int32_t NumAttributes;
	/** Array of attributes associated with the analytics event being published. */
	const EOS_GameClips_MetricAttribute* Attributes;
));

/**
 * Function prototype definition for callbacks passed to EOS_GameClips_AddNotifyMetricPublished.
 * @param Data A EOS_GameClips_OnMetricPublishedCallbackInfo containing the progress information and result
 */
EOS_DECLARE_CALLBACK(EOS_GameClips_OnMetricPublishedCallback, const EOS_GameClips_OnMetricPublishedCallbackInfo* Data);


#pragma pack(pop)
