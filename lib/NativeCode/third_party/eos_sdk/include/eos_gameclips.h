// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_gameclips_types.h"

/**
 * The GameClips Interface is designed to allow developers to capture video clips of gameplay and upload it.
 * All GameClips Interface calls take a handle of type EOS_HGameClips as the first parameter.
 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetGameClipsInterface function.
 *
 * @see EOS_Platform_GetGameClipsInterface
 */

/**
 * Activate feature before first use.
 * @note Before activating the game clip feature no resources will be consumed.
 *
 * @param Options Structure with options for background recording.
 *
 * @return EOS_Success if the operation completes successfully
 *         EOS_NotImplemented if not available on this platform.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_GameClips_Activate(EOS_HGameClips Handle, const EOS_GameClips_ActivateOptions* Options);

/**
 * Register to receive changes to game clips recording/upload status.
 * @note must call RemoveNotifyStatusChanged to remove the notification
 *
 * @param ClientData arbitrary data that is passed back to you in the callback
 * @param Notification a callback that is fired when the status of game clip recording/upload changes
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_GameClips_AddNotifyStatusChanged(EOS_HGameClips Handle, void* ClientData, const EOS_GameClips_OnStatusChangedCallback NotificationFn);

/**
 * Unregister from receiving changes to game clips recording/upload status.
 *
 * @param InId handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_GameClips_RemoveNotifyStatusChanged(EOS_HGameClips Handle, EOS_NotificationId InId);

/**
 * Register to receive changes to user connections status.
 * @note must call RemoveNotifyUserStatusChanged to remove the notification
 *
 * @param ClientData arbitrary data that is passed back to you in the callback
 * @param Notification a callback that is fired when the status of a user changes
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_GameClips_AddNotifyUserStatusChanged(EOS_HGameClips Handle, void* ClientData, const EOS_GameClips_OnUserStatusChangedCallback NotificationFn);

/**
 * Unregister from receiving changes to game clips recording/upload status.
 *
 * @param InId handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_GameClips_RemoveNotifyUserStatusChanged(EOS_HGameClips Handle, EOS_NotificationId InId);

/**
 * Register to receive requests to collect the game logs.
 * @note must call RemoveNotifyLogCollectionRequested to remove the notification
 *
 * @param ClientData arbitrary data that is passed back to you in the callback
 * @param Notification a callback that is fired when log collection should be triggered.
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_GameClips_AddNotifyLogCollectionRequested(EOS_HGameClips Handle, void* ClientData, const EOS_GameClips_OnNotifyLogCollectionRequestedCallback NotificationFn);

/**
 * Unregister from receiving requests to collect the game logs.
 *
 * @param InId handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_GameClips_RemoveNotifyLogCollectionRequested(EOS_HGameClips Handle, EOS_NotificationId InId);

/**
 * Enable background recording of game clip videos.
 *
 * @param Options Structure with options for background recording.
 *
 * @return EOS_Success if the operation completes successfully
 *         EOS_InvalidParameters if any of the options are incorrect
 *         EOS_InvalidRequest Not available - see `Available` field in EOS_GameClips_Status for details
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_GameClips_Enable(EOS_HGameClips Handle, const EOS_GameClips_EnableOptions* Options);

/**
 * Disable background recording of game clip videos.
 *
 * @note Pending uploads will still be completed but the status update callback will
 *       be cleared.
 */
EOS_DECLARE_FUNC(void) EOS_GameClips_Disable(EOS_HGameClips Handle);

/**
 * Capture a clip and upload it.
 *
 * @param Options Structure with options for background recording.
 *
 * @return EOS_Success if feature was enabled and capture was started.
 *         EOS_InvalidRequest Not available - see `Available` field in EOS_GameClips_Status for details
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_GameClips_Capture(EOS_HGameClips Handle, const EOS_GameClips_CaptureOptions* Options);

/**
 * Create a video clip from the background recording and upload it.
 *
 * @param Options Structure with options for clip creation recording.
 * @param ClientData arbitrary data that is passed back to you in the CompletionDelegate
 * @param ProgressDelegate a callback that is fired to report progress and completion
 *
 * @return ClipID - a integer to identify clip.
 */
