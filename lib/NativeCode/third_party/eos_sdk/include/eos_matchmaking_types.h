// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

/** Handle to the matchmaking interface. */
EOS_EXTERN_C typedef struct EOS_MatchmakingHandle* EOS_HMatchmaking;
/** Handle to the calls related to a Matchmaking Constraint. */
EOS_EXTERN_C typedef struct EOS_MatchmakingConstraintHandle* EOS_HMatchmakingConstraint;
/** Alias for the Matchmaking Session Id type. */
EOS_EXTERN_C typedef const char* EOS_Matchmaking_SessionId;
/** Alias for the Matchmaking Ticket Id type. */
EOS_EXTERN_C typedef const char* EOS_Matchmaking_TicketId;

/** Max length of the Matchmaking Attribute Key. */
#define EOS_MATCHMAKING_ATTRIBUTEKEY_MAX_LENGTH 1024

/** Max length of the Matchmaking Attribute Value. */
#define EOS_MATCHMAKING_ATTRIBUTEVALUE_MAX_LENGTH 1024

/** Max length of the Matchmaking session attributes Utf8 JSON string. */
#define EOS_MATCHMAKING_SESSION_ATTRIBUTES_MAX_LENGTH 1024000

/** Max length of the Matchmaking match ID. */
#define EOS_MATCHMAKING_MATCH_ID_MAX_LENGTH 512

/** Max length of the Match drop message. */
#define EOS_MATCHMAKING_MATCH_DROP_MESSAGE_MAX_LENGTH 1024

/** Max length of the Matchmaking ticket ID. */
#define EOS_MATCHMAKING_TICKET_ID_MAX_LENGTH 512

/** Max size of the party members list. */
#define EOS_MATCHMAKING_PARTY_MEMBERS_MAX_SIZE 200

/** Max length of the Matchmaking player attributes Utf8 JSON string. */
#define EOS_MATCHMAKING_PLAYER_ATTRIBUTES_MAX_LENGTH 1024000

/**
 * Release the memory associated with a Matchmaking Constraint handle. This must be called on data retrieved from EOS_Matchmaking_CreateConstraint.
 * @Note: If a constraint has sub constraints, the memory for the sub constraints will also be released. In this case, calling EOS_MatchmakingConstraint_Release
 * on the released sub constraints will result in a no op.
 *
 * @param ConstraintHandle - The matchmaking constraint handle to release.
 *
 * @see EOS_Matchmaking_CreateConstraint
 */
EOS_DECLARE_FUNC(void) EOS_MatchmakingConstraint_Release(EOS_HMatchmakingConstraint ConstraintHandle);

/**
 * Supported types of data that can be stored with inside an attribute used by matchmaking.
 *
 * @see EOS_MatchmakingConstraint_AddConditionOptions
 */
EOS_ENUM(EOS_EMatchmakingAttributeType,
	/** Boolean value (true/false) */
	EOS_MAT_BOOLEAN = 0,
	/** 64 bit integers */
	EOS_MAT_INT64 = 1,
	/** Double/floating point precision */
	EOS_MAT_DOUBLE = 2,
	/** UTF8 Strings */
	EOS_MAT_STRING = 3,
	/** 64 bit integer Array type */
	EOS_MAT_INT64_ARRAY = 4,
	/** Double Array type */
	EOS_MAT_DOUBLE_ARRAY = 5,
	/** UTF8 String Array type */
	EOS_MAT_STRING_ARRAY = 6
);

/** The most recent version of the EOS_Matchmaking_AttributeData struct. */
#define EOS_MATCHMAKING_ATTRIBUTEDATA_API_LATEST 1

/**
 * Contains information about the matchmaking constraint data.
 */
EOS_STRUCT(EOS_Matchmaking_AttributeData, (
	/** API Version: Set this to EOS_MATCHMAKING_ATTRIBUTEDATA_API_LATEST. */
	int32_t ApiVersion;
	/** The operand value for this condition. */
	union
	{
		/** Stored as an 8 byte integer */
		int64_t AsInt64;
		/** Stored as a double precision floating point */
		double AsDouble;
		/** Stored as a boolean */
		EOS_Bool AsBool;
		/** Stored as a null terminated UTF8 string. Should not be nullptr. Max length is EOS_MATCHMAKING_ATTRIBUTEVALUE_MAX_LENGTH. */
		const char* AsUtf8;
		/** Stored as an array of int64 values. Should not be nullptr. */
		const int64_t* AsInt64Array;
		/** Stored as an array of double values. Should not be nullptr. */
		const double* AsDoubleArray;
		/** Stored as an array of null terminated UTF8 strings. Should not be nullptr. Max length of individual UTF8 strings is EOS_MATCHMAKING_ATTRIBUTEVALUE_MAX_LENGTH. */
		const char** AsUtf8Array;
	} Value;
	/** Should be set if the attribute is an array type. */
	uint32_t ValueCount;
	/** Type of value stored in the union */
	EOS_EMatchmakingAttributeType ValueType;
));

