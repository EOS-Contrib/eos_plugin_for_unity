// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_ui_types.h"

/**
 * The UI Interface is used to access the Social Overlay UI.  Each UI component will have a function for
 * opening it.  All UI Interface calls take a handle of type EOS_HUI as the first parameter.
 * This handle can be retrieved from an EOS_HPlatform handle by using the EOS_Platform_GetUIInterface function.
 *
 * @see EOS_Platform_GetUIInterface
 */

/**
 * Opens the Social Overlay with a request to show the friends list.
 *
 * @param Options Structure containing the Epic Account ID of the friends list to show.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate A callback that is fired when the request to show the friends list has been sent to the Social Overlay, or on an error.
 *
 * @see EOS_UI_ShowFriendsOptions
 * @see EOS_UI_OnShowFriendsCallback
 */
EOS_DECLARE_FUNC(void) EOS_UI_ShowFriends(EOS_HUI Handle, const EOS_UI_ShowFriendsOptions* Options, void* ClientData, const EOS_UI_OnShowFriendsCallback CompletionDelegate);

/**
 * Hides the active Social Overlay.
 *
 * @param Options Structure containing the Epic Account ID of the browser to close.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate A callback that is fired when the request to hide the friends list has been processed, or on an error.
 *
 * @see EOS_UI_HideFriendsOptions
 * @see EOS_UI_OnHideFriendsCallback
 */
EOS_DECLARE_FUNC(void) EOS_UI_HideFriends(EOS_HUI Handle, const EOS_UI_HideFriendsOptions* Options, void* ClientData, const EOS_UI_OnHideFriendsCallback CompletionDelegate);

/**
 * Gets the friends overlay visibility.
 *
 * @param Options Structure containing the Epic Account ID of the friends Social Overlay owner.
 *
 * @return EOS_TRUE If the overlay is visible.
 *
 * @see EOS_UI_GetFriendsVisibleOptions
 */
EOS_DECLARE_FUNC(EOS_Bool) EOS_UI_GetFriendsVisible(EOS_HUI Handle, const EOS_UI_GetFriendsVisibleOptions* Options);

/**
 * Gets the friends overlay exclusive input state.
 *
 * @param Options Structure containing the Epic Account ID of the friends Social Overlay owner.
 *
 * @return EOS_TRUE If the overlay has exclusive input.
 *
 * @see EOS_UI_GetFriendsExclusiveInputOptions
 */
EOS_DECLARE_FUNC(EOS_Bool) EOS_UI_GetFriendsExclusiveInput(EOS_HUI Handle, const EOS_UI_GetFriendsExclusiveInputOptions* Options);

/**
 * Register to receive notifications when the overlay display settings are updated.
 * Newly registered handlers will always be called the next tick with the current state.
 * @note If the returned NotificationId is valid, you must call EOS_UI_RemoveNotifyDisplaySettingsUpdated when you no longer wish to have your NotificationHandler called.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the NotificationFn.
 * @param NotificationFn A callback that is fired when the overlay display settings are updated.
 *
 * @return handle representing the registered callback
 *
 * @see EOS_UI_AddNotifyDisplaySettingsUpdatedOptions
 * @see EOS_UI_OnDisplaySettingsUpdatedCallback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_UI_AddNotifyDisplaySettingsUpdated(EOS_HUI Handle, const EOS_UI_AddNotifyDisplaySettingsUpdatedOptions* Options, void* ClientData, const EOS_UI_OnDisplaySettingsUpdatedCallback NotificationFn);

/**
 * Unregister from receiving notifications when the overlay display settings are updated.
 *
 * @param Id Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_UI_RemoveNotifyDisplaySettingsUpdated(EOS_HUI Handle, EOS_NotificationId Id);

/**
 * Updates the current Toggle Friends Key. This key can be used by the user to toggle the friends
 * overlay when available. The default value represents `Shift + F3` as `((int32_t)EOS_UIK_Shift | (int32_t)EOS_UIK_F3)`.
 * The provided key should satisfy EOS_UI_IsValidKeyCombination. The value EOS_UIK_None is specially handled
 * by resetting the key binding to the system default.
 *
 * @param Options Structure containing the key combination to use.
 *
 * @return EOS_EResult containing the result of the operation.
 * Possible result codes:
 * - EOS_Success If the overlay has been notified about the request.
 * - EOS_IncompatibleVersion if the API version passed in is incorrect.
 * - EOS_InvalidParameters If any of the options are incorrect.
 * - EOS_NotConfigured If the overlay is not properly configured.
 * - EOS_NoChange If the key combination did not change.
 *
 * @see EOS_UI_SetToggleFriendsKeyOptions
 * @see EOS_UI_IsValidKeyCombination
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_SetToggleFriendsKey(EOS_HUI Handle, const EOS_UI_SetToggleFriendsKeyOptions* Options);

/**
 * Returns the current Toggle Friends Key. This key can be used by the user to toggle the friends
 * overlay when available. The default value represents `Shift + F3` as `((int32_t)EOS_UIK_Shift | (int32_t)EOS_UIK_F3)`.
 *
 * @param Options Structure containing any options that are needed to retrieve the key.
 * @return A valid key combination which represent a single key with zero or more modifier keys.
 * - EOS_UIK_None will be returned if any error occurs.
 *
 * @see EOS_UI_GetToggleFriendsKeyOptions
 */
