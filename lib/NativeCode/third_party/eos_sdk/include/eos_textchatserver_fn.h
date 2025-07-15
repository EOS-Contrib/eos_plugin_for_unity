// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_textchatserver_fn_types.h"

/**
 * Create a new conversation (chat room).
 * Initial members list is optional.
 *
 * This function requires TextChat Admin permissions granted only to dedicated servers.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate Pointer to a function that handles receiving the completion information.
 */
EOS_DECLARE_FUNC(void) EOS_TextChatServer_FN_CreateConversation(EOS_HTextChatServerFN Handle, const EOS_TextChatServer_FN_CreateConversationOptions* Options, void* ClientData, const EOS_TextChatServer_FN_CreateConversationCallback CompletionDelegate);

/**
 * Delete conversation.
 * The conversation will not be functional after this point.
 * Conversations that weren't deleted explicitly will be deleted by the backend automatically after some time.
 *
 * This function requires TextChat Admin permissions granted only to dedicated servers.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate Pointer to a function that handles receiving the completion information.
 */
EOS_DECLARE_FUNC(void) EOS_TextChatServer_FN_DeleteConversation(EOS_HTextChatServerFN Handle, const EOS_TextChatServer_FN_DeleteConversationOptions* Options, void* ClientData, const EOS_TextChatServer_FN_DeleteConversationCallback CompletionDelegate);

/**
 * Add new members to conversation.
 *
 * This function requires TextChat Admin permissions granted only to dedicated servers.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate Pointer to a function that handles receiving the completion information.
 */
EOS_DECLARE_FUNC(void) EOS_TextChatServer_FN_AddConversationMembers(EOS_HTextChatServerFN Handle, const EOS_TextChatServer_FN_AddConversationMembersOptions* Options, void* ClientData, const EOS_TextChatServer_FN_AddConversationMembersCallback CompletionDelegate);

/**
 * Remove members from conversation.
 *
 * This function requires TextChat Admin permissions granted only to dedicated servers.
 *
 * @param Options Input parameters.
 * @param ClientData Arbitrary context data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate Pointer to a function that handles receiving the completion information.
 */
EOS_DECLARE_FUNC(void) EOS_TextChatServer_FN_RemoveConversationMembers(EOS_HTextChatServerFN Handle, const EOS_TextChatServer_FN_RemoveConversationMembersOptions* Options, void* ClientData, const EOS_TextChatServer_FN_RemoveConversationMembersCallback CompletionDelegate);