#define EOS_MATCHMAKINGCONSTRAINT_ADDCONDITION_API_LATEST 1

/**
 * Input parameters for the EOS_MatchmakingConstraint_AddCondition function.
 */
EOS_STRUCT(EOS_MatchmakingConstraint_AddConditionOptions, (
	/** API Version: Set this to EOS_MATCHMAKINGCONSTRAINT_ADDCONDITION_API_LATEST. */
	int32_t ApiVersion;
	/** The Key to the attribute that will serve as the LHS for this condition. Max length is EOS_MATCHMAKING_ATTRIBUTEKEY_MAX_LENGTH */
	const char* LHSAttributeKey;
	/** The attribute value that will serve as the RHS for this condition. */
	const EOS_Matchmaking_AttributeData* RHSAttributeValue;
	/** The comparison operator for this condition. */
	EOS_EComparisonOp ComparisonOp;
));

#define EOS_MATCHMAKINGCONSTRAINT_ADDSUBCONSTRAINT_API_LATEST 1

/**
 * Input parameters for the EOS_MatchmakingConstraint_AddSubConstraint function.
 */
EOS_STRUCT(EOS_MatchmakingConstraint_AddSubConstraintOptions, (
	/** API Version: Set this to EOS_MATCHMAKINGCONSTRAINT_ADDSUBCONSTRAINT_API_LATEST. */
	int32_t ApiVersion;
	/** The sub constraint that will be part of this constraint. */
	EOS_HMatchmakingConstraint SubConstraintHandle;
));

#define EOS_MATCHMAKINGCONSTRAINT_RESET_API_LATEST 1

/**
 * Input parameters for the EOS_MatchmakingConstraint_Reset function.
 */
