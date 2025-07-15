// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_TextChatClientHandleFN* EOS_HTextChatClientFN;


/** The most recent version of the EOS_TextChatClient_FN_SendConversationMessageOptions struct. */
#define EOS_TEXTCHATCLIENT_FN_SENDCONVERSATIONMESSAGEOPTIONS_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatClient_FN_SendConversationMessage function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_SendConversationMessageOptions, (
	/** API Version: Set this to EOS_TEXTCHATCLIENT_FN_SENDCONVERSATIONMESSAGEOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** A reserved field that should always be nulled. */
	void* Reserved;
	/** The Epic Account ID of the local authenticated user. */
	EOS_EpicAccountId LocalUserId;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
	/** The TextChat Message body. */
	const char* MessageBody;
	/** Message Metadata map keys. Number of keys must be equal to MessageMetadataCount. */
	const char** MessageMetadataKeys;
	/** Message Metadata map values. Number of items must be equal to MessageMetadataCount. */
	const char** MessageMetadataValues;
	/** The number of Message Metadata map elements. */
	uint32_t MessageMetadataCount;
	/**
	 * An optional array of Conversation Members which are allowed to receive this message.
	 * In a team chat typically this would include all known team members, except for those that the local user has blocked.
	 * The AllowedRecipients list is ignored if the ProhibitedRecipients list is not empty.
	 * If AllowedRecipients list is empty, then all conversation members can receive this message.
	 */
	EOS_EpicAccountId* AllowedRecipients;
	/** The number of AllowedRecipients array elements. */
	uint32_t AllowedRecipientsCount;
	/**
	 * An array of Conversation Members which are prohibited from receiving this message.
	 * In a team chat typically this would include all known team members that the local user has blocked.
	 * If ProhibitedRecipients list is empty, then the AllowedRecipients list is used if present.
	 * If ProhibitedRecipients list is not empty, then the AllowedRecipients list is ignored.
	 *
	 * Using ProhibitedRecipients is unreliable and can result in message being sent to a prohitibited recipient.
	 * - If the prohitibited recipient joins conversation on backend around the same time this message is sent,
	 *   the member join notification may not arrive on client yet due to laws of time/physics,
	 *   and the local user may not put the prohibited recipient on the list, resulting in a message being received.
	 * - In some countries regulators may find this unacceptable.
	 * TODO: Remove?
	 */
	EOS_EpicAccountId* ProhibitedRecipients;
	/** The number of ProhibitedRecipients array elements. */
	uint32_t ProhibitedRecipientsCount;
));

/**
 * Async result of the EOS_TextChatClient_FN_SendConversationMessage function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_SendConversationMessageCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_TextChatClient_FN_SendConversationMessage. */
	void* ClientData;
	/** The Epic Account ID of the local authenticated user. */
	EOS_EpicAccountId LocalUserId;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
));

/**
 * Function prototype definition for callbacks passed to EOS_TextChatClient_FN_SendConversationMessage
 * @param Data A EOS_TextChatClient_FN_SendConversationMessageCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_TextChatClient_FN_SendConversationMessageCallback, const EOS_TextChatClient_FN_SendConversationMessageCallbackInfo* Data);



/** The most recent version of the EOS_TextChatClient_FN_SendDirectMessageOptions struct. */
#define EOS_TEXTCHATCLIENT_FN_SENDDIRECTMESSAGEOPTIONS_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatClient_FN_SendDirectMessage function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_SendDirectMessageOptions, (
	/** API Version: Set this to EOS_TEXTCHATCLIENT_FN_SENDDIRECTMESSAGEOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** A reserved field that should always be nulled. */
	void* Reserved;
	/** The Epic Account ID of the sender. */
	EOS_EpicAccountId LocalUserId;
	/** The Epic Account ID of the recipient. */
	EOS_EpicAccountId TargetUserId;
	/** The TextChat Message body. */
	const char* MessageBody;
	/** Message Metadata map keys. Number of keys must be equal to MessageMetadataCount. */
	const char** MessageMetadataKeys;
	/** Message Metadata map values. Number of items must be equal to MessageMetadataCount. */
	const char** MessageMetadataValues;
	/** The number of Message Metadata map elements. */
	uint32_t MessageMetadataCount;
));

