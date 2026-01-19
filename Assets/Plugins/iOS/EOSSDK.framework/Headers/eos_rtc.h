// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_rtc_types.h"
#include "eos_rtc_audio_types.h"
#include "eos_rtc_data_types.h"

/**
 * The RTC Interface is used to manage joining and leaving rooms.
 *
 * @see EOS_Platform_GetRTCInterface
 */

/**
 * Get a handle to the Audio interface
 * @return EOS_HRTCAudio handle
 *
 * @see eos_rtc_audio.h
 * @see eos_rtc_audio_types.h
 */
EOS_DECLARE_FUNC(EOS_HRTCAudio) EOS_RTC_GetAudioInterface(EOS_HRTC Handle);

/**
 * Get a handle to the Data interface
 * @return EOS_HRTCData handle
 *
 * @see eos_rtc_data.h
 * @see eos_rtc_data_types.h
 */
EOS_DECLARE_FUNC(EOS_HRTCData) EOS_RTC_GetDataInterface(EOS_HRTC Handle);

/**
 * Use this function to join a room.
 *
 * This function does not need to called for the Lobby RTC Room system; doing so will return EOS_AccessDenied. The lobby system will
 * automatically join and leave RTC Rooms for all lobbies that have RTC rooms enabled.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate a callback that is fired when the async operation completes, either successfully or in error
 *
 * @see EOS_RTC_JoinRoomOptions
 * @see EOS_RTC_OnJoinRoomCallback
 */
EOS_DECLARE_FUNC(void) EOS_RTC_JoinRoom(EOS_HRTC Handle, const EOS_RTC_JoinRoomOptions* Options, void* ClientData, const EOS_RTC_OnJoinRoomCallback CompletionDelegate);

/**
 * Use this function to leave a room and clean up all the resources associated with it. This function has to always be called when the
 * room is abandoned even if the user is already disconnected for other reasons.
 *
 * This function does not need to called for the Lobby RTC Room system; doing so will return EOS_AccessDenied. The lobby system will
 * automatically join and leave RTC Rooms for all lobbies that have RTC rooms enabled.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate a callback that is fired when the async operation completes, either successfully or in error
 *
 * @see EOS_RTC_LeaveRoomOptions
 * @see EOS_RTC_OnLeaveRoomCallback
 */
EOS_DECLARE_FUNC(void) EOS_RTC_LeaveRoom(EOS_HRTC Handle, const EOS_RTC_LeaveRoomOptions* Options, void* ClientData, const EOS_RTC_OnLeaveRoomCallback CompletionDelegate);

/**
 * Use this function to block a participant already connected to the room. After blocking them no media will be sent or received between
 * that user and the local user. This method can be used after receiving the OnParticipantStatusChanged notification.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate a callback that is fired when the async operation completes, either successfully or in error
 *
 * @see EOS_RTC_BlockParticipantOptions
 * @see EOS_RTC_OnBlockParticipantCallback
 */
EOS_DECLARE_FUNC(void) EOS_RTC_BlockParticipant(EOS_HRTC Handle, const EOS_RTC_BlockParticipantOptions* Options, void* ClientData, const EOS_RTC_OnBlockParticipantCallback CompletionDelegate);

