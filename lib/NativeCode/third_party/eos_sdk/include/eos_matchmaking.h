// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_matchmaking_types.h"

/**
 * The Matchmaking Interface is used to host matches or join the matchmaking queue using the EOS Matchmaking Service.
 * @see EOS_Platform_GetMatchmakingInterface
 */

/**
 * Creates an empty matchmaking constraints handle.
 *
 * Constraints are a set of rules that must be met for certain criteria to be satisfied. They are very similar to "IF" blocks where a bunch of conditions can be
 * logically combined to form a more complex set of rules.
 *
 * A condition is a basic building block for a constraint. Each condition comprises of an Attribute Key and Data pair along with a comparison operator. Constraints can
 * also have sub constraints. By chaining sub constraints together more complex conditions can be formed.
 *
 * While creating a constraint, a logical combine operator (AND or OR) can be specified using which conditions or sub constraints are logically combined.
 *
 * In the Matchmaking context, constraints are used to specify the rules under which a registered matchmaking session will be selected for match assignments.
 *
 * Example to roughly show how constraints can be configured:
 * Constraint 1 with AND operator and PlayerCount and Game Map conditions:
 *  - ((MatchAttribute["PlayerCount"] LESSTHANOREQUAL 30) AND (MatchAttribute["GameMap"] EQUAL "FOREST"))
 *
 * Constraint 2 with OR operator and PlayerScore and Game Map conditions:
 *  - ((MatchAttribute["PlayerScore"] GREATERTHAN 1000) OR (MatchAttribute["GameMap"] EQUAL "DESERT"))
 *
 * Constraint 3 with OR operator and Constraint1 and Constrain2 as sub constraints:
 *  - ((MatchAttribute["PlayerCount"] LESSTHANOREQUAL 30) AND (MatchAttribute["GameMap"] EQUAL "FOREST")) OR ((MatchAttribute["PlayerScore"] GREATERTHAN 1000) OR (MatchAttribute["GameMap"] EQUAL "DESERT"))
 *
 * Conditions to constraints can be added using the EOS_MatchmakingConstraint_AddCondition API.
 * Sub constraints to constraints can be added using the EOS_MatchmakingConstraint_AddSubConstraint API.
 *
 * @param Options Structure containing information about the Matchmaking Sessions to register.
 * @param ClientData Arbitrary data that is passed back to you in the CompletioDelegate.
 * @param CompletionDelegate A callback that is fired when the join operation completes, either successfully or in error.
 *
 * @return EOS_Success if the operation completes successfully
 *         EOS_InvalidParameters if any of the options are incorrect
 *
 * @see EOS_MatchmakingConstraint_AddCondition
 * @see EOS_MatchmakingConstraint_AddSubConstraint
 * @see EOS_Matchmaking_RegisterSession
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_Matchmaking_CreateConstraint(EOS_HMatchmaking Handle, const EOS_Matchmaking_CreateConstraintOptions* Options, EOS_HMatchmakingConstraint* OutConstraintHandle);

/**
 * Registers a Matchmaking session with the EOS Matchmaking Service to host a match.
 *
 * Once EOS Matchmaking Service determines a suitable match, a match assignment is sent to the matchmaking session. It is up to the server to either accept or reject the match assignment.
 *
 * Notifications to matchmaking sessions can be subscribed to using the EOS_Matchmaking_AddNotifySessionStateChanged, EOS_Matchmaking_AddNotifySessionClosed
 * and EOS_Matchmaking_AddNotifySessionAssignmentRequestReceived APIs. The assignment response can be sent using the EOS_Matchmaking_SendSessionAssignmentResponse API.
 *
 * @param Options Structure containing information about the Matchmaking Sessions to register.
 * @param ClientData Arbitrary data that is passed back to you in the CompletioDelegate.
 * @param CompletionDelegate A callback that is fired when the join operation completes, either successfully or in error.
 *
 * Possible result codes for the operation:
 * - EOS_Success:                 The request was successful.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_InvalidUser:             An invalid Product User Id is specified.
 * - EOS_InvalidAuth:             A valid Product User Id is specified but the local user is not logged in.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 *
 * @see EOS_Matchmaking_AddNotifySessionStateChanged
 * @see EOS_Matchmaking_AddNotifySessionAssignmentRequestReceived
 * @see EOS_Matchmaking_AddNotifySessionClosed
 * @see EOS_Matchmaking_SendSessionAssignmentResponse
 */
