// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_k3s_types.h"

/**
 * The K3S Interface is used to access the backend services for K3S, including the management of settings and playtime tracking. 
 * All K3S Interface calls take a handle of type EOS_HK3S as the first parameter.
 * This handle can be retrieved from an EOS_HPlatform handle by using the EOS_Platform_GetK3SInterface function.
 *
 * @see EOS_Platform_GetK3SInterface
 */

/**
 * Query the service for settings.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate A callback that is fired when the operation completes, either successfully or in error.
 *
 * @see EOS_K3S_QuerySettingsCallbackInfo
 *
 * Possible result codes:
 * - EOS_Success:             The query was successful.
 * - EOS_IncompatibleVersion: The API version passed in is incorrect.
 * - EOS_InvalidParameters:   An options parameter is incorrect.
 * - EOS_InvalidUser:         The local user does not exist.
 * - EOS_InvalidAuth:         The local user exists but is not logged in.
 * - EOS_TooManyRequests:     There are too many requests in progress at this time.
 */
EOS_DECLARE_FUNC(void) EOS_K3S_QuerySettings(EOS_HK3S Handle, const EOS_K3S_QuerySettingsOptions* Options, void* ClientData, const EOS_K3S_OnQuerySettings CompletionDelegate);

/**
 * Get the number of settings for the given user.
 *
 * @param Options A structure containing the input parameters for this operation.
 *
 * @return The number of settings for the given user.
 */
EOS_DECLARE_FUNC(uint32_t) EOS_K3S_GetSettingCount(EOS_HK3S Handle, const EOS_K3S_GetSettingCountOptions* Options);

/**
 * Copy a setting by the given index.
 *
 * If the call returns an EOS_Success result, the out parameter OutSetting must be passed to EOS_K3S_Setting_Release to release the memory associated with it.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param OutSetting The out parameter which will receive the EOS_K3S_Setting structure.
 *
 * @return EOS_Success:             The OutSetting was set successfully.
 *         EOS_IncompatibleVersion: The API version passed in is incorrect.
 *         EOS_InvalidParameters:   The OutSetting is null, or an input parameter is incorrect.
 *         EOS_InvalidUser:         The local user does not exist.
 *         EOS_InvalidAuth:         The local user exists but is not logged in.
 *         EOS_NotFound:            The local user has no cache, or there is no match for the given index.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_K3S_CopySettingByIndex(EOS_HK3S Handle, const EOS_K3S_CopySettingByIndexOptions* Options, EOS_K3S_Setting ** OutSetting);

/**
 * Copy a setting by the given namespace and setting name.
 *
 * If the call returns an EOS_Success result, the out parameter OutSetting must be passed to EOS_K3S_Setting_Release to release the memory associated with it.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param OutSetting The out parameter which will receive the EOS_K3S_Setting structure.
 *
 * @return EOS_Success:             The OutSetting was successfully set.
 *         EOS_IncompatibleVersion: The API version passed in is incorrect.
 *         EOS_InvalidParameters:   The OutSetting is null, or an input parameter is incorrect.
 *         EOS_LimitExceeded:       An input parameter string is too long.
 *         EOS_InvalidUser:         The local user does not exist.
 *         EOS_InvalidAuth:         The local user exists but is not logged in.
 *         EOS_NotFound:            The local user has no cache, or there is no match for the given namespace and setting name.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_K3S_CopySettingByName(EOS_HK3S Handle, const EOS_K3S_CopySettingByNameOptions* Options, EOS_K3S_Setting ** OutSetting);

/**
 * Send updated settings to the service.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate A callback that is fired when the operation completes, either successfully or in error.
 *
 * @see EOS_K3S_UpdateSettingsCallbackInfo
 *
 * Possible result codes:
 * - EOS_Success:             The request was successful.
 * - EOS_IncompatibleVersion: The API version passed in is incorrect.
 * - EOS_InvalidParameters:   An options parameter is incorrect.
 * - EOS_InvalidUser:         The local user does not exist.
 * - EOS_InvalidAuth:         The local user exists but is not logged in.
 *   EOS_LimitExceeded:       An input parameter string is too long.
 * - EOS_TooManyRequests:     There are too many requests in progress at this time.
 */
EOS_DECLARE_FUNC(void) EOS_K3S_UpdateSettings(EOS_HK3S Handle, const EOS_K3S_UpdateSettingsOptions* Options, void* ClientData, const EOS_K3S_OnUpdateSettings CompletionDelegate);

/**
 * Sets the current playtime activity.
 *
 * @param Options A structure containing the input parameters for this operation.
 *
 * @return EOS_Success:             The current activity was set successfully.
 *         EOS_IncompatibleVersion: The API version passed in is incorrect.
 *         EOS_InvalidUser:         The local user does not exist.
 *         EOS_InvalidAuth:         The local user exists but is not logged in.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_K3S_SetCurrentPlaytimeActivity(EOS_HK3S Handle, const EOS_K3S_SetCurrentPlaytimeActivityOptions* Options);

/**
 * Request additional playtime.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate A callback that is fired when the operation completes, either successfully or in error.
 *
 * @see EOS_K3S_RequestAdditionalPlaytimeCallbackInfo
 *
 * Possible result codes:
 * - EOS_Success:             The request was successful.
 * - EOS_IncompatibleVersion: The API version passed in is incorrect.
 * - EOS_InvalidParameters:   An options parameter is incorrect.
 * - EOS_InvalidUser:         The local user does not exist.
 * - EOS_InvalidAuth:         The local user exists but is not logged in.
 * - EOS_TooManyRequests      There are too many requests in progress at this time.
 */
