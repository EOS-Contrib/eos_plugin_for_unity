// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_TextChatServerHandleFN* EOS_HTextChatServerFN;



/** The most recent version of the EOS_TextChatServer_FN_CreateConversationOptions struct. */
#define EOS_TEXTCHATSERVER_FN_CREATECONVERSATIONOPTIONS_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatServer_FN_CreateConversation function.
 */
EOS_STRUCT(EOS_TextChatServer_FN_CreateConversationOptions, (
	/** API Version: Set this to EOS_TEXTCHATSERVER_FN_CREATECONVERSATIONOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** A reserved field that should always be nulled. */
	void* Reserved;
	/** The TextChat Conversation human readable title. TODO: TBD, currently unused and not present in Epic Connect notifications. */
	const char* Title;
	/** The TextChat Conversation Type. TODO: Consider enumeration after API stabilizes. Currently one of: "game", "team", "party". */
	const char* ConversationType;
	/** An array of User ID's to add to conversation. */
	EOS_EpicAccountId* Members;
	/** The number of Members array elements. */
	uint32_t MembersCount;
	/** Conversation Metadata map keys. Number of items must be equal to ConversationMetadataCount. */
	const char** ConversationMetadataKeys;
	/** Conversation Metadata map values. Number of items must be equal to ConversationMetadataCount. */
	const char** ConversationMetadataValues;
	/** The number of Conversation Metadata map elements. */
	uint32_t ConversationMetadataCount;
));

/**
 * Async result of the EOS_TextChatServer_FN_CreateConversation function.
 */
EOS_STRUCT(EOS_TextChatServer_FN_CreateConversationCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_TextChatServer_FN_CreateConversation. */
	void* ClientData;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
));

/**
 * Function prototype definition for callbacks passed to EOS_TextChatServer_FN_CreateConversation
 * @param Data A EOS_TextChatServer_FN_CreateConversationCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_TextChatServer_FN_CreateConversationCallback, const EOS_TextChatServer_FN_CreateConversationCallbackInfo* Data);



/** The most recent version of the EOS_TextChatServer_FN_DeleteConversationOptions struct. */
#define EOS_TEXTCHATSERVER_FN_DELETECONVERSATIONOPTIONS_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatServer_FN_DeleteConversation function.
 */
EOS_STRUCT(EOS_TextChatServer_FN_DeleteConversationOptions, (
	/** API Version: Set this to EOS_TEXTCHATSERVER_FN_DELETECONVERSATIONOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** A reserved field that should always be nulled. */
	void* Reserved;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
));

/**
 * Async result of the EOS_TextChatServer_FN_DeleteConversation function.
 */
EOS_STRUCT(EOS_TextChatServer_FN_DeleteConversationCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_TextChatServer_FN_DeleteConversation. */
	void* ClientData;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
));

/**
 * Function prototype definition for callbacks passed to EOS_TextChatServer_FN_DeleteConversation
 * @param Data A EOS_TextChatServer_FN_DeleteConversationCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_TextChatServer_FN_DeleteConversationCallback, const EOS_TextChatServer_FN_DeleteConversationCallbackInfo* Data);



/** The most recent version of the EOS_TextChatServer_FN_AddConversationMembersOptions struct. */
#define EOS_TEXTCHATSERVER_FN_ADDCONVERSATIONMEMBERSOPTIONS_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatServer_FN_AddConversationMembers function.
 */
EOS_STRUCT(EOS_TextChatServer_FN_AddConversationMembersOptions, (
	/** API Version: Set this to EOS_TEXTCHATSERVER_FN_ADDCONVERSATIONMEMBERSOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** A reserved field that should always be nulled. */
	void* Reserved;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
	/** An array of User ID's to add to conversation. */
	EOS_EpicAccountId* Members;
	/** The number of Members array elements. */
	uint32_t MembersCount;
));

/**
 * Async result of the EOS_TextChatServer_FN_AddConversationMembers function.
 */
EOS_STRUCT(EOS_TextChatServer_FN_AddConversationMembersCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_TextChatServer_FN_AddConversationMembers. */
	void* ClientData;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
	/** An array of User ID's to add to conversation. */
	EOS_EpicAccountId* Members;
	/** The number of Members array elements. */
	uint32_t MembersCount;
));

/**
 * Function prototype definition for callbacks passed to EOS_TextChatServer_FN_AddConversationMembers
 * @param Data A EOS_TextChatServer_FN_AddConversationMembersCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_TextChatServer_FN_AddConversationMembersCallback, const EOS_TextChatServer_FN_AddConversationMembersCallbackInfo* Data);



/** The most recent version of the EOS_TextChatServer_FN_RemoveConversationMembersOptions struct. */
#define EOS_TEXTCHATSERVER_FN_REMOVECONVERSATIONMEMBERSOPTIONS_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatServer_FN_RemoveConversationMembers function.
 */
EOS_STRUCT(EOS_TextChatServer_FN_RemoveConversationMembersOptions, (
	/** API Version: Set this to EOS_TEXTCHATSERVER_FN_REMOVECONVERSATIONMEMBERSOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** A reserved field that should always be nulled. */
	void* Reserved;
	/** The TextChat conversation ID. Each chat room created has unique id. */
	const char* ConversationId;
	/** An array of User ID's to remove from conversation. */
	EOS_EpicAccountId* Members;
	/** The number of Members array elements. */
	uint32_t MembersCount;
));

/**
 * Async result of the EOS_TextChatServer_FN_RemoveConversationMembers function.
 */
EOS_STRUCT(EOS_TextChatServer_FN_RemoveConversationMembersCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_TextChatServer_FN_RemoveConversationMembers. */
	void* ClientData;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
	/** An array of User ID's to add to conversation. */
	EOS_EpicAccountId* Members;
	/** The number of Members array elements. */
	uint32_t MembersCount;
));

/**
 * Function prototype definition for callbacks passed to EOS_TextChatServer_FN_RemoveConversationMembers
 * @param Data A EOS_TextChatServer_FN_RemoveConversationMembersCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_TextChatServer_FN_RemoveConversationMembersCallback, const EOS_TextChatServer_FN_RemoveConversationMembersCallbackInfo* Data);


#pragma pack(pop)
