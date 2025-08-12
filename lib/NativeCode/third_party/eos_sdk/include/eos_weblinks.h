// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_weblinks_types.h"

/**
 * The Weblinks Interface is used to handle invites originating from a weblink and routing them to the correct Session, Lobby, or Game specific invite handler.
 * All Weblinks Interface calls take a handle of type EOS_HWeblinks as the first parameter.
 * This handle can be retrieved from an EOS_HPlatform handle by using the EOS_Platform_GetWeblinksInterface function.
 *
 * @see EOS_Platform_GetWeblinksInterface
 */

/**
 * Register to receive notifications when a weblink invite is recieved.
 * @note If the returned NotificationId is valid, you must call EOS_Weblinks_RemoveNotifyWeblinkInviteReceived when you no longer wish to have your NotificationHandler called.
 *
 * @see EOS_Weblinks_RemoveNotifyWeblinkInviteReceived
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the NotificationFn.
 * @param NotificationFn A callback that is fired when a a notification is received.
 *
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Weblinks_AddNotifyWeblinkInviteReceived(EOS_HWeblinks Handle, const EOS_Weblinks_AddNotifyWeblinkInviteReceivedOptions* Options, void* ClientData, const EOS_Weblinks_OnWeblinkInviteReceivedCallback NotificationFn);

/**
 * Unregister from receiving notifications when a weblink invite is Received.
 *
 * @param NotificationId  A bound notification ID representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_Weblinks_RemoveNotifyWeblinkInviteReceived(EOS_HWeblinks Handle, EOS_NotificationId NotificationId);

/**
 * Create a weblink session.
 *
 * @param Options Required fields for the creation of a weblink session
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate
 * @param CompletionDelegate A callback that is fired when the create operation completes, either successfully or in error
 *
 * @return EOS_Success if the creation completes successfully
 *         EOS_InvalidParameters if any of the options are incorrect
 */
EOS_DECLARE_FUNC(void) EOS_Weblinks_CreateWeblinkSession(EOS_HWeblinks Handle, const EOS_Weblinks_CreateWeblinkSessionOptions* Options, void* ClientData, const EOS_Weblinks_OnCreateWeblinkSessionCallback CompletionDelegate);