/**
 * Async result of the EOS_TextChatClient_FN_SendDirectMessage function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_SendDirectMessageCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_TextChatClient_FN_SendDirectMessage. */
	void* ClientData;
	/** The Epic Account ID of the local authenticated user. */
	EOS_EpicAccountId LocalUserId;
	/** The Epic Account ID of the target user. */
	EOS_EpicAccountId TargetUserId;
));

/**
 * Function prototype definition for callbacks passed to EOS_TextChatClient_FN_SendDirectMessage
 * @param Data A EOS_TextChatClient_FN_SendDirectMessageCallbackInfo containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_TextChatClient_FN_SendDirectMessageCallback, const EOS_TextChatClient_FN_SendDirectMessageCallbackInfo* Data);


/** The most recent version of the EOS_TextChatClient_FN_AddNotifyConversationCreatedOptions struct. */
#define EOS_TEXTCHATCLIENT_FN_ADDNOTIFYCONVERSATIONCREATED_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatClient_FN_AddNotifyConversationCreated function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_AddNotifyConversationCreatedOptions, (
	/** API Version: Set this to EOS_TEXTCHATCLIENT_FN_ADDNOTIFYCONVERSATIONCREATED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Notification callback data of the EOS_TextChatClient_FN_AddNotifyConversationCreated function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_OnConversationCreatedCallbackInfo, (
	/** Context that was passed into EOS_TextChatClient_FN_AddNotifyConversationCreated */
	void* ClientData;
	/** The Epic Account ID of the notification recipient. */
	EOS_EpicAccountId LocalUserId;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
	/** The TextChat Conversation Type. TODO: Consider enumeration after API stabilizes. Currently one of: "game", "team", "party". */
	const char* ConversationType;
	/** An array of User ID's present in Conversation. */
	EOS_EpicAccountId* Members;
	/** The number of Members array elements. */
	uint32_t MembersCount;
	/** Conversation Metadata map keys. Number of items is equal to ConversationMetadataCount. */
	const char** ConversationMetadataKeys;
	/** Conversation Metadata map values. Number of items is equal to ConversationMetadataCount. */
	const char** ConversationMetadataValues;
	/** The number of Conversation Metadata map elements. */
	uint32_t ConversationMetadataCount;
));

/**
 * Function prototype definition for notification callbacks passed to EOS_TextChatClient_FN_AddNotifyConversationCreated
 * @param Data A EOS_TextChatClient_FN_OnConversationCreatedCallbackInfo containing the notification data
 */
EOS_DECLARE_CALLBACK(EOS_TextChatClient_FN_OnConversationCreatedCallback, const EOS_TextChatClient_FN_OnConversationCreatedCallbackInfo* Data);


/** The most recent version of the EOS_TextChatClient_FN_AddNotifyConversationDeletedOptions struct. */
#define EOS_TEXTCHATCLIENT_FN_ADDNOTIFYCONVERSATIONDELETED_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatClient_FN_AddNotifyConversationDeleted function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_AddNotifyConversationDeletedOptions, (
	/** API Version: Set this to EOS_TEXTCHATCLIENT_FN_ADDNOTIFYCONVERSATIONDELETED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Notification callback data of the EOS_TextChatClient_FN_AddNotifyConversationDeleted function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_OnConversationDeletedCallbackInfo, (
	/** Context that was passed into EOS_TextChatClient_FN_AddNotifyConversationDeleted */
	void* ClientData;
	/** The Epic Account ID of the notification recipient. */
	EOS_EpicAccountId LocalUserId;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
));

