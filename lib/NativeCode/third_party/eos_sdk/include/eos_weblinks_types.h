// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"
#include "eos_ui_types.h"

#pragma pack(push, 8)

/** Handle to the Weblinks interface */
EOS_EXTERN_C typedef struct EOS_WeblinksHandle* EOS_HWeblinks;

/** A type representing the Unique Identifier used by a Weblink session as a null terminated UTF8 string */
EOS_EXTERN_C typedef const char* EOS_WeblinkId;

/** Minimum number of characters allowed in the weblink session id  override */
#define EOS_WEBLINKS_MIN_WEBLINKSESSIONID_LENGTH 4
/** Maximum number of characters allowed in the weblink session id override */
#define EOS_WEBLINKS_MAX_WEBLINKSESSIONID_LENGTH 60
/** Minimum number of characters allowed in the weblink session id  override */
#define EOS_WEBLINKS_MIN_LINKEDSESSIONID_LENGTH 4
/** Maximum number of characters allowed in the weblink session id override */
#define EOS_WEBLINKS_MAX_LINKEDSESSIONID_LENGTH 60

/** The type of session the weblink is associated with */
EOS_ENUM(EOS_EWeblinksInviteType,
	/** Weblink of unknown type */
	EOS_WIT_UNKNOWN = 0,
	/** Weblink links to a lobby */
	EOS_WIT_LOBBY = 1,
	/** Weblink links to a session */
	EOS_WIT_SESSION = 2,
	/** Weblink links to an application defined type */
	EOS_WIT_CUSTOM = 3
);

/** The most recent version of the EOS_Weblinks_AddNotifyWeblinkInviteReceived API. */
#define EOS_WEBLINKS_ADDNOTIFYWEBLINKINVITERECEIVED_API_LATEST 1

EOS_STRUCT(EOS_Weblinks_AddNotifyWeblinkInviteReceivedOptions, (
	/** API Version: Set this to EOS_WEBLINKS_ADDNOTIFYWEBLINKINVITERECEIVED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Weblinks_OnWeblinkInviteReceivedCallback Function.
 */
EOS_STRUCT(EOS_Weblinks_OnWeblinkInviteReceivedCallbackInfo, (
	/** Context that was passed into EOS_Weblinks_OnWeblinkInviteReceivedCallbackInfo */
	void* ClientData;
	/** The Product User ID of the local user invited to join Weblink session */
	EOS_ProductUserId LocalUserId;
	/** Type of session represented by weblink invite */
	EOS_EWeblinksInviteType InviteType;
	/** Payload associated with weblink as a null terminated UTF8 string; the payload is dependent on InviteType and represents a Lobby Id, Session Id or application defined identifier */
	const char* LinkedSessionId;
));

/**
 * Function prototype definition for notifications that comes from EOS_Weblinks_AddNotifyWeblinkInviteReceived
 *
 * @param Data A EOS_Weblinks_OnWeblinkInviteReceivedCallback containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Weblinks_OnWeblinkInviteReceivedCallback, const EOS_Weblinks_OnWeblinkInviteReceivedCallbackInfo* Data);

/** The most recent version of the EOS_Weblinks_CreateWeblinkSession API. */
#define EOS_WEBLINKS_CREATEWEBLINKSESSION_API_LATEST 1

EOS_STRUCT(EOS_Weblinks_CreateWeblinkSessionOptions, (
	/** API Version: Set this to EOS_WEBLINKS_CREATEWEBLINKSESSION_API_LATEST */
	int32_t ApiVersion;
	/** The Product User ID of the local user creating the weblink session */
	EOS_ProductUserId LocalUserId;
	/** Weblinks Session ID as a null terminated UTF8 string */
	EOS_WeblinkId WeblinkSessionId;
	/** The session ID associated with the weblink as a null terminated UTF8 string; will vary based on SessionType */
	const char* LinkedSessionId;
	/** Type of session represented by weblink */
	EOS_EWeblinksInviteType LinkedSessionType;
	/** The maximum time in seconds the session is expected to exist. Must be greater than 60. Sessions existing for longer are subject to automatic cleanup.*/
	uint32_t SessionTtlSeconds;
	/** Creation time for the session as a POSIX timestamp in milliseconds .*/
	int64_t CreationTimeMS;
));

/**
 * Output parameters for the EOS_Weblinks_CreateWeblinkSession Function.
 */
EOS_STRUCT(EOS_Weblinks_CreateWeblinkSessionCallbackInfo, (
	/** Context that was passed into EOS_Weblinks_CreateWeblinkSession */
	void* ClientData;
	/** Weblinks Session ID */
	EOS_WeblinkId WeblinkSessionId;
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
));

/**
 * Function prototype definition for notifications that comes from EOS_Weblinks_CreateWeblinkSession
 *
 * @param Data A EOS_Weblinks_OnCreateWeblinkSessionCallback containing the output information and result
 */
EOS_DECLARE_CALLBACK(EOS_Weblinks_OnCreateWeblinkSessionCallback, const EOS_Weblinks_CreateWeblinkSessionCallbackInfo* Data);
#pragma pack(pop)