EOS_DECLARE_FUNC(void) EOS_K3S_RequestAdditionalPlaytime(EOS_HK3S Handle, const EOS_K3S_RequestAdditionalPlaytimeOptions* Options, void* ClientData, const EOS_K3S_OnRequestAdditionalPlaytime CompletionDelegate);

/**
 * Grant additional playtime.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate A callback that is fired when the operation completes, either successfully or in error.
 *
 * @see EOS_K3S_RequestAdditionalPlaytimeCallbackInfo
 *
 * Possible result codes:
 * - EOS_Success:             The grant was successful.
 * - EOS_IncompatibleVersion: The API version passed in is incorrect.
 * - EOS_InvalidParameters:   An options parameter is incorrect.
 * - EOS_InvalidUser:         The local user does not exist.
 * - EOS_InvalidAuth:         The local user exists but is not logged in.
 * - EOS_TooManyRequests      There are too many requests in progress at this time.
 */
EOS_DECLARE_FUNC(void) EOS_K3S_GrantAdditionalPlaytime(EOS_HK3S Handle, const EOS_K3S_GrantAdditionalPlaytimeOptions* Options, void* ClientData, const EOS_K3S_OnGrantAdditionalPlaytime CompletionDelegate);

/**
 * Get the playtime for the given playtime activity and scope.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param OutPlaytime The out parameter which will receive the playtime.
 *
 * @return EOS_Success:             The OutPlaytime was successfully set.
 *         EOS_InvalidParameters:   The OutPlaytime is null.
 *         EOS_IncompatibleVersion: The API version passed in is incorrect.
 *         EOS_InvalidUser:         The local user does not exist.
 *         EOS_InvalidAuth:         The local user exists but is not logged in.
 *         EOS_NotFound:            The local user has no cache, or there is no cache for the given activity and scope.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_K3S_GetPlaytime(EOS_HK3S Handle, const EOS_K3S_GetPlaytimeOptions* Options, float* OutPlaytime);

/**
 * Copy a playtime schedule window.
 *
 * If the call returns an EOS_Success result, the out parameter OutPlaytimeScheduleWindow must be passed to EOS_K3S_PlaytimeScheduleWindow_Release to release the memory associated with it.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param OutPlaytimeScheduleWindow The out parameter which will receive the EOS_K3S_PlaytimeScheduleWindow structure.
 *
 * @return EOS_Success:             The OutPlaytimeScheduleWindow was successfully set.
 *         EOS_InvalidParameters:   The OutPlaytimeScheduleWindow is null.
 *         EOS_IncompatibleVersion: The API version passed in is incorrect.
 *         EOS_InvalidUser:         The local user does not exist.
 *         EOS_InvalidAuth:         The local user exists but is not logged in.
 *         EOS_NotFound:            The local user has no cache, or there is no cache for the given activity and scope.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_K3S_CopyPlaytimeScheduleWindow(EOS_HK3S Handle, const EOS_K3S_CopyPlaytimeScheduleWindowOptions* Options, EOS_K3S_PlaytimeScheduleWindow ** OutPlaytimeScheduleWindow);

/**
 * Register to receive notifications about changes to supservised settings.
 * If n settings are changed at once, the callback will be called n times for the number of settings that have changed.
 *
 * @note If the returned NotificationId is valid, you must call EOS_K3S_RemoveNotifySettingChanged when you no longer wish to have your NotificationHandler called.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a notification is received.
 *
 * @return A handle representing the registered callback.
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_K3S_AddNotifySettingChanged(EOS_HK3S Handle, const EOS_K3S_AddNotifySettingChangedOptions* Options, void* ClientData, const EOS_K3S_OnSettingChangedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a setting changes.
 *
 * @param InId The handle representing the registered callback.
 */
EOS_DECLARE_FUNC(void) EOS_K3S_RemoveNotifySettingChanged(EOS_HK3S Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when additional playtime is granted.
 *
 * @note If the returned NotificationId is valid, you must call EOS_K3S_RemoveNotifyAdditionalPlaytimeGranted when you no longer wish to have your NotificationHandler called.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a notification is received.
 *
 * @return A handle representing the registered callback.
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_K3S_AddNotifyAdditionalPlaytimeGranted(EOS_HK3S Handle, const EOS_K3S_AddNotifyAdditionalPlaytimeGrantedOptions* Options, void* ClientData, const EOS_K3S_OnAdditionalPlaytimeGrantedCallback NotificationFn);

/**
 * Unregister from receiving notifications when additional playtime is granted.
 *
 * @param InId The handle representing the registered callback.
 */
EOS_DECLARE_FUNC(void) EOS_K3S_RemoveNotifyAdditionalPlaytimeGranted(EOS_HK3S Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when a remaining playtime limit is reached.
 *
 * @note If the returned NotificationId is valid, you must call EOS_K3S_RemoveNotifyRemainingPlaytimeLimitReached when you no longer wish to have your NotificationHandler called.
 *
 * @param Options A structure containing the input parameters for this operation.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a notification is received.
 *
 * @return A handle representing the registered callback.
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_K3S_AddNotifyRemainingPlaytimeLimitReached(EOS_HK3S Handle, const EOS_K3S_AddNotifyRemainingPlaytimeLimitReachedOptions* Options, void* ClientData, const EOS_K3S_OnRemainingPlaytimeLimitReachedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a remaining playtime limit is reached.
 *
 * @param InId The handle representing the registered callback.
 */
EOS_DECLARE_FUNC(void) EOS_K3S_RemoveNotifyRemainingPlaytimeReached(EOS_HK3S Handle, EOS_NotificationId InId);
