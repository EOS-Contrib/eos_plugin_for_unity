// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_rtc_audiomoderation_types.h"

/**
 * The RTC AudioModeration Interface. This is used to manage AudioModeration specific RTC features
 *
 * @see EOS_RTCAudio_GetModerationInterface
 */

/**
 * Creates an audio buffer.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when the operation completes, either successfully or in error
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidParameters if any of the parameters are incorrect
 *         EOS_RTC_AllocationFailed if failed to allocate memory
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_CreateBuffer(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_CreateBufferOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnCreateBufferCallback CompletionDelegate);

/**
 * Destroys an audio buffer clearing all allocated resources.
 *
 * The buffer might be destroyed if not assigned with any audio record.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when the operation completes, either successfully or in error
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidParameters if any of the parameters are incorrect
 *         EOS_NotFound if the Buffer with provided id is not exists
 *         EOS_InvalidState if the Buffer assigned for existing record
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_DestroyBuffer(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_DestroyBufferOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnDestroyBufferCallback CompletionDelegate);

/**
 * Creates an audio room record.
 *
 * The recording will be in a suspended state.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when the operation completes, either successfully or in error
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidParameters if any of the parameters are incorrect
 *         EOS_NotFound if the Buffer isn't found by BufferId.
 *         EOS_InvalidState if the Buffer is already assigned to another record.
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_Create(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_CreateOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnCreateCallback CompletionDelegate);

/**
 * Resumes a suspended room record.
 *
 * @note It is not allowed to resume recording during its uploading
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when the operation completes, either successfully or in error
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidParameters if any of the parameters are incorrect
 *         EOS_InvalidState if call to resume is made when RTC Record state is not suspended
 *         EOS_NoChange if call to resume is made when RTC Record state is already resumed
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_Resume(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_ResumeOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnResumeCallback CompletionDelegate);

/**
 * Suspends a resumed room record.
 *
 * It can be resumed later by demand.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when the operation completes, either successfully or in error
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidParameters if any of the parameters are incorrect
 *         EOS_InvalidState if call to suspend is made when RTC Record state is not resumed
 *         EOS_NoChange if call to suspend is made when RTC Record state is already suspended
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_Suspend(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_SuspendOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnSuspendCallback CompletionDelegate);

/**
 * Destroys an audio room record clearing all allocated resources.
 *
 * All uploadings will be stopped and resumed recordings will be suspended.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when the operation completes, either successfully or in error
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidParameters if any of the parameters are incorrect
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_Destroy(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_DestroyOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnDestroyCallback CompletionDelegate);

/**
 * Start the content uploading of a room record. This request will occur asynchronously, potentially over multiple frames.
 *
 * It can be cancelled at any time using EOS_RTCAudioModeration_StopUpload
 *
 * @note Recording will be temporary suspended if not yet to execute this operation.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when the operation completes, either successfully or in error
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidRequest if called not in game thread context
 *         EOS_InvalidParameters if any of the parameters are incorrect
 *         EOS_NotFound if the Record isn't found by RecordId.
 *
 * @see EOS_RTCAudioModeration_StopUpload
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_StartUpload(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_StartUploadOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnStartUploadCallback CompletionDelegate);

/**
 * Stop the content uploading of a room record.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when the operation completes, either successfully or in error
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidParameters if any of the parameters are incorrect
 *
 * @see EOS_RTCAudioModeration_StartUpload
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_StopUpload(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_StopUploadOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnStopUploadCallback CompletionDelegate);

/**
 * Use this function to change outgoing audio signature for a room.
 *
 * @note Due to internal implementation details, this function requires that you first register to any notification for room
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when the operation completes, either successfully or in error
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidParameters if any of the parameters are incorrect
 *         EOS_NotFound if the local user is not in the room
 *         EOS_InvalidState if the room has already been joined using another keys
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_UpdateSendingSignature(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_UpdateSendingSignatureOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnUpdateSendingSignatureCallback CompletionDelegate);

/**
 * Use this function to query information about an audio record.
 *
 * @note Recording will be temporary suspended if not yet to execute this operation.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when the operation completes, either successfully or in error
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidParameters if any of the parameters are incorrect
 *         EOS_NotFound if the record is not exists
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_QueryRecordInformation(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_QueryRecordInformationOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnQueryRecordInformationCallback CompletionDelegate);

/**
 * Use this function to get a copy of information of audio record.
 *
 * @note Returns a copy of information queried from the previous successfully completed EOS_RTCAudioModeration_QueryRecordInformation call.
 *
 * @param Options structure containing the parameters for the operation.
 * @param OutRecordInformation The audio record's information for the given record ID, if it exists, use EOS_RTCAudioModeration_RecordInformation_Release when finished
 * @return EOS_Success if the operation succeeded
 *         EOS_InvalidParameters if any of the parameters are incorrect
 *         EOS_NotFound if the record is not exists
 *
 * @see EOS_RTCAudioModeration_RecordInformation_Release
 * @see EOS_RTCAudioModeration_QueryRecordInformation
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTCAudioModeration_CopyRecordInformation(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_CopyRecordInformationOptions* Options, EOS_RTCAudioModeration_RecordInformation ** OutRecordInformation);

/**
 * Register to receive notifications when a participant's audio moderation status changes
 * If the returned NotificationId is valid, you must call EOS_RTCAudioModeration_RemoveNotifyParticipantStatusChanged when you no longer wish to have your CompletionDelegate called.
 *
 * If you register to this notification before joining a room, you will receive a notification for every member already in the room when you join said room.
 *
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when an audio moderation status change occurs
 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
 *
 * @note This notification is also raised for the local user.
 *
 * @see EOS_INVALID_NOTIFICATIONID
 * @see EOS_RTCAudioModeration_RemoveNotifyParticipantStatusChanged
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTCAudioModeration_AddNotifyParticipantStatusChanged(EOS_HRTCAudioModeration Handle, const EOS_RTCAudioModeration_AddNotifyParticipantStatusChangedOptions* Options, void* ClientData, const EOS_RTCAudioModeration_OnParticipantStatusChangedCallback CompletionDelegate);

/**
 * Unregister a previously bound notification handler from receiving participant audio moderation status change notifications
 *
 * @param NotificationId The Notification ID representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_RTCAudioModeration_RemoveNotifyParticipantStatusChanged(EOS_HRTCAudioModeration Handle, EOS_NotificationId NotificationId);