EOS_DECLARE_FUNC(EOS_UI_EKeyCombination) EOS_UI_GetToggleFriendsKey(EOS_HUI Handle, const EOS_UI_GetToggleFriendsKeyOptions* Options);

/**
 * Determine if a key combination is valid. A key combinations must have a single key and at least one modifier.
 * The single key must be one of the following: F1 through F12, Space, Backspace, Escape, or Tab.
 * The modifier key must be one or more of the following: Shift, Control, or Alt.
 *
 * @param KeyCombination The key to test.
 * @return EOS_TRUE if the provided key combination is valid.
 */
EOS_DECLARE_FUNC(EOS_Bool) EOS_UI_IsValidKeyCombination(EOS_HUI Handle, EOS_UI_EKeyCombination KeyCombination);

/**
 * Updates the current Toggle Friends Button. This button can be used by the user to toggle the friends
 * overlay when available.
 *
 * The default value is EOS_UISBF_None.
 * The provided button must satisfy EOS_UI_IsValidButtonCombination.
 *
 * On PC the EOS Overlay automatically listens to gamepad input and routes it to the overlay when appropriate. If this button is configured, the user may open the overlay using either this button or the toggle friends key.
 * On console platforms, the game must be calling EOS_UI_ReportInputState to route gamepad input to the EOS Overlay.
 *
 * Note: If you do not have a button mapped, it'll suppress the part of the toast notification that prompts the user to press it.
 *
 * @param Options Structure containing the button combination to use.
 *
 * @return EOS_EResult containing the result of the operation.
 * Possible result codes:
 * - EOS_Success If the overlay has been notified about the request.
 * - EOS_IncompatibleVersion if the API version passed in is incorrect.
 * - EOS_InvalidParameters If any of the options are incorrect.
 * - EOS_NotConfigured If the overlay is not properly configured.
 * - EOS_NoChange If the button combination did not change.
 *
 * @see EOS_UI_SetToggleFriendsButtonOptions
 * @see EOS_UI_IsValidButtonCombination
 * @see EOS_UI_ReportInputState
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_SetToggleFriendsButton(EOS_HUI Handle, const EOS_UI_SetToggleFriendsButtonOptions* Options);

/**
 * Returns the current Toggle Friends Button.  This button can be used by the user to toggle the friends
 * overlay when available. The default value is EOS_UISBF_None.
 *
 * @param Options Structure containing any options that are needed to retrieve the button.
 * @return A valid button combination which represents any number of buttons.
 * - EOS_UISBF_None will be returned if any error occurs.
 *
 * @see EOS_UI_GetToggleFriendsButtonOptions
 */
EOS_DECLARE_FUNC(EOS_UI_EInputStateButtonFlags) EOS_UI_GetToggleFriendsButton(EOS_HUI Handle, const EOS_UI_GetToggleFriendsButtonOptions* Options);

/**
 * Determine if a button combination is valid.
 *
 * @param ButtonCombination The button to test.
 * @return EOS_TRUE if the provided button combination is valid.
 */
EOS_DECLARE_FUNC(EOS_Bool) EOS_UI_IsValidButtonCombination(EOS_HUI Handle, EOS_UI_EInputStateButtonFlags ButtonCombination);

/**
 * Define any preferences for any display settings.
 *
 * @param Options Structure containing any options that are needed to set
 * @return EOS_EResult containing the result of the operation.
 * Possible result codes:
 * - EOS_Success If the overlay has been notified about the request.
 * - EOS_IncompatibleVersion if the API version passed in is incorrect.
 * - EOS_InvalidParameters If any of the options are incorrect.
 * - EOS_NotConfigured If the overlay is not properly configured.
 * - EOS_NoChange If the preferences did not change.
 *
 * @see EOS_UI_SetDisplayPreferenceOptions
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_SetDisplayPreference(EOS_HUI Handle, const EOS_UI_SetDisplayPreferenceOptions* Options);

/**
 * Returns the current notification location display preference.
 * @return The current notification location display preference.
 */
