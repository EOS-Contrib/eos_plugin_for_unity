// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_eulatracking_types.h"

/**
 * Queries the user's response status to the latest version of the agreement referenced.
 *
 * @param Options Structure containing the Epic Account ID of the user and the agreement key to see the status of.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate A callback that contains the agreement status for the user once the query is complete, or on an error.
 *
 * @return EOS_Success If the callback contains the user's response state for the agreement.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_InvalidParameters If any of the options are incorrect.
 *         EOS_ApplicationSuspended If the application is suspended.
 *         EOS_NetworkDisconnected If the network is disconnected.
 */
EOS_DECLARE_FUNC(void) EOS_EULATracking_QueryUserAgreementStatus(EOS_HEULATracking Handle, const EOS_EULATracking_QueryUserAgreementStatusOptions* Options, void* ClientData, const EOS_EULATracking_OnQueryUserAgreementStatusCallback CompletionDelegate);

/**
 * Listen for changes to agreement statuses.
 *
 * @param Options Contains API Version.
 * @param ClientData This value is returned to the caller when UserAgreementStatusUpdateHandler is invoked.
 * @param UserAgreementStatusUpdateHandler The callback to be invoked when a change to the agreement status changes.
 * @return A valid notification ID if successfully bound, or EOS_INVALID_NOTIFICATIONID otherwise
 */
EOS_DECLARE_FUNC(EOS_NotificationId) EOS_EULATracking_AddNotifyUserAgreementStatusUpdate(EOS_HEULATracking Handle, const EOS_EULATracking_AddNotifyUserAgreementStatusUpdateOptions* Options, void* ClientData, const EOS_EULATracking_OnUserAgreementStatusUpdateCallback UserAgreementStatusUpdateHandler);

/**
 * Stop listening for user agreement status changes on a previously bound handler.
 *
 * @param NotificationId The previously bound notification ID.
 */
EOS_DECLARE_FUNC(void) EOS_EULATracking_RemoveNotifyUserAgreementStatusUpdate(EOS_HEULATracking Handle, EOS_NotificationId NotificationId);