EOS_STRUCT(EOS_MatchmakingConstraint_ResetOptions, (
	/** API Version: Set this to EOS_MATCHMAKINGCONSTRAINT_RESET_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_Matchmaking_CreateConstraint API. */
#define EOS_MATCHMAKING_CREATECONSTRAINT_API_LATEST 1

/**
 * Input parameters for the EOS_Matchmaking_CreateConstraint function.
 */
EOS_STRUCT(EOS_Matchmaking_CreateConstraintOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_CREATECONSTRAINT_API_LATEST. */
	int32_t ApiVersion;
	/** The operator used by this constraint to logically combine conditions or sub constraints. */
	EOS_ELogicalCombineOp LogicalCombineOp;
));

/** The most recent version of the EOS_Matchmaking_RegisterSession API. */
#define EOS_MATCHMAKING_REGISTERSESSION_API_LATEST 1

/**
 * Input parameters for the EOS_Matchmaking_RegisterSession function.
 */
EOS_STRUCT(EOS_Matchmaking_RegisterSessionOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_REGISTERSESSION_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * The handle to a Matchmaking constraint that operates on the Match attributes. It is used to define the criteria that must be satisfied before
	 * a match assignment is made to the matchmaking session.
	 */
	EOS_HMatchmakingConstraint SessionConstraintHandle;
	/**
	 * A null terminated UTF8 Json string representing session specific attributes that will be sent to the players upon match assignment.
	 * Max length is EOS_MATCHMAKING_SESSION_ATTRIBUTES_MAX_LENGTH.
	 */
	const char* SessionPlayerAttributesUtf8Json;
	/**
	 * Product User ID for the user who registers the session.
	 * Will be nulled for Dedicated Server where no local user is available.
	 */
	EOS_ProductUserId LocalUserId;
	/** 
	* An ID to uniquely identify the server registering the session.
	* As an added security measure if an extended EOS exchange token is used to call this API, the sessionId claims should match the value specified here.
	*/
	const char* ServerId;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for the EOS_Matchmaking_RegisterSession API call.
 */
EOS_STRUCT(EOS_Matchmaking_RegisterSessionCallbackInfo, (
	/**
	 * Possible result codes for the operation:
	 * - EOS_Success:                 The request was successful.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_InvalidUser:             An invalid Product User Id is specified.
	 * - EOS_InvalidAuth:             A valid Product User Id is specified but the local user is not logged in.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Matchmaking_RegisterSession. */
	void* ClientData;
	/**
	 * Product User ID for the user who registers the session.
	 * Will be nulled for Dedicated Server where no local user is available. 
	 */
	EOS_ProductUserId LocalUserId;
	/** The null terminated UTF8 Matchmaking session Id string that can be used to track assignments and state notifications for the session. */
	EOS_Matchmaking_SessionId MatchmakingSessionId;
));

/**
 * Function prototype definition for callback passed to the EOS_Matchmaking_RegisterSession function.
 * @param Data A EOS_Matchmaking_RegisterSessionCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnRegisterSessionCallback, const EOS_Matchmaking_RegisterSessionCallbackInfo* Data);

/** The most recent version of the EOS_Matchmaking_DeregisterSession API. */
#define EOS_MATCHMAKING_DEREGISTERSESSION_API_LATEST 1

/**
 * Input parameters for the EOS_Matchmaking_DeregisterSession function.
 */
EOS_STRUCT(EOS_Matchmaking_DeregisterSessionOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_DEREGISTERSESSION_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * The matchmaking session id to Deregister. Should be the same value that is received in the Register Session Callback.
	 */
	EOS_Matchmaking_SessionId MatchmakingSessionId;
	/**
	 * Product User ID for the user who Deregisters the session.
	 * Will be nulled for Dedicated Server where no local user is available.
	 */
	EOS_ProductUserId LocalUserId;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for the EOS_Matchmaking_DeregisterSession API call.
 */
EOS_STRUCT(EOS_Matchmaking_DeregisterSessionCallbackInfo, (
	/**
	 * Possible result codes for the operation:
	 * - EOS_Success:                 The request was successful.
	 * - EOS_NotFound				   If the Matchmaking Session Id is invalid.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_InvalidUser:             An invalid Product User Id is specified.
	 * - EOS_InvalidAuth:             A valid Product User Id is specified but the local user is not logged in.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Matchmaking_DeregisterSession. */
	void* ClientData;
	/**
	 * Product User ID for the user who registers the session.
	 * Will be nulled for Dedicated Server where no local user is available.
	 */
	EOS_ProductUserId LocalUserId;
	/** The null terminated UTF8 Matchmaking session Id string that can be used to track assignments and state notifications to the session. */
	EOS_Matchmaking_SessionId MatchmakingSessionId;
));

/**
 * Function prototype definition for callback passed to the EOS_Matchmaking_DeregisterSession function.
 * @param Data A EOS_Matchmaking_DeregisterSessionCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnDeregisterSessionCallback, const EOS_Matchmaking_DeregisterSessionCallbackInfo* Data);

/** The most recent version of the EOS_Matchmaking_AddNotifySessionStateChanged API. */
#define EOS_MATCHMAKING_ADDNOTIFYSESSIONSTATECHANGED_API_LATEST 1

/**
 * Input parameters for the EOS_Matchmaking_AddNotifySessionStateChanged function.
 */
EOS_STRUCT(EOS_Matchmaking_AddNotifySessionStateChangedOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_ADDNOTIFYSESSIONSTATECHANGED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * An enumeration defining the different states of a Matchmaking Session.
 */
EOS_ENUM(EOS_Matchmaking_ESessionState,
	/** The Matchmaking Session is Created. */
	EOS_MSS_CREATED = 0,
	/** The Matchmaking Session is waiting for Assignment. */
	EOS_MSS_WAITING_FOR_ASSIGNMENT = 1,
	/** A Match Assignment for this session has been received. */
	EOS_MSS_ASSIGNMENT_RECEIVED = 2,
	/** A Match Assignment response for this session is in progress. */
	EOS_MSS_ASSIGNMENT_ACKNOWLEDGMENT_IN_PROGRESS = 3,
	/** The Matchmaking Session is closed. No more assignments will be received after this state. */
	EOS_MSS_CLOSED = 4
);

/**
 * Output parameters for the EOS_Matchmaking_OnSessionStateChangedCallback Function.
 */
EOS_STRUCT(EOS_Matchmaking_SessionStateChangedCallbackInfo, (
	/** Context that was passed into EOS_Matchmaking_AddNotifySessionStateChanged */
	void* ClientData;
	/** The User Id of the player this session belongs to. Will be NULL on dedicated servers. */
	EOS_ProductUserId LocalUserId;
	/** The ID of the Matchmaking session this notification belongs to. */
	EOS_Matchmaking_SessionId MatchmakingSessionId;
	/** The state of the matchmaking session */
	EOS_Matchmaking_ESessionState State;
));

/**
 * Function prototype definition for notifications that come from EOS_Matchmaking_AddNotifySessionStateChanged
 * @param Data A EOS_Matchmaking_SessionStateChangedCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnSessionStateChangedCallback, const EOS_Matchmaking_SessionStateChangedCallbackInfo* Data);

/** The most recent version of the EOS_Matchmaking_AddNotifySessionAssignmentRequestReceived API. */
#define EOS_MATCHMAKING_ADDNOTIFYSESSIONASSIGNMENTREQUESTRECEIVED_API_LATEST 1

/**
 * Input parameters for the EOS_Matchmaking_AddNotifySessionAssignmentRequestReceived function.
 */
EOS_STRUCT(EOS_Matchmaking_AddNotifySessionAssignmentRequestReceivedOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_ADDNOTIFYSESSIONASSIGNMENTREQUESTRECEIVED_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_Matchmaking_MatchAssignmentInfo Struct. */
#define EOS_MATCHMAKING_MATCHASSIGNMENTINFO_API_LATEST 1

/**
 * Struct containing Match assignment information.
 */
EOS_STRUCT(EOS_Matchmaking_MatchAssignmentInfo, (
	/** API Version: Set this to EOS_MATCHMAKING_MATCHASSIGNMENTINFO_API_LATEST. */
	int32_t ApiVersion;
	/** A null terminated UTF8 Identifier of the bucket used to form the match assignment. */
	const char* BucketId;
	/** A null terminated UTF8 Identifier of the match assignment. */
	const char* MatchId;
	/** A null terminated UTF8 Json of the Match Attributes. */
	const char* MatchAttributesUtf8Json;
));

/**
 * Output parameters for the EOS_Matchmaking_OnSessionAssignmentRequestReceivedCallback Function.
 */
EOS_STRUCT(EOS_Matchmaking_SessionAssignmentRequestReceivedCallbackInfo, (
	/** Context that was passed into EOS_Matchmaking_AddNotifySessionAssignmentRequestReceived */
	void* ClientData;
	/** The User Id of the player this session belongs to. Will be NULL on dedicated servers. */
	EOS_ProductUserId LocalUserId;
	/** The ID of the Matchmaking session this notification belongs to. */
	EOS_Matchmaking_SessionId MatchmakingSessionId;
	/** The Assignment info received in the request. */
	const EOS_Matchmaking_MatchAssignmentInfo* MatchAssignmentInfo;
));

/**
 * Function prototype definition for notifications that come from EOS_Matchmaking_AddNotifySessionAssignmentRequestReceived
 * @param Data A EOS_Matchmaking_SessionAssignmentRequestReceivedCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnSessionAssignmentRequestReceivedCallback, const EOS_Matchmaking_SessionAssignmentRequestReceivedCallbackInfo* Data);

/** The most recent version of the EOS_Matchmaking_AddNotifySessionClosed API. */
#define EOS_MATCHMAKING_ADDNOTIFYSESSIONCLOSED_API_LATEST 1

/**
 * Input parameters for the EOS_Matchmaking_AddNotifySessionClosed function.
 */
EOS_STRUCT(EOS_Matchmaking_AddNotifySessionClosedOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_ADDNOTIFYSESSIONCLOSED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * An enumeration defining the different reasons for closure of a matchmaking session.
 */
EOS_ENUM(EOS_Matchmaking_ESessionCloseReason,
	/** The Matchmaking Session is closed because a failure occurred. */
	EOS_MSCR_MATCHMAKING_ERROR = 0,
	/** The Matchmaking Session is closed because the Session expired. */
	EOS_MSCR_EXPIRED = 1,
	/** The Matchmaking Session is closed because the Session has been Deregistered. */
	EOS_MSCR_DEREGISTERED = 2,
	/** The Matchmaking Session is closed because the assignment acknowledgment failed. */
	EOS_MSCR_ASSIGNMENT_ACK_FAILED = 3,
	/** The Matchmaking Session is closed because the user is logged out or if the SDK is being shutdown. */
	EOS_MSCR_SHUTDOWN = 4,
	/** The Matchmaking Session is closed because the match assignment is successful. */
	EOS_MSCR_SUCCESSFUL = 5
);

/**
 * Output parameters for the EOS_Matchmaking_OnSessionClosedCallback Function.
 */
EOS_STRUCT(EOS_Matchmaking_SessionClosedCallbackInfo, (
	/** Context that was passed into EOS_Matchmaking_AddNotifySessionClosed */
	void* ClientData;
	/** The Product User Id of the player this session belongs to. Will be NULL on dedicated servers. */
	EOS_ProductUserId LocalUserId;
	/** The ID of the Matchmaking session this notification belongs to. */
	EOS_Matchmaking_SessionId MatchmakingSessionId;
	/** The reason for the closure. */
	EOS_Matchmaking_ESessionCloseReason CloseReason;
));

/**
 * Function prototype definition for notifications that come from EOS_Matchmaking_AddNotifySessionClosed
 * @param Data A EOS_Matchmaking_SessionClosedCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnSessionClosedCallback, const EOS_Matchmaking_SessionClosedCallbackInfo* Data);

/** The most recent version of the EOS_Matchmaking_SendSessionAssignmentResponse API. */
#define EOS_MATCHMAKING_SENDSESSIONASSIGNMENTRESPONSE_API_LATEST 1

/**
 * An enumeration defining the different match assignment decision types.
 */
EOS_ENUM(EOS_Matchmaking_EMatchAssignmentDecisionType,
	/** Accept the Match assignment. */
	EOS_MADT_ACCEPT = 0,
	/** Reject the Match assignment and wait for a new assignment. The match assignment will be sent to other matchmaking sessions.*/
	EOS_MADT_REJECT = 1,
	/** Reject the Match assignment and wait for a new assignment. The match is dropped and no other sessions will get this match assignment. */
	EOS_MADT_REJECT_AND_DROP_MATCH = 2,
	/** Reject the Match assignment and stop getting new assignments. The match assignment will be sent to other matchmaking sessions. */
	EOS_MADT_REJECT_AND_DROP_SESSION = 3,
	/** Reject the Match assignment and stop getting new assignments. The match is dropped and no other sessions will get this match assignment. */
	EOS_MADT_REJECT_AND_DROP_MATCH_AND_SESSION = 4
);

/**
 * Input parameters for the EOS_Matchmaking_SendSessionAssignmentResponse function.
 */
EOS_STRUCT(EOS_Matchmaking_SendSessionAssignmentResponseOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_SENDSESSIONASSIGNMENTRESPONSE_API_LATEST. */
	int32_t ApiVersion;
	/** The Match Id of the Assignment. Should match the one received in the Match Assignment notification. Max length is EOS_MATCHMAKING_MATCH_ID_MAX_LENGTH. */
	const char* MatchId;
	/** The matchmaking session id for which the assignment response is being sent. Should match the session id received in the Register Session callback. */
	EOS_Matchmaking_SessionId MatchmakingSessionId;
	/** The assignment decision of the server. */
	EOS_Matchmaking_EMatchAssignmentDecisionType AssignmentDecision;
	/**
	 * The optional match drop message that will be sent to the players when the AssignmentDecision is EOS_MADT_REJECT_AND_DROP_MATCH.
	 * Note: This parameter should be set to null for other decision types. Max length should be EOS_MATCHMAKING_MATCHREJECTMESSAGE_MAX_LENGTH.
	 */
	const char* MatchDropMessage;
	/**
	 * Product User ID for the user who registers the session.
	 * Will be nulled for Dedicated Server where no local user is available.
	 */
	EOS_ProductUserId LocalUserId;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
* Data containing the result information for the EOS_Matchmaking_SendSessionAssignmentResponse API call.
*/
EOS_STRUCT(EOS_Matchmaking_SendSessionAssignmentResponseCallbackInfo, (
	/**
	 * Possible result codes for the operation:
	 * - EOS_Success:									     The request was successful.
	 * - EOS_NotFound									     If the Matchmaking Session Id is invalid.
	 * - EOS_IncompatibleVersion:						     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:							     An options parameter is incorrect.
	 * - EOS_InvalidUser:								     An invalid Product User Id is specified.
	 * - EOS_InvalidAuth:								     A valid Product User Id is specified but the local user is not logged in.
	 * - EOS_TooManyRequests:							     There are too many requests in progress at this time.
	 * - EOS_Matchmaking_MatchAssignmentResponseInProgress:  A Match assignment response is in progress.
	 * - EOS_Matchmaking_MatchAssignmentResponseFailed:      Processing the assignment response failed.
	 * - EOS_Matchmaking_MatchAssignmentAlreadyFinished:     The match assignment specified in the response has already finished.
	 * - EOS_Matchmaking_MatchNotFound:                      The match ID specified in the response is not found.
	 * - EOS_Matchmaking_MatchAssignmentNotFound:            Match assignment for this session was not found.
	 * - EOS_Matchmaking_FailedToRegisterSession:            The session failed to re-register after sending a REJECT response.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Matchmaking_SendSessionAssignmentResponse. */
	void* ClientData;
	/** The matchmaking session id for which this assignment response was sent for. */
	EOS_Matchmaking_SessionId MatchmakingSessionId;
	/**
	 * Product User ID for the user who registers the session.
	 * Will be nulled for Dedicated Server where no local user is available.
	 */
	EOS_ProductUserId LocalUserId;
));

/**
 * Function prototype definition for callback passed to the EOS_Matchmaking_SendSessionAssignmentResponse function.
 * @param Data A EOS_Matchmaking_SendSessionAssignmentResponseCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnSendSessionAssignmentResponseCallback, const EOS_Matchmaking_SendSessionAssignmentResponseCallbackInfo* Data);

/** The most recent version of the EOS_Matchmaking_PlayerInfo Struct. */
#define EOS_MATCHMAKING_PLAYERINFO_API_LATEST 1

/**
 * Struct containing player related information.
 */
EOS_STRUCT(EOS_Matchmaking_PlayerInfo, (
	/** API Version: Set this to EOS_MATCHMAKING_PLAYERINFO_API_LATEST. */
	int32_t ApiVersion;
	/** The list of Product User Ids of the party members including the player. */
	const EOS_ProductUserId* PartyMembers;
	/** The number of items in the PartyMembers list. Max length is EOS_MATCHMAKING_PARTY_MEMBERS_MAX_SIZE */
	uint32_t PartyMembersCount;
	/** Player Attributes that will be used in the matchmaking strategy. Max length is EOS_MATCHMAKING_PLAYER_ATTRIBUTES_MAX_LENGTH */
	const char* PlayerAttributesUtf8Json;
));

/** The most recent version of the EOS_Matchmaking_PlayerJoinTicket API. */
#define EOS_MATCHMAKING_PLAYERJOINTICKET_API_LATEST 1

/**
 * Input parameters for the EOS_Matchmaking_PlayerJoinTicket function.
 */
EOS_STRUCT(EOS_Matchmaking_PlayerJoinTicketOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_PLAYERJOINTICKET_API_LATEST. */
	int32_t ApiVersion;
	/** Required: The Product User ID of the local user requesting to join the ticket. */
	EOS_ProductUserId LocalUserId;
/**
	 * The optional matchmaking ticket id used to join the matchmaking ticket. Max length is EOS_MATCHMAKING_TICKET_ID_MAX_LENGTH. 
	 * Note: If matchmaking ticket id is not provided the SDK will use the backend generated matchmaking ticket Id. 
	 */
	EOS_Matchmaking_TicketId MatchmakingTicketId;
/**
	 * The player info of the player who is joining the matchmaking ticket.
	 * Note: The party members list must be the same as the one used in the ticket request.
	 */
	const EOS_Matchmaking_PlayerInfo* PlayerInfo;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for the EOS_Matchmaking_PlayerJoinTicket API call.
 */
EOS_STRUCT(EOS_Matchmaking_PlayerJoinTicketCallbackInfo, (
	/**
	 * Possible result codes for the operation:
	 * - EOS_Success:                 The request was successful.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_InvalidUser:             An invalid Product User Id is specified.
	 * - EOS_InvalidAuth:             A valid Product User Id is specified but the local user is not logged in.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Matchmaking_PlayerJoinTicket. */
	void* ClientData;
	/** The Product User ID of the local user requesting to join the matchmaking ticket. */
	EOS_ProductUserId LocalUserId;
	/** The matchmaking ticket id with which the EOS_Matchmaking_PlayerJoinTicket API call was made. */
	EOS_Matchmaking_TicketId MatchmakingTicketId;
));

/**
 * Function prototype definition for callback passed to the EOS_Matchmaking_PlayerJoinTicket function.
 * @param Data A EOS_Matchmaking_PlayerJoinTicketCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnPlayerJoinTicketCallback, const EOS_Matchmaking_PlayerJoinTicketCallbackInfo* Data);

/** The most recent version of the EOS_Matchmaking_PlayerCloseTicket API. */
#define EOS_MATCHMAKING_PLAYERCLOSETICKET_API_LATEST 1

/**
 * Input parameters for the EOS_Matchmaking_PlayerCloseTicket function.
 */
EOS_STRUCT(EOS_Matchmaking_PlayerCloseTicketOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_PLAYERCLOSETICKET_API_LATEST. */
	int32_t ApiVersion;
	/** Required: The Product User ID of the local user requesting to close the matchmaking ticket. */
	EOS_ProductUserId LocalUserId;
	/** The matchmaking ticket id to leave. Max length is EOS_MATCHMAKING_TICKET_ID_MAX_LENGTH. */
	EOS_Matchmaking_TicketId MatchmakingTicketId;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for the EOS_Matchmaking_PlayerCloseTicket API call.
 */
EOS_STRUCT(EOS_Matchmaking_PlayerCloseTicketCallbackInfo, (
	/**
	 * Possible result codes for the operation:
	 * - EOS_Success:                 The request was successful.
	 * - EOS_NotFound				  If the Matchmaking Session Id is invalid.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_InvalidUser:             An invalid Product User Id is specified.
	 * - EOS_InvalidAuth:             A valid Product User Id is specified but the local user is not logged in.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Matchmaking_PlayerCloseTicket. */
	void* ClientData;
	/** The Product User ID of the local user requesting the close the ticket. */
	EOS_ProductUserId LocalUserId;
	/** The matchmaking ticket id with which the EOS_Matchmaking_PlayerCloseTicket API call was made. */
	EOS_Matchmaking_TicketId MatchmakingTicketId;
));

/**
 * Function prototype definition for callback passed to the EOS_Matchmaking_PlayerCloseTicket function.
 * @param Data A EOS_Matchmaking_PlayerCloseTicketCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnPlayerCloseTicketCallback, const EOS_Matchmaking_PlayerCloseTicketCallbackInfo* Data);

/**
 * An enumeration defining the different states of a matchmaking ticket.
 */
EOS_ENUM(EOS_Matchmaking_ETicketState,
	/** The matchmaking ticket has been created. */
	EOS_MTS_CREATED = 0,
	/** The matchmaking ticket is queued for matchmaking. */
	EOS_MTS_QUEUED = 1,
	/** The matchmaking ticket received an Assignment. */
	EOS_MTS_ASSIGNMENT_RECEIVED = 2,
	/** Matchmaking for this ticket has closed. Match assignments will no longer be received. */
	EOS_MTS_CLOSED = 3
);

/** The most recent version of the EOS_Matchmaking_AddNotifyTicketStateChanged API. */
#define EOS_MATCHMAKING_ADDNOTIFYTICKETSTATECHANGED_API_LATEST 1

/**
 * Input parameters for the EOS_Matchmaking_AddNotifyTicketStateChanged function.
 */
EOS_STRUCT(EOS_Matchmaking_AddNotifyTicketStateChangedOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_ADDNOTIFYTICKETSTATECHANGED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Matchmaking_OnTicketStateChangedCallback Function.
 */
EOS_STRUCT(EOS_Matchmaking_TicketStateChangedCallbackInfo, (
	/** Context that was passed into EOS_Matchmaking_AddNotifyTicketStateChanged */
	void* ClientData;
	/** The Product User ID of the local user being notified. */
	EOS_ProductUserId LocalUserId;
	/** The ID of the Matchmaking ticket this notification belongs to. */
	EOS_Matchmaking_TicketId MatchmakingTicketId;
	/** The state of the matchmaking ticket */
	EOS_Matchmaking_ETicketState State;
	/** Num. Players Joined. */
	uint32_t NumPlayersJoined;
));

/**
 * Function prototype definition for notifications that come from EOS_Matchmaking_AddNotifyTicketStateChanged
 * @param Data A EOS_Matchmaking_TicketStateChangedCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnTicketStateChangedCallback, const EOS_Matchmaking_TicketStateChangedCallbackInfo* Data);

/** The most recent version of the EOS_Matchmaking_AddNotifyTicketAssignmentReceived API. */
#define EOS_MATCHMAKING_ADDNOTIFYTICKETASSIGNMENTRECEIVED_API_LATEST 1

EOS_STRUCT(EOS_Matchmaking_AddNotifyTicketAssignmentReceivedOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_ADDNOTIFYTICKETASSIGNMENTRECEIVED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Matchmaking_OnTicketAssignmentReceivedCallback Function.
 */
EOS_STRUCT(EOS_Matchmaking_TicketAssignmentReceivedCallbackInfo, (
	/** Context that was passed into EOS_Matchmaking_AddNotifyTicketAssignmentReceived */
	void* ClientData;
	/** The Product User ID of the local user being notified. */
	EOS_ProductUserId LocalUserId;
	/** The ID of the Matchmaking ticket this notification belongs to. */
	EOS_Matchmaking_TicketId MatchmakingTicketId;
	/**
	 * The Matchmaking session specific attributes UTF8 Json. The session attributes originate from the
	 * server that accepted the match assignment.
	 */
	const char* MatchmakingSessionAttributesUtf8;
	/** The Match assignment information. */
	const EOS_Matchmaking_MatchAssignmentInfo* MatchAssignmentInfo;
));

/**
 * Function prototype definition for notifications that come from EOS_Matchmaking_AddNotifyTicketAssignmentReceived
 * @param Data A EOS_Matchmaking_TicketAssignmentReceivedCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnTicketAssignmentReceivedCallback, const EOS_Matchmaking_TicketAssignmentReceivedCallbackInfo* Data);

/** The most recent version of the EOS_Matchmaking_AddNotifyTicketClosed API. */
#define EOS_MATCHMAKING_ADDNOTIFYTICKETCLOSED_API_LATEST 1

EOS_STRUCT(EOS_Matchmaking_AddNotifyTicketClosedOptions, (
	/** API Version: Set this to EOS_MATCHMAKING_ADDNOTIFYTICKETCLOSED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * An enumeration defining the different reasons for closure of a matchmaking ticket.
 */
EOS_ENUM(EOS_Matchmaking_ETicketCloseReason,
	/** The Matchmaking Ticket failed because of a matchmaking error. */
	EOS_MTCR_MATCHMAKING_ERROR = 0,
	/** The Matchmaking ticket is closed because the host rejected the match. */
	EOS_MTCR_MATCH_REJECTED_BY_HOST = 1,
	/** The Matchmaking ticket closed because of an invalid internal configuration error. */
	EOS_MTCR_INVALID_INTERNAL_CONFIGURATION = 2,
	/** The Matchmaking ticket is closed because an invalid ticket id. */
	EOS_MTCR_INVALID_TICKET = 3,
	/** The Matchmaking ticket is closed because the Ticket expired. */
	EOS_MTCR_EXPIRED = 4,
	/** The Matchmaking ticket is closed because the Ticket was closed with a call to EOS_Matchmaking_PlayerCloseTicket */
	EOS_MTCR_PLAYER_TICKET_CLOSED = 5,
	/** The Matchmaking ticket is closed because the user is logged out or if the SDK is being shutdown. */
	EOS_MTCR_SHUTDOWN = 6,
	/** The Matchmaking ticket is closed because the match assignment is successful. */
	EOS_MTCR_SUCCESSFUL = 7
);

/**
 * Output parameters for the EOS_Matchmaking_OnTicketClosedCallback Function.
 */
EOS_STRUCT(EOS_Matchmaking_TicketClosedCallbackInfo, (
	/** Context that was passed into EOS_Matchmaking_AddNotifyTicketClosed */
	void* ClientData;
	/** The Product User ID of the local user being notified. */
	EOS_ProductUserId LocalUserId;
	/** The ID of the Matchmaking ticket this notification belongs to. */
	EOS_Matchmaking_TicketId MatchmakingTicketId;
	/** The Ticket Close Reason. */
	EOS_Matchmaking_ETicketCloseReason CloseReason;
	/** The optional detailed error when the ticket is closed because of a matchmaking error. */
	const char* DetailedErrorMessage;
));

/**
 * Function prototype definition for notifications that come from EOS_Matchmaking_AddNotifyTicketClosed
 * @param Data A EOS_Matchmaking_TicketClosedCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Matchmaking_OnTicketClosedCallback, const EOS_Matchmaking_TicketClosedCallbackInfo* Data);

#pragma pack(pop)