/**
 * Function prototype definition for notification callbacks passed to EOS_TextChatClient_FN_AddNotifyConversationDeleted
 * @param Data A EOS_TextChatClient_FN_OnConversationDeletedCallbackInfo containing the notification data
 */
EOS_DECLARE_CALLBACK(EOS_TextChatClient_FN_OnConversationDeletedCallback, const EOS_TextChatClient_FN_OnConversationDeletedCallbackInfo* Data);


/** The most recent version of the EOS_TextChatClient_FN_AddNotifyMembersJoinOptions struct. */
#define EOS_TEXTCHATCLIENT_FN_ADDNOTIFYMEMBERSJOIN_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatClient_FN_AddNotifyMembersJoin function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_AddNotifyMembersJoinOptions, (
	/** API Version: Set this to EOS_TEXTCHATCLIENT_FN_ADDNOTIFYMEMBERSJOIN_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Notification callback data of the EOS_TextChatClient_FN_AddNotifyMembersJoin function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_OnMembersJoinCallbackInfo, (
	/** Context that was passed into EOS_TextChatClient_FN_AddNotifyMembersJoin */
	void* ClientData;
	/** The Epic Account ID of the notification recipient. */
	EOS_EpicAccountId LocalUserId;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
	/** An array of User ID's added to Conversation. */
	EOS_EpicAccountId* Members;
	/** The number of Members array elements. */
	uint32_t MembersCount;
));

/**
 * Function prototype definition for notification callbacks passed to EOS_TextChatClient_FN_AddNotifyMembersJoin
 * @param Data A EOS_TextChatClient_FN_OnMembersJoinCallbackInfo containing the notification data
 */
EOS_DECLARE_CALLBACK(EOS_TextChatClient_FN_OnMembersJoinCallback, const EOS_TextChatClient_FN_OnMembersJoinCallbackInfo* Data);


/** The most recent version of the EOS_TextChatClient_FN_AddNotifyMembersLeftOptions struct. */
#define EOS_TEXTCHATCLIENT_FN_ADDNOTIFYMEMBERSLEFT_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatClient_FN_AddNotifyMembersLeft function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_AddNotifyMembersLeftOptions, (
	/** API Version: Set this to EOS_TEXTCHATCLIENT_FN_ADDNOTIFYMEMBERSLEFT_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Notification callback data of the EOS_TextChatClient_FN_AddNotifyMembersLeft function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_OnMembersLeftCallbackInfo, (
	/** Context that was passed into EOS_TextChatClient_FN_AddNotifyMembersLeft */
	void* ClientData;
	/** The Epic Account ID of the notification recipient. */
	EOS_EpicAccountId LocalUserId;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
	/** An array of User ID's removed from Conversation. */
	EOS_EpicAccountId* Members;
	/** The number of Members array elements. */
	uint32_t MembersCount;
));

/**
 * Function prototype definition for notification callbacks passed to EOS_TextChatClient_FN_AddNotifyMembersLeft
 * @param Data A EOS_TextChatClient_FN_OnMembersLeftCallbackInfo containing the notification data
 */
EOS_DECLARE_CALLBACK(EOS_TextChatClient_FN_OnMembersLeftCallback, const EOS_TextChatClient_FN_OnMembersLeftCallbackInfo* Data);