/**
 * Register to receive notifications when disconnected from the room. If the returned NotificationId is valid, you must call
 * EOS_RTC_RemoveNotifyDisconnected when you no longer wish to have your CompletionDelegate called.
 *
 * This function will always return EOS_INVALID_NOTIFICATIONID when used with lobby RTC room. To be notified of the connection
 * status of a Lobby-managed RTC room, use the EOS_Lobby_AddNotifyRTCRoomConnectionChanged function instead.
 *
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when a participant is disconnected from the room
 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
 *
 * @see EOS_INVALID_NOTIFICATIONID
 * @see EOS_RTC_AddNotifyDisconnectedOptions
 * @see EOS_RTC_OnDisconnectedCallback
 * @see EOS_RTC_RemoveNotifyDisconnected
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyDisconnected(EOS_HRTC Handle, const EOS_RTC_AddNotifyDisconnectedOptions* Options, void* ClientData, const EOS_RTC_OnDisconnectedCallback CompletionDelegate);

/**
 * Unregister a previously bound notification handler from receiving room disconnection notifications
 *
 * @param NotificationId The Notification ID representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyDisconnected(EOS_HRTC Handle, EOS_NotificationId NotificationId);

/**
 * Register to receive notifications when a participant's status changes (e.g: join or leave the room), or when the participant is added or removed
 * from an applicable block list (e.g: Epic block list and/or current platform's block list).
 * If the returned NotificationId is valid, you must call EOS_RTC_RemoveNotifyParticipantStatusChanged when you no longer wish to have your CompletionDelegate called.
 *
 * If you register to this notification before joining a room, you will receive a notification for every member already in the room when you join said room.
 * This allows you to know who is already in the room when you join.
 *
 * To be used effectively with a Lobby-managed RTC room, this should be registered during the EOS_Lobby_CreateLobby or EOS_Lobby_JoinLobby completion
 * callbacks when the ResultCode is EOS_Success. If this notification is registered after that point, it is possible to miss notifications for
 * already-existing room participants.
 *
 * You can use this notification to detect internal automatic RTC blocks due to block lists.
 * When a participant joins a room and while the system resolves the block list status of said participant, the participant is set to blocked and you'll receive
 * a notification with ParticipantStatus set to EOS_RTCPS_Joined and bParticipantInBlocklist set to true.
 * Once the block list status is resolved, if the player is not in any applicable block list(s), it is then unblocked and a new notification is sent with
 * ParticipantStatus set to EOS_RTCPS_Joined and bParticipantInBlocklist set to false.
 *
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when a participant changes status
 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
 *
 * @note This notification is also raised when the local user joins the room, but NOT when the local user leaves the room.
 *
 * @see EOS_INVALID_NOTIFICATIONID
 * @see EOS_RTC_AddNotifyParticipantStatusChangedOptions
 * @see EOS_RTC_OnParticipantStatusChangedCallback
 * @see EOS_RTC_RemoveNotifyParticipantStatusChanged
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyParticipantStatusChanged(EOS_HRTC Handle, const EOS_RTC_AddNotifyParticipantStatusChangedOptions* Options, void* ClientData, const EOS_RTC_OnParticipantStatusChangedCallback CompletionDelegate);

/**
 * Unregister a previously bound notification handler from receiving participant status change notifications
 *
 * @param NotificationId The Notification ID representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyParticipantStatusChanged(EOS_HRTC Handle, EOS_NotificationId NotificationId);

/**
 * Register to receive notifications of when the RTC Room is about to be created and joined.
 *
 * This gives you access to the RTC Room about to be joined, allowing for example to apply sending or receiving settings.
 *
 * If the returned NotificationId is valid, you must call EOS_RTC_RemoveNotifyRoomBeforeJoin when you no longer wish to
 * have your CompletionDelegate called.
 *
 * @param Options structure containing the parameters for the operation.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate The callback to be fired when the RTC Room is about to be created and joined
 *
 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not.
 *
 * @see EOS_INVALID_NOTIFICATIONID
 * @see EOS_RTC_AddNotifyRoomBeforeJoinOptions
 * @see EOS_RTC_OnRoomBeforeJoinCallback
 * @see EOS_RTC_RemoveNotifyRoomBeforeJoin
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyRoomBeforeJoin(EOS_HRTC Handle, const EOS_RTC_AddNotifyRoomBeforeJoinOptions* Options, void* ClientData, const EOS_RTC_OnRoomBeforeJoinCallback CompletionDelegate);

/**
 * Unregister from receiving notifications when the RTC Room is about to be created and joined.
 *
 * @param NotificationId The Notification ID representing the registered callback
 *
 * @see EOS_RTC_AddNotifyRoomBeforeJoin
 */
EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyRoomBeforeJoin(EOS_HRTC Handle, EOS_NotificationId NotificationId);

/**
 * Use this function to control settings.
 *
 * The available settings are documented as part of EOS_RTC_SetSettingOptions.
 *
 * @param Options structure containing the parameters for the operation
 * @return EOS_EResult containing the result of the operation.
 * Possible result codes:
 * - EOS_Success when the setting is successfully set
 * - EOS_NotFound when the setting is unknown
 * - EOS_InvalidParameters when the value is invalid.
 *
 * @see EOS_RTC_SetSettingOptions
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_SetSetting(EOS_HRTC Handle, const EOS_RTC_SetSettingOptions* Options);

/**
 * Use this function to control settings for the specific room.
 *
 * The available settings are documented as part of EOS_RTC_SetRoomSettingOptions.
 *
 * @param Options structure containing the parameters for the operation
 * @return EOS_EResult containing the result of the operation.
 * Possible result codes:
 * - EOS_Success when the setting is successfully set
 * - EOS_NotFound when the setting is unknown
 * - EOS_InvalidParameters when the value is invalid.
 *
 * @see EOS_RTC_SetRoomSettingOptions
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_RTC_SetRoomSetting(EOS_HRTC Handle, const EOS_RTC_SetRoomSettingOptions* Options);

/**
 * Register to receive notifications to receiving periodical statistics update.
 *
 * If the returned NotificationId is valid, you must call
 * EOS_RTC_RemoveNotifyRoomStatisticsUpdated when you no longer wish to have your CompletionDelegate called.
 *
 * @param Options structure containing the parameters for the operation
 * @param ClientData Arbitrary data that is passed back in the CompletionDelegate
 * @param CompletionDelegate The callback to be fired when a statistics updated.
 * @return Notification ID representing the registered callback if successful, an invalid NotificationId if not
 *
 * @see EOS_INVALID_NOTIFICATIONID
 * @see EOS_RTC_AddNotifyRoomStatisticsUpdatedOptions
 * @see EOS_RTC_OnRoomStatisticsUpdatedCallback
 * @see EOS_RTC_RemoveNotifyRoomStatisticsUpdated
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_RTC_AddNotifyRoomStatisticsUpdated(EOS_HRTC Handle, const EOS_RTC_AddNotifyRoomStatisticsUpdatedOptions* Options, void* ClientData, const EOS_RTC_OnRoomStatisticsUpdatedCallback CompletionDelegate);

/**
 * Unregister a previously bound notification handler from receiving periodical statistics update notifications
 *
 * @param NotificationId The Notification ID representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_RTC_RemoveNotifyRoomStatisticsUpdated(EOS_HRTC Handle, EOS_NotificationId NotificationId);