EOS_DECLARE_FUNC(void) EOS_Matchmaking_RegisterSession(EOS_HMatchmaking Handle, const EOS_Matchmaking_RegisterSessionOptions* Options, void* ClientData, const EOS_Matchmaking_OnRegisterSessionCallback CompletionDelegate);

/**
 * De-registers a Matchmaking session from EOS Matchmaking Service. Deregistered sessions will no longer receive match assignments.
 *
 * Note: When completed successfully, the game will receive a notification that the session closed if a callback is registered through EOS_Matchmaking_AddNotifySessionClosed.
 *
 * @param Options Structure containing information about the Matchmaking Sessions to unregister.
 * @param ClientData Arbitrary data that is passed back to you in the CompletioDelegate.
 * @param CompletionDelegate A callback that is fired when the join operation completes, either successfully or in error.
 *
 * Possible result codes for the operation:
 * - EOS_Success:                 The request was successful.
 * - EOS_NotFound:                If the Matchmaking Session Id is invalid.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_InvalidUser:             An invalid Product User Id is specified.
 * - EOS_InvalidAuth:             A valid Product User Id is specified but the local user is not logged in.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 *
 * @see EOS_Matchmaking_AddNotifySessionClosed
 */
EOS_DECLARE_FUNC(void) EOS_Matchmaking_DeregisterSession(EOS_HMatchmaking Handle, const EOS_Matchmaking_DeregisterSessionOptions* Options, void* ClientData, const EOS_Matchmaking_OnDeregisterSessionCallback CompletionDelegate);

/**
 * Register to receive notifications when a Matchmaking session undergoes a state change.
 * @note If the returned NotificationId is valid, you must call EOS_Matchmaking_RemoveNotifySessionStateChanged when you no longer wish to have your NotificationHandler called.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a a notification is received.
 *
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise.
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Matchmaking_AddNotifySessionStateChanged(EOS_HMatchmaking Handle, const EOS_Matchmaking_AddNotifySessionStateChangedOptions* Options, void* ClientData, const EOS_Matchmaking_OnSessionStateChangedCallback NotificationFn);

/**
 * Deregister from receiving notifications when a matchmaking session state has changed.
 *
 * @param InId Handle representing the registered callback.
 */