EOS_DECLARE_FUNC(EOS_UI_ENotificationLocation) EOS_UI_GetNotificationLocationPreference(EOS_HUI Handle);

/**
 * Lets the SDK know that the given UI event ID has been acknowledged and should be released.
 *
 * @return An EOS_EResult is returned to indicate success or an error.
 *
 * EOS_Success is returned if the UI event ID has been acknowledged.
 * EOS_NotFound is returned if the UI event ID does not exist.
 *
 * @see EOS_UI_AcknowledgeEventIdOptions
 * @see EOS_Presence_JoinGameAcceptedCallbackInfo
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_AcknowledgeEventId(EOS_HUI Handle, const EOS_UI_AcknowledgeEventIdOptions* Options);

/**
 * Pushes platform agnostic input state to the SDK. The state is passed to the EOS Overlay on console platforms.
 * This function has an empty implementation (i.e. returns EOS_NotImplemented) on all non-consoles platforms.
 *
 * @param Options Structure containing the input state
 *
 * @return EOS_EResult containing the result of the operation.
 * Possible result codes:
 * - EOS_Success If the Social Overlay has been notified about the request.
 * - EOS_IncompatibleVersion if the API version passed in is incorrect.
 * - EOS_NotConfigured If the Social Overlay is not properly configured.
 * - EOS_ApplicationSuspended If the application is suspended.
 * - EOS_NotImplemented If this function is not implemented on the current platform.
 *
 * @see EOS_UI_ReportInputStateOptions
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_ReportInputState(EOS_HUI Handle, const EOS_UI_ReportInputStateOptions* Options);

/**
 * Gives the Overlay the chance to issue its own drawing commands on console platforms.
 * Issued by the hosting application after it has finished the backbuffer and is ready to trigger presenting it.
 * As this process can be involved and rather varied depending on platform we do not plan to make the call
 * replace the standard "present" call, but rather expect it to be issued "just before" that call.
 * This function has an empty implementation (i.e. returns EOS_NotImplemented) on all non-consoles platforms.
 *
 * @param Options will vary from platform to platform.
 *        Main difference will be due to a platforms ability to provide multiple rendering queues.
 *
 * @return An EOS_EResult is returned to indicate success or an error.
 *
 * @see EOS_UI_PrePresentOptions
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_PrePresent(EOS_HUI Handle, const EOS_UI_PrePresentOptions* Options);

/**
 * Requests that the Social Overlay open and display the "Block User" flow for the specified user.
 *
 * @param ClientData Arbitrary data that is passed back to you in the NotificationFn.
 * @param CompletionDelegate A callback that is fired when the user exits the Block UI.
 *
 * @see EOS_UI_ShowBlockPlayerOptions
 * @see EOS_UI_OnShowBlockPlayerCallback
 */
EOS_DECLARE_FUNC(void) EOS_UI_ShowBlockPlayer(EOS_HUI Handle, const EOS_UI_ShowBlockPlayerOptions* Options, void* ClientData, const EOS_UI_OnShowBlockPlayerCallback CompletionDelegate);

/**
 * Requests that the Social Overlay open and display the "Report User" flow for the specified user.
 *
 * @param ClientData Arbitrary data that is passed back to you in the NotificationFn.
 * @param CompletionDelegate A callback that is fired when the user exits the Report UI.
 *
 * @see EOS_UI_ShowReportPlayerOptions
 * @see EOS_UI_OnShowReportPlayerCallback
 */
EOS_DECLARE_FUNC(void) EOS_UI_ShowReportPlayer(EOS_HUI Handle, const EOS_UI_ShowReportPlayerOptions* Options, void* ClientData, const EOS_UI_OnShowReportPlayerCallback CompletionDelegate);