/** The most recent version of the EOS_TextChatClient_FN_AddNotifyNewDirectMessageOptions struct. */
#define EOS_TEXTCHATCLIENT_FN_ADDNOTIFYNEWDIRECTMESSAGE_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatClient_FN_AddNotifyNewDirectMessage function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_AddNotifyNewDirectMessageOptions, (
	/** API Version: Set this to EOS_TEXTCHATCLIENT_FN_ADDNOTIFYNEWDIRECTMESSAGE_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Notification callback data of the EOS_TextChatClient_FN_AddNotifyNewDirectMessage function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_OnNewDirectMessageCallbackInfo, (
	/** Context that was passed into EOS_TextChatClient_FN_AddNotifyNewDirectMessage */
	void* ClientData;
	/** The Epic Account ID of the notification recipient. */
	EOS_EpicAccountId LocalUserId;
	/** The Epic Account ID of the message sender. */
	EOS_EpicAccountId SenderId;
	/** The time at which message was sent. Messages can arrive out of order. */
	int64_t Time;
	/** The TextChat Message body. */
	const char* MessageBody;
	/** Message Metadata map keys. Number of keys is equal to MessageMetadataCount. */
	const char** MessageMetadataKeys;
	/** Message Metadata map values. Number of items is equal to MessageMetadataCount. */
	const char** MessageMetadataValues;
	/** The number of Message Metadata map elements. */
	uint32_t MessageMetadataCount;
));

/**
 * Function prototype definition for notification callbacks passed to EOS_TextChatClient_FN_AddNotifyNewDirectMessage
 * @param Data A EOS_TextChatClient_FN_OnNewDirectMessageCallbackInfo containing the notification data
 */
EOS_DECLARE_CALLBACK(EOS_TextChatClient_FN_OnNewDirectMessageCallback, const EOS_TextChatClient_FN_OnNewDirectMessageCallbackInfo* Data);


/** The most recent version of the EOS_TextChatClient_FN_AddNotifyNewConversationMessageOptions struct. */
#define EOS_TEXTCHATCLIENT_FN_ADDNOTIFYNEWCONVERSATIONMESSAGE_API_LATEST 1

/**
 * Input parameters for the EOS_TextChatClient_FN_AddNotifyNewConversationMessage function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_AddNotifyNewConversationMessageOptions, (
	/** API Version: Set this to EOS_TEXTCHATCLIENT_FN_ADDNOTIFYNEWCONVERSATIONMESSAGE_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Notification callback data of the EOS_TextChatClient_FN_AddNotifyNewConversationMessage function.
 */
EOS_STRUCT(EOS_TextChatClient_FN_OnNewConversationMessageCallbackInfo, (
	/** Context that was passed into EOS_TextChatClient_FN_AddNotifyNewConversationMessage */
	void* ClientData;
	/** The Epic Account ID of the notification recipient. */
	EOS_EpicAccountId LocalUserId;
	/** The Epic Account ID of the message sender. */
	EOS_EpicAccountId SenderId;
	/** The time at which message was sent. Messages can arrive out of order. */
	int64_t Time;
	/** The TextChat Message body. */
	const char* MessageBody;
	/** The TextChat Conversation ID. Each chat room created has unique ID. */
	const char* ConversationId;
	/** The TextChat Conversation Type. TODO: Consider enumeration after API stabilizes. Currently one of: "game", "team", "party". */
	const char* ConversationType;
	/** Conversation Metadata map keys. Number of items is equal to ConversationMetadataCount. */
	const char** ConversationMetadataKeys;
	/** Conversation Metadata map values. Number of items is equal to ConversationMetadataCount. */
	const char** ConversationMetadataValues;
	/** The number of Conversation Metadata map elements. */
	uint32_t ConversationMetadataCount;
	/** Message Metadata map keys. Number of keys is equal to MessageMetadataCount. */
	const char** MessageMetadataKeys;
	/** Message Metadata map values. Number of items is equal to MessageMetadataCount. */
	const char** MessageMetadataValues;
	/** The number of Message Metadata map elements. */
	uint32_t MessageMetadataCount;
));

/**
 * Function prototype definition for notification callbacks passed to EOS_TextChatClient_FN_AddNotifyNewConversationMessage
 * @param Data A EOS_TextChatClient_FN_OnNewConversationMessageCallbackInfo containing the notification data
 */
EOS_DECLARE_CALLBACK(EOS_TextChatClient_FN_OnNewConversationMessageCallback, const EOS_TextChatClient_FN_OnNewConversationMessageCallbackInfo* Data);


#pragma pack(pop)
