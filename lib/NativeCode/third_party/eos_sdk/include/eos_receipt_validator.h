// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_receipt_validator_types.h"

/**
 * The Receipt Validator interface.
 * This is used to verify purchases made from external providers.
 *
 * @see EOS_Platform_GetReceiptValidatorInterface
 */

/**
 * Verify a purchase.
 *
 * @param Options Structure containing information about the purchase we're verifying.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate
 * @param CompletionDelegate This function is called when the verify purchase operation completes.
 *
 * @return EOS_Success if the operation completes successfully
 *         EOS_InvalidParameters if any of the options are incorrect
 */
EOS_DECLARE_FUNC(void) EOS_ReceiptValidator_VerifyPurchase(EOS_HReceiptValidator Handle, const EOS_ReceiptValidator_VerifyPurchaseOptions* Options, void* ClientData, const EOS_ReceiptValidator_OnVerifyPurchaseCompleteCallback CompletionDelegate);
