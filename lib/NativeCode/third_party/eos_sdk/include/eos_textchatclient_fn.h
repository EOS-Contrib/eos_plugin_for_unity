// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_textchatclient_fn_types.h"

/**
 * Send a message to a conversation (chat room).
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate Pointer to a function that handles receiving the completion information.
 */
EOS_DECLARE_FUNC(void) EOS_TextChatClient_FN_SendConversationMessage(EOS_HTextChatClientFN Handle, const EOS_TextChatClient_FN_SendConversationMessageOptions* Options, void* ClientData, const EOS_TextChatClient_FN_SendConversationMessageCallback CompletionDelegate);

/**
 * Send a direct message to another user.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate Pointer to a function that handles receiving the completion information.
 */
EOS_DECLARE_FUNC(void) EOS_TextChatClient_FN_SendDirectMessage(EOS_HTextChatClientFN Handle, const EOS_TextChatClient_FN_SendDirectMessageOptions* Options, void* ClientData, const EOS_TextChatClient_FN_SendDirectMessageCallback CompletionDelegate);

/**
 * Add new listener for new conversations being created.
 * Sent only for conversations with one of local users in the initial members list.
 *
 * Due to network conditions, the notification may arrive out of order or may not arrive at all.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the NotificationFn.
 * @param NotificationFn The callback to be invoked.
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_TextChatClient_FN_AddNotifyConversationCreated(EOS_HTextChatClientFN Handle, const EOS_TextChatClient_FN_AddNotifyConversationCreatedOptions* Options, void* ClientData, const EOS_TextChatClient_FN_OnConversationCreatedCallback NotificationFn);

/**
 * Remove a EOS_TextChatClient_FN_AddNotifyConversationCreated notification listener.
 *
 * @param NotificationId The previously bound notification ID
 */
EOS_DECLARE_FUNC(void) EOS_TextChatClient_FN_RemoveNotifyConversationCreated(EOS_HTextChatClientFN Handle, EOS_NotificationId NotificationId);

/**
 * Add new listener for conversations being deleted.
 * Sent only for conversations with one of local users in the members list.
 *
 * Due to network conditions, the notification may arrive out of order or may not arrive at all.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the NotificationFn.
 * @param NotificationFn The callback to be invoked.
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_TextChatClient_FN_AddNotifyConversationDeleted(EOS_HTextChatClientFN Handle, const EOS_TextChatClient_FN_AddNotifyConversationDeletedOptions* Options, void* ClientData, const EOS_TextChatClient_FN_OnConversationDeletedCallback NotificationFn);

/**
 * Remove a EOS_TextChatClient_FN_AddNotifyConversationDeleted notification listener.
 *
 * @param NotificationId The previously bound notification ID
 */
EOS_DECLARE_FUNC(void) EOS_TextChatClient_FN_RemoveNotifyConversationDeleted(EOS_HTextChatClientFN Handle, EOS_NotificationId NotificationId);

/**
 * Add new listener for new members being added to conversations.
 * Sent only for conversations with one of local users in the members list.
 *
 * Due to network conditions, the notification may arrive out of order or may not arrive at all.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the NotificationFn.
 * @param NotificationFn The callback to be invoked.
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_TextChatClient_FN_AddNotifyMembersJoin(EOS_HTextChatClientFN Handle, const EOS_TextChatClient_FN_AddNotifyMembersJoinOptions* Options, void* ClientData, const EOS_TextChatClient_FN_OnMembersJoinCallback NotificationFn);

/**
 * Remove a EOS_TextChatClient_FN_AddNotifyMembersJoin notification listener.
 *
 * @param NotificationId The previously bound notification ID
 */
EOS_DECLARE_FUNC(void) EOS_TextChatClient_FN_RemoveNotifyMembersJoin(EOS_HTextChatClientFN Handle, EOS_NotificationId NotificationId);

/**
 * Add new listener for members being removed from conversations.
 * Sent only for conversations with one of local users in the members list.
 *
 * Due to network conditions, the notification may arrive out of order or may not arrive at all.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the NotificationFn.
 * @param NotificationFn The callback to be invoked.
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_TextChatClient_FN_AddNotifyMembersLeft(EOS_HTextChatClientFN Handle, const EOS_TextChatClient_FN_AddNotifyMembersLeftOptions* Options, void* ClientData, const EOS_TextChatClient_FN_OnMembersLeftCallback NotificationFn);

/**
 * Remove a EOS_TextChatClient_FN_AddNotifyMembersLeft notification listener.
 *
 * @param NotificationId The previously bound notification ID
 */
EOS_DECLARE_FUNC(void) EOS_TextChatClient_FN_RemoveNotifyMembersLeft(EOS_HTextChatClientFN Handle, EOS_NotificationId NotificationId);

/**
 * Add new listener for new direct messages.
 * Sent only for messages where recipient is one of local users.
 *
 * Due to network conditions, the notification may arrive out of order or may not arrive at all.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the NotificationFn.
 * @param NotificationFn The callback to be invoked.
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_TextChatClient_FN_AddNotifyNewDirectMessage(EOS_HTextChatClientFN Handle, const EOS_TextChatClient_FN_AddNotifyNewDirectMessageOptions* Options, void* ClientData, const EOS_TextChatClient_FN_OnNewDirectMessageCallback NotificationFn);

/**
 * Remove a EOS_TextChatClient_FN_AddNotifyNewDirectMessage notification listener.
 *
 * @param NotificationId The previously bound notification ID
 */
EOS_DECLARE_FUNC(void) EOS_TextChatClient_FN_RemoveNotifyNewDirectMessage(EOS_HTextChatClientFN Handle, EOS_NotificationId NotificationId);

/**
 * Add new listener for new conversation messages.
 * Sent only for conversations with one of local users in the members list.
 *
 * Due to network conditions, the notification may arrive out of order or may not arrive at all.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the NotificationFn.
 * @param NotificationFn The callback to be invoked.
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_TextChatClient_FN_AddNotifyNewConversationMessage(EOS_HTextChatClientFN Handle, const EOS_TextChatClient_FN_AddNotifyNewConversationMessageOptions* Options, void* ClientData, const EOS_TextChatClient_FN_OnNewConversationMessageCallback NotificationFn);

/**
 * Remove a EOS_TextChatClient_FN_AddNotifyNewConversationMessage notification listener.
 *
 * @param NotificationId The previously bound notification ID
 */
EOS_DECLARE_FUNC(void) EOS_TextChatClient_FN_RemoveNotifyNewConversationMessage(EOS_HTextChatClientFN Handle, EOS_NotificationId NotificationId);
