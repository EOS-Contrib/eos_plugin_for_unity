// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_inventory_world_types.h"

/**
 * The following EOS_WorldInventory_* functions allow you to manage world inventory items.
 * This includes opening an inventory to query for inventory items that have been defined for your application.
 * Updates can be made to a world inventory via an explicit transaction write.
 */

/**
 * Open world inventory.
 *
 * This will query the given world inventory and on success return a pointer to root item in that inventory data.
 *
 * @param Options Structure containing information about the world inventory we're opening.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate This function is called when the open operation completes.
 *
 * @see EOS_WorldInventory_OnOpenCompleteCallbackInfo
 *
 * Possible result codes:
 * - EOS_Success:                 The request was successful.
 * - EOS_MissingPermissions:      Server permission is required.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 * - EOS_Inventory_InvalidLockId: The inventory failed to be locked.
 * - EOS_Inventory_AlreadyOpen:   The inventory has already been opened previously.
 * - EOS_Inventory_BinaryDataInvalid: The inventory response from the backend contains invalid binary data.
 */
EOS_DECLARE_FUNC(void) EOS_WorldInventory_Open(EOS_HWorldInventory Handle, const EOS_WorldInventory_OpenOptions* Options, void* ClientData, const EOS_WorldInventory_OnOpenCompleteCallback CompletionDelegate);

/**
 * Open world tile inventory.
 *
 * This will query the given world tile inventory and on success return a pointer to root item in that inventory data.
 *
 * @param Options Structure containing information about the inventory we're opening.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate This function is called when the open operation completes.
 *
 * @see EOS_WorldInventory_OnOpenTileCompleteCallbackInfo
 *
 * Possible result codes:
 * - EOS_Success:                 The request was successful.
 * - EOS_MissingPermissions:      Server permission is required.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 * - EOS_Inventory_InvalidLockId: The inventory failed to be locked.
 * - EOS_Inventory_AlreadyOpen:   The inventory has already been opened previously.
 * - EOS_Inventory_BinaryDataInvalid: The inventory response from the backend contains invalid binary data.
 */
EOS_DECLARE_FUNC(void) EOS_WorldInventory_OpenTile(EOS_HWorldInventory Handle, const EOS_WorldInventory_OpenTileOptions* Options, void* ClientData, const EOS_WorldInventory_OnOpenTileCompleteCallback CompletionDelegate);

/**
 * Open world player inventory.
 *
 * This will query the given world player inventory and on success return a pointer to root item in that inventory data.
 *
 * @param Options Structure containing information about the inventory we're opening.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate This function is called when the open operation completes.
 *
 * @see EOS_WorldInventory_OnOpenPlayerCompleteCallbackInfo
 *
 * Possible result codes:
 * - EOS_Success:                 The request was successful.
 * - EOS_MissingPermissions:      Server permission is required.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_Invalid_ProductUserID:   An invalid target user is specified.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 * - EOS_Inventory_InvalidLockId: The inventory failed to be locked.
 * - EOS_Inventory_AlreadyOpen:   The inventory has already been opened previously.
 * - EOS_Inventory_BinaryDataInvalid: The inventory response from the backend contains invalid binary data.
 */
EOS_DECLARE_FUNC(void) EOS_WorldInventory_OpenPlayer(EOS_HWorldInventory Handle, const EOS_WorldInventory_OpenPlayerOptions* Options, void* ClientData, const EOS_WorldInventory_OnOpenPlayerCompleteCallback CompletionDelegate);

/**
 * Close world inventory.
 *
 * This will close the given world inventory.
 *
 * @param Options Structure containing information about the inventory we're closing.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate This function is called when the close operation completes.
 *
 * Possible result codes:
 * - EOS_Success:                 The request was successful.
 * - EOS_MissingPermissions:      Server permission is required.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 * - EOS_Inventory_InvalidLockId: The inventory failed to be unlocked.
 */
EOS_DECLARE_FUNC(void) EOS_WorldInventory_Close(EOS_HWorldInventory Handle, const EOS_WorldInventory_CloseOptions* Options, void* ClientData, const EOS_WorldInventory_OnCloseCompleteCallback CompletionDelegate);