/**
 * Sets the bIsPaused state of the overlay.
 * While true then all notifications will be delayed until after the bIsPaused is false again.
 * While true then the key and button events will not toggle the overlay.
 * If the Overlay was visible before being paused then it will be hidden.
 * If it is known that the Overlay should now be visible after being paused then it will be shown.
 *
 * @return EOS_EResult containing the result of the operation.
 * Possible result codes:
 * - EOS_Success If the overlay has been notified about the request.
 * - EOS_IncompatibleVersion if the API version passed in is incorrect.
 * - EOS_InvalidParameters If any of the options are incorrect.
 * - EOS_NotConfigured If the overlay is not properly configured.
 *
 * @see EOS_UI_PauseSocialOverlayOptions
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_PauseSocialOverlay(EOS_HUI Handle, const EOS_UI_PauseSocialOverlayOptions* Options);

/**
 * Gets the bIsPaused state of the overlay as set by any previous calls to EOS_UI_PauseSocialOverlay().
 *
 * @return EOS_TRUE If the overlay is paused.
 *
 * @see EOS_UI_IsSocialOverlayPausedOptions
 * @see EOS_UI_PauseSocialOverlay
 */
EOS_DECLARE_FUNC(EOS_Bool) EOS_UI_IsSocialOverlayPaused(EOS_HUI Handle, const EOS_UI_IsSocialOverlayPausedOptions* Options);

/**
 * Register to receive notifications from the memory monitor.
 * Newly registered handlers will always be called the next tick with the current state.
 * @note If the returned NotificationId is valid, you must call EOS_UI_RemoveNotifyMemoryMonitor when you no longer wish to have your NotificationHandler called.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the NotificationFn.
 * @param NotificationFn A callback that is fired when the overlay display settings are updated.
 *
 * @return handle representing the registered callback
 *
 * @see EOS_UI_AddNotifyMemoryMonitorOptions
 * @see EOS_UI_OnMemoryMonitorCallback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_UI_AddNotifyMemoryMonitor(EOS_HUI Handle, const EOS_UI_AddNotifyMemoryMonitorOptions* Options, void* ClientData, const EOS_UI_OnMemoryMonitorCallback NotificationFn);

/**
 * Unregister from receiving notifications when the memory monitor posts a notification.
 *
 * @param Id Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_UI_RemoveNotifyMemoryMonitor(EOS_HUI Handle, EOS_NotificationId Id);

/**
 * Requests that the native ID for a target player be identified and the native profile be displayed for that player.
 *
 * @param ClientData Arbitrary data that is passed back to you in the NotificationFn.
 * @param CompletionDelegate A callback that is fired when the profile has been shown.
 *
 * @see EOS_UI_ShowNativeProfileOptions
 * @see EOS_UI_OnShowNativeProfileCallback
 */
EOS_DECLARE_FUNC(void) EOS_UI_ShowNativeProfile(EOS_HUI Handle, const EOS_UI_ShowNativeProfileOptions* Options, void* ClientData, const EOS_UI_OnShowNativeProfileCallback CompletionDelegate);

/**
 * Configures the on screen keyboard.
 *
 * @note This API only works on Windows.
 *
 * @return EOS_EResult containing the result of the operation.
 * Possible result codes:
 * - EOS_Success If the on screen keyboard was successfully configured.
 * - EOS_IncompatibleVersion If the API version passed in is incorrect.
 * - EOS_InvalidParameters If any of the options are incorrect.
 * - EOS_Disabled If the overlay is not available.
 * - EOS_NotConfigured If the overlay is not ready.
 *
 * @see EOS_UI_ConfigureOnScreenKeyboardOptions
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_UI_ConfigureOnScreenKeyboard(EOS_HUI Handle, const EOS_UI_ConfigureOnScreenKeyboardOptions* Options);

/**
 * Register to receive notifications for when an on screen keyboard has been requested.
 *
 * @note This API only works on Windows.
 * @note This API will only fire notifications if the on screen keyboard has been configured for it.
 * @note If the returned EOS_NotificationId is valid, you must call EOS_UI_RemoveNotifyOnScreenKeyboardRequested when you no longer wish to receive notifications.
 *
 * @param Options A structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the NotificationFn.
 * @param NotificationFn A callback that is fired when an on screen keyboard has been requested.
 *
 * @return A handle representing the registered callback.
 *
 * @see EOS_UI_AddNotifyOnScreenKeyboardRequestedOptions
 * @see EOS_UI_OnScreenKeyboardRequestedCallback
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_UI_AddNotifyOnScreenKeyboardRequested(EOS_HUI Handle, const EOS_UI_AddNotifyOnScreenKeyboardRequestedOptions* Options, void* ClientData, const EOS_UI_OnScreenKeyboardRequestedCallback NotificationFn);

/**
 * Unregister from receiving notifications for when an on screen keyboard has been requested.
 *
 * @note This API only works on Windows.
 *
 * @param Id A handle representing the registered callback.
 */
EOS_DECLARE_FUNC(void) EOS_UI_RemoveNotifyOnScreenKeyboardRequested(EOS_HUI Handle, EOS_NotificationId Id);