EOS_DECLARE_FUNC(int32_t) EOS_GameClips_Create(EOS_HGameClips Handle, const EOS_GameClips_CreateOptions* Options, void* ClientData, const EOS_GameClips_CreateProgressCallback ProgressDelegate);

/**
 * Use this function to control settings.
 *
 * The available settings are documented as part of EOS_GameClips_SetSettingOptions.
 *
 * @param Options structure containing the parameters for the operation
 * @return EOS_Success when the setting is successfully set, EOS_NotFound when the setting is unknown, EOS_InvalidParameters when the value is invalid.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_GameClips_SetSetting(EOS_HGameClips Handle, const EOS_GameClips_SetSettingOptions* Options);

/**
 * Record a single frame of video into the ring buffer.
 *
 * This function is only used on platforms that do not provide OS functionality to record video in the background.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData data that will be passed along to callback.
 * @param OnCompleted callback that will be issued when recording was completed and frame is no longer needed.
 * @return EOS_Success when the setting is successfully set, EOS_NotFound when the setting is unknown, EOS_InvalidParameters when the value is invalid.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_GameClips_RecordVideoFrame(EOS_HGameClips Handle, const EOS_GameClips_RecordVideoFrameOptions* Options, void* ClientData, const EOS_GameClips_OnRecordVideoFrameCompleted OnCompleted);

/**
 * Enable/disable 'clean' audio track to be used instead of the system audio track.
 *
 * Your game might need to replace the audio in the recorded video clips with a 'clean' version,
 * that - for example - does not contain voice chat or licensed audio.
 * Use this function to enable/disable 'clean' audio and then use EOS_GameClips_SendCleanAudio to provide
 * data for the audio track to use instead.
 *
 * @param Options structure containing the parameters for the operation
 * @return EOS_Success if the operation completes successfully
 *         EOS_InvalidParameters if any of the options are incorrect
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_GameClips_EnableCleanAudio(EOS_HGameClips Handle, const EOS_GameClips_EnableCleanAudioOptions* Options);

/**
 * Send 'clean' audio track to be used instead of the system audio track.
 *
 * @param Options structure containing the parameters for the operation
 * @return EOS_Success if the operation completes successfully
 *         EOS_InvalidState if clean audio is currently not enabled
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_GameClips_SendCleanAudio(EOS_HGameClips Handle, const EOS_GameClips_SendCleanAudioOptions* Options);

/**
 * Hide/blur a rectangular area in the recorded video.
 *
 * The function returns a handle that can be used to un-hide the area. You can hide more than
 * one area at a time.
 *
 * @param Options structure containing the parameters for the operation
 * @return EOS_GameClips_HideHandle with the handle to use to un-hide the area.
 */
EOS_DECLARE_FUNC(EOS_GameClips_HideHandle) EOS_GameClips_HideVideoArea(EOS_HGameClips Handle, const EOS_GameClips_HideVideoAreaOptions* Options);

/**
 * Un-hide a previously hidden area of the recorded video clip.
 *
 * @param HideHandle The handle returned from calling EOS_GameClips_HideVideoArea.
 */
EOS_DECLARE_FUNC(void) EOS_GameClips_UnHideVideoArea(EOS_HGameClips Handle, EOS_GameClips_HideHandle HideHandle);

/**
 * Set/clear meta information to be attached to the uploaded clip.
 *
 * @param Options structure containing the parameters for the operation.
 * @return EOS_Success if the operation completes successfully
 *         EOS_InvalidParameters if any of the options are incorrect
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_GameClips_SetMeta(EOS_HGameClips Handle, const EOS_GameClips_SetMetaOptions* Options);

/**
 * Register to receive the metrics collected by the GameClips plugin.
 * @note must call RemoveNotifyMetricPublished to remove the notification
 *
 * @param ClientData arbitrary data that is passed back to you in the callback
 * @param Notification a callback that is fired when a metric was collected.
 *
 * @return handle representing the registered callback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_GameClips_AddNotifyMetricPublished(EOS_HGameClips Handle, void* ClientData, const EOS_GameClips_OnMetricPublishedCallback NotificationFn);

/**
 * Unregister from receiving metrics from the GameClips plugin.
 *
 * @param InId handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_GameClips_RemoveNotifyMetricPublished(EOS_HGameClips Handle, EOS_NotificationId InId);