/**
 * Write transaction.
 *
 * This will write any updates made to the inventory data since it was opened and since any previous write.
 * The update will be transactional and will constitute data for multiple inventories.
 *
 * @param Options Structure containing information about the transaction we're updating.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate This function is called when the operation completes.
 *
 * Possible result codes:
 * - EOS_Success:                                     The request was successful.
 * - EOS_MissingPermissions:                          Server permission is required.
 * - EOS_IncompatibleVersion:                         The API version passed in is incorrect.
 * - EOS_InvalidParameters:                           An options parameter is incorrect.
 * - EOS_TooManyRequests:                             There are too many requests in progress at this time.
 * - EOS_Inventory_InvalidLockId:                     The inventory failed to be written.
 * - EOS_NotFound:                                    An inventory was not found matching a supplied inventory handle.
 * - EOS_Inventory_MaxTransactionInventoriesExceeded: The transaction failed because it exceeded the maximum number of update items allowed.
 * - EOS_Inventory_MaxTransactionSizeExceeded:        The transaction failed because it exceeded the maximum total byte size allowed.
 * - EOS_Inventory_ServiceUnavailable:                Inventory Service is unavailable, the operation should be retried later.
 */
EOS_DECLARE_FUNC(void) EOS_WorldInventory_WriteTransaction(EOS_HWorldInventory Handle, const EOS_WorldInventory_WriteTransactionOptions* Options, void* ClientData, const EOS_WorldInventory_OnWriteTransactionCompleteCallback CompletionDelegate);

/**
 * Query for a list of version metadata for a world inventory.
 *
 * @note On a successful query the previous cached data will be cleared before caching the new version metadata.
 * Only the most recent version metadata will be available in the cache using EOS_WorldInventory_CopyVersionMetadataByIndex.
 *
 * @param Options Structure containing information about the inventory we're querying.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate This function is called when the open operation completes.
 *
 * @see EOS_WorldInventory_OnQueryVersionMetadataCompleteCallbackInfo
 *
 * Possible result codes:
 * - EOS_Success:                 The request was successful.
 * - EOS_MissingPermissions:      Server permission is required.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_Invalid_ProductUserID:   An invalid playerId in inventory info was specified.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 */
EOS_DECLARE_FUNC(void) EOS_WorldInventory_QueryVersionMetadata(EOS_HWorldInventory Handle, const EOS_WorldInventory_QueryVersionMetadataOptions* Options, void* ClientData, const EOS_WorldInventory_OnQueryVersionMetadataCompleteCallback CompletionDelegate);

/**
 * Fetch the number of inventory version metadata objects that are cached locally.
 *
 * @param Options The Options associated with retrieving the inventory metadata count.
 *
 * @see EOS_WorldInventory_CopyVersionMetadataByIndex
 *
 * @return Number of inventory version metadata objects or 0 if there is an error
 */
EOS_DECLARE_FUNC(uint32_t) EOS_WorldInventory_GetVersionMetadataCount(EOS_HWorldInventory Handle, const EOS_WorldInventory_GetVersionMetadataCountOptions* Options);

/**
 * Fetches an inventory version metadata from a given index.
 *
 * @param Options Structure containing the index being accessed.
 * @param OutInventoryVersionMetadata The inventory metadata for the given index, if it exists and is valid, use EOS_WorldInventory_VersionMetadata_Release when finished.
 *
 * @see EOS_WorldInventory_VersionMetadata_Release
 *
 * @return EOS_Success if the information is available and passed out in OutInventoryVersionMetadata
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter
 *         EOS_NotFound if the inventory metadata is not found
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_WorldInventory_CopyVersionMetadataByIndex(EOS_HWorldInventory Handle, const EOS_WorldInventory_CopyVersionMetadataByIndexOptions* Options, EOS_WorldInventory_VersionMetadata ** OutInventoryVersionMetadata);

/**
 * Write inventory data to the backend.
 *
 * This will write any updates made to the inventory data since it was opened and since any previous write.
 *
 * @param Options Structure containing information about the inventory we're updating.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate This function is called when the operation completes.
 *
 * Possible result codes:
 * - EOS_Success:                 The request was successful.
 * - EOS_MissingPermissions:      Server permission is required.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 * - EOS_Inventory_InvalidLockId: The inventory failed to be written.
 * - EOS_NotFound:                An inventory was not found matching a supplied inventory handle.
 */
EOS_DECLARE_FUNC(void) EOS_WorldInventory_Write(EOS_HWorldInventory Handle, const EOS_WorldInventory_WriteOptions* Options, void* ClientData, const EOS_WorldInventory_OnWriteCompleteCallback CompletionDelegate);