EOS_DECLARE_FUNC(void) EOS_Matchmaking_RemoveNotifySessionStateChanged(EOS_HMatchmaking Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when a Matchmaking session receives a matchmaking assignment.
 * @note If the returned NotificationId is valid, you must call EOS_Matchmaking_RemoveNotifySessionAssignmentRequestReceived when you no longer wish to have your NotificationHandler called.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a a notification is received.
 *
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise.
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Matchmaking_AddNotifySessionAssignmentRequestReceived(EOS_HMatchmaking Handle, const EOS_Matchmaking_AddNotifySessionAssignmentRequestReceivedOptions* Options, void* ClientData, const EOS_Matchmaking_OnSessionAssignmentRequestReceivedCallback NotificationFn);

/**
 * Deregister from receiving notifications when a matchmaking session receives a Matchmaking assignment.
 *
 * @param InId Handle representing the registered callback.
 */
EOS_DECLARE_FUNC(void) EOS_Matchmaking_RemoveNotifySessionAssignmentRequestReceived(EOS_HMatchmaking Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when a Matchmaking session is closed.
 * @note If the returned NotificationId is valid, you must call EOS_Matchmaking_RemoveNotifySessionClosed when you no longer wish to have your NotificationHandler called.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a a notification is received.
 *
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise.
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Matchmaking_AddNotifySessionClosed(EOS_HMatchmaking Handle, const EOS_Matchmaking_AddNotifySessionClosedOptions* Options, void* ClientData, const EOS_Matchmaking_OnSessionClosedCallback NotificationFn);

/**
 * Deregister from receiving notifications when a matchmaking session is closed.
 *
 * @param InId Handle representing the registered callback.
 */
EOS_DECLARE_FUNC(void) EOS_Matchmaking_RemoveNotifySessionClosed(EOS_HMatchmaking Handle, EOS_NotificationId InId);

/**
 * Sends response to a matchmaking assignment.
 *
 * @param Options Structure containing information about the Matchmaking Session Assignment response.
 * @param ClientData Arbitrary data that is passed back to you in the CompletioDelegate.
 * @param CompletionDelegate A callback that is fired when the send assignment operation completes, either successfully or in error.
 *
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
EOS_DECLARE_FUNC(void) EOS_Matchmaking_SendSessionAssignmentResponse(EOS_HMatchmaking Handle, const EOS_Matchmaking_SendSessionAssignmentResponseOptions* Options, void* ClientData, const EOS_Matchmaking_OnSendSessionAssignmentResponseCallback CompletionDelegate);

/**
 * Allows a player with the given ticket id to join the matchmaking ticket.
 *
 * Note: The list of party members in the request must be the same as the party members list used while requesting the ticket.
 *
 * The ticket state undergoes various changes throughout the course of the matchmaking process and they can be tracked using EOS_Matchmaking_AddNotifyTicketStateChanged API.
 * Once the server picks the match, the match assignment is notified through registered callbacks using EOS_Matchmaking_AddNotifyTicketAssignmentReceived API.
 * The matchmaking ticket will be closed once the assignment is complete or if there is an error in matchmaking, this can be tracked using the EOS_Matchmaking_AddNotifyTicketClosed API.
 *
 * @param Options Structure containing information about the ticket to join.
 * @param ClientData Arbitrary data that is passed back to you in the CompletioDelegate.
 * @param CompletionDelegate A callback that is fired when the join operation completes, either successfully or in error.
 *
 * Possible result codes for the operation:
 * - EOS_Success:                 The request was successful.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_InvalidUser:             An invalid Product User Id is specified.
 * - EOS_InvalidAuth:             A valid Product User Id is specified but the local user is not logged in.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 *
 * @see EOS_Matchmaking_AddNotifyTicketStateChanged
 * @see EOS_Matchmaking_AddNotifyTicketClosed
 * @see EOS_Matchmaking_AddNotifyTicketAssignmentReceived
 */
EOS_DECLARE_FUNC(void) EOS_Matchmaking_PlayerJoinTicket(EOS_HMatchmaking Handle, const EOS_Matchmaking_PlayerJoinTicketOptions* Options, void* ClientData, const EOS_Matchmaking_OnPlayerJoinTicketCallback CompletionDelegate);

/**
 * Closes the matchmaking ticket with the given ticket id.
 *
 * @param Options Structure containing information about the ticket to leave.
 * @param ClientData Arbitrary data that is passed back to you in the CompletioDelegate.
 * @param CompletionDelegate A callback that is fired when the leave operation completes, either successfully or in error.
 *
 * Possible result codes for the operation:
 * - EOS_Success:                 The request was successful.
 * - EOS_NotFound:                If the Matchmaking Session Id is invalid.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_InvalidUser:             An invalid Product User Id is specified.
 * - EOS_InvalidAuth:             A valid Product User Id is specified but the local user is not logged in.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 */
EOS_DECLARE_FUNC(void) EOS_Matchmaking_PlayerCloseTicket(EOS_HMatchmaking Handle, const EOS_Matchmaking_PlayerCloseTicketOptions* Options, void* ClientData, const EOS_Matchmaking_OnPlayerCloseTicketCallback CompletionDelegate);

/**
 * Register to receive notifications when a Matchmaking Ticket state has changed.
 * @note If the returned NotificationId is valid, you must call RemoveNotifyTicketStateChanged when you no longer wish to have your NotificationHandler called.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a a notification is received.
 *
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise.
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Matchmaking_AddNotifyTicketStateChanged(EOS_HMatchmaking Handle, const EOS_Matchmaking_AddNotifyTicketStateChangedOptions* Options, void* ClientData, const EOS_Matchmaking_OnTicketStateChangedCallback NotificationFn);

/**
 * Deregister from receiving notifications when a the matchmaking ticket state changes.
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_Matchmaking_RemoveNotifyTicketStateChanged(EOS_HMatchmaking Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when a Matchmaking Assignment is received.
 * @note If the returned NotificationId is valid, you must call RemoveNotifyTicketAssignmentReceived when you no longer wish to have your NotificationHandler called.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a a notification is received.
 *
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise.
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Matchmaking_AddNotifyTicketAssignmentReceived(EOS_HMatchmaking Handle, const EOS_Matchmaking_AddNotifyTicketAssignmentReceivedOptions* Options, void* ClientData, const EOS_Matchmaking_OnTicketAssignmentReceivedCallback NotificationFn);

/**
 * Deregister from receiving notifications when a match assignment for a matchmaking ticket is received.
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_Matchmaking_RemoveNotifyTicketAssignmentReceived(EOS_HMatchmaking Handle, EOS_NotificationId InId);

/**
 * Register to receive notifications when a Matchmaking ticket is closed.
 * @note A Closed notification is sent even in cases where the matchmaking is successfully completed.
 * @note Once closed, the Matchmaking ticket will no longer receive any matchmaking assignments.
 * @note If the returned NotificationId is valid, you must call RemoveNotifyTicketClosed when you no longer wish to have your NotificationHandler called.
 *
 * @param Options Structure containing information about the request.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param NotificationFn A callback that is fired when a a notification is received.
 *
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise.
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_Matchmaking_AddNotifyTicketClosed(EOS_HMatchmaking Handle, const EOS_Matchmaking_AddNotifyTicketClosedOptions* Options, void* ClientData, const EOS_Matchmaking_OnTicketClosedCallback NotificationFn);

/**
 * Deregister from receiving notifications when a matchmaking ticket is closed.
 *
 * @param InId Handle representing the registered callback
 */
EOS_DECLARE_FUNC(void) EOS_Matchmaking_RemoveNotifyTicketClosed(EOS_HMatchmaking Handle, EOS_NotificationId InId);

#include "eos_matchmaking_types.h"

/**
 * Class responsible for the creation and setup of a Matchmaking Session Constraint.
 * The session constraint is a logical expression that can contain a combination of constraint conditions and other constraints.
 */

/**
 * Adds the given condition to the constraint after validating the input.
 *
 * @param Options Structure containing information about the Constraint condition to be added.
 *
 * @return EOS_Success if adding the condition was successful.
 *         EOS_NotImplemented if unsupported operators are used.
 *         EOS_InvalidParameters if the condition has invalid or incompatible parameters.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_MatchmakingConstraint_AddCondition(EOS_HMatchmakingConstraint Handle, const EOS_MatchmakingConstraint_AddConditionOptions* Options);

/**
 * Adds the given sub constraint to the given constraint after validating the input.
 * Note: Adding an empty constraint without any conditions results in an error. Similarly,
 * creating cycles using constraints will result in an error.
 *
 * @param Options Structure containing information about the Constraint to be added.
 *
 * @return EOS_Success if adding the condition was successful.
 *         EOS_InvalidParameters if the number of results requested is invalid
 *         EOS_IncompatibleVersion if the API version passed in is incorrect
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_MatchmakingConstraint_AddSubConstraint(EOS_HMatchmakingConstraint Handle, const EOS_MatchmakingConstraint_AddSubConstraintOptions* Options);

/**
 * Removes all conditions and sub constraints from the given constraint.
 *
 * @param Options Structure containing the reset information.
 *
 * @return EOS_Success if all conditions and sub constraints are successfully removed.
 *         EOS_IncompatibleVersion If the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_MatchmakingConstraint_Reset(EOS_HMatchmakingConstraint Handle, const EOS_MatchmakingConstraint_ResetOptions* Options);
