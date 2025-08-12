// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_inventory_types.h"

/**
 * The following EOS_Inventory_* functions allow you to manage a user's inventory items.
 * This includes opening an inventory to query for a user's inventory items that have been defined for your application.
 * Updates can be made to a user's inventory via an explicit write or once the inventory is closed.
 */

/**
 * Open inventory.
 *
 * This will query the given user's inventory and on success return a pointer to root item in that inventory data.
 *
 * @param Options Structure containing information about the inventory we're opening.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate This function is called when the open operation completes.
 *
 * @see EOS_Inventory_OnOpenCompleteCallbackInfo
 *
 * Possible result codes:
 * - EOS_Success:                 The request was successful.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_Inventory_AlreadyOpen    The specified inventory has already been opened.
 * - EOS_InvalidUser:             If required (not a trusted server), the local user does not exist or is not authenticated.
 * - EOS_InvalidAuth:             If required (not a trusted server), the local user is not logged in.
 * - EOS_Invalid_ProductUserID:   An invalid target user is specified.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 * - EOS_Inventory_InvalidLockId: The inventory failed to be locked.
 */
EOS_DECLARE_FUNC(void) EOS_Inventory_Open(EOS_HInventory Handle, const EOS_Inventory_OpenOptions* Options, void* ClientData, const EOS_Inventory_OnOpenCompleteCallback CompletionDelegate);

/**
 * Close inventory.
 *
 * This will close the given inventory, any updates made to the inventory data will be written before it is closed.
 *
 * @param Options Structure containing information about the inventory we're closing.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate This function is called when the open operation completes.
 *
 * Possible result codes:
 * - EOS_Success:                 The request was successful.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_InvalidUser:             If required (not a trusted server), the local user does not exist or is not authenticated.
 * - EOS_InvalidAuth:             If required (not a trusted server), the local user is not logged in.
 * - EOS_Invalid_ProductUserID:   An invalid target user is specified.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 * - EOS_Inventory_InvalidLockId: The inventory failed to be unlocked.
 */
EOS_DECLARE_FUNC(void) EOS_Inventory_Close(EOS_HInventory Handle, const EOS_Inventory_CloseOptions* Options, void* ClientData, const EOS_Inventory_OnCloseCompleteCallback CompletionDelegate);

/**
 * Write inventory.
 *
 * This will write any updates made to the inventory data since it was opened and since any previous write.
 *
 * @param Options Structure containing information about the inventory we're updating.
 * @param ClientData Arbitrary data that is passed back to you in the CompletionDelegate.
 * @param CompletionDelegate This function is called when the operation completes.
 *
 * Possible result codes:
 * - EOS_Success:                 The request was successful.
 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
 * - EOS_InvalidParameters:       An options parameter is incorrect.
 * - EOS_Invalid_ProductUserID:   An invalid target user is specified.
 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
 * - EOS_Inventory_InvalidLockId: The inventory failed to be written.
 */
EOS_DECLARE_FUNC(void) EOS_Inventory_Write(EOS_HInventory Handle, const EOS_Inventory_WriteOptions* Options, void* ClientData, const EOS_Inventory_OnWriteCompleteCallback CompletionDelegate);

/**
 * Class responsible for the management of inventory items.
 * This includes creating, deleting and retrieving inventory items.
 */

/**
 * Fetch the key name for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 * @param OutKey The Key name for the inventory item. The key must be long enough to hold a string of EOS_INVENTORYITEM_KEY_MAX_LENGTH.
 * @param InOutKeyLength The size of the OutKey in characters.
 *                       The input buffer should include enough space to be null-terminated.
 *                       When the function returns, this parameter will be filled with the length of the string copied into OutKey.
 *
 * @return EOS_Success if the key is retrieved successfully and passed out in OutKey.
 *         EOS_InvalidParameters if you pass a null pointer for either out parameter.
 *         EOS_LimitExceeded if the OutKey is not large enough to receive the key. InOutKeyLength contains the required minimum length to perform the operation successfully.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetKey(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetKeyOptions* Options, char* OutKey, uint32_t* InOutKeyLength);

/**
 * Sets the key name for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if the key set successfully.
 *         EOS_InvalidParameters if any of the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetKey(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetKeyOptions* Options);

/**
 * Fetch the type associated with the value for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 * @param OutValueType The type for the stored value.
 *
 * @return EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetValueType(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetValueTypeOptions* Options, EOS_EInventoryItem_ValueType* OutValueType);

/**
 * Fetch the value for this inventory item as a String.
 *
 * @param Options Structure containing the input parameters.
 * @param OutValue A string representing the value for the inventory item. The value must be long enough to hold a string of EOS_INVENTORYITEM_STRING_VALUE_MAX_LENGTH.
 * @param InOutValueLength The size of the OutValue in characters.
 *                         The input buffer should include enough space to be null-terminated.
 *                         When the function returns, this parameter will be filled with the length of the string copied into OutValue.
 *
 * @return EOS_Success if the value is retrieved successfully and passed out in OutValue.
 *         EOS_InvalidParameters if you pass a null pointer for either out parameter or the value stored for the inventory item is not a string type.
 *         EOS_LimitExceeded if the OutValue is not large enough to receive the value. InOutValueLength contains the required minimum length to perform the operation successfully.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetValueAsString(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetValueAsStringOptions* Options, char* OutValue, uint32_t* InOutValueLength);

/**
 * Fetch the value for this inventory item as a JSON String.
 *
 * @param Options Structure containing the input parameters.
 * @param OutValue A JSON string representing the value for the inventory item. The value must be long enough to hold a string of EOS_INVENTORYITEM_STRING_VALUE_MAX_LENGTH.
 *                 If passed nullptr, the InOutValueLength will contain the expected length of the value required.
 * @param InOutValueLength The size of the OutValue in characters.
 *                         The input buffer should include enough space to be null-terminated.
 *                         When the function returns, this parameter will be filled with the length of the string copied into OutValue.
 *
 * @return EOS_Success if the value is retrieved successfully and passed out in OutValue.
 *         EOS_InvalidParameters if you pass a null pointer for either out parameter or the value stored for the inventory item is not a string type.
 *         EOS_LimitExceeded if the OutValue is not large enough to receive the value. InOutValueLength contains the required minimum length to perform the operation successfully.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetValueAsJsonString(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetValueAsJsonStringOptions* Options, char* OutValue, uint32_t* InOutValueLength);

/**
 * Fetch the value for this inventory item as a Boolean.
 *
 * @param Options Structure containing the input parameters.
 * @param OutValue A pointer to a boolean representing the value for the inventory item.
 *
 * @return EOS_Success if the value is retrieved successfully and passed out in OutValue.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or the value stored for the inventory item is not a boolean type.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetValueAsBool(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetValueAsBoolOptions* Options, EOS_Bool* OutValue);

/**
 * Fetch the value for this inventory item as a 64-bit integer.
 *
 * @param Options Structure containing the input parameters.
 * @param OutValue A pointer to an integer representing the value for the inventory item.
 *
 * @return EOS_Success if the value is retrieved successfully and passed out in OutValue.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or the value stored for the inventory item is not an integer type.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetValueAsInt64(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetValueAsInt64Options* Options, int64_t* OutValue);

/**
 * Fetch the value for this inventory item as a double.
 *
 * @param Options Structure containing the input parameters.
 * @param OutValue A pointer to a double representing the value for the inventory item.
 *
 * @return EOS_Success if the value is retrieved successfully and passed out in OutValue.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or the value stored for the inventory item is not a double type.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetValueAsDouble(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetValueAsDoubleOptions* Options, double* OutValue);

/**
 * Fetch the value for this inventory item as a POSIX timestamp.
 *
 * @param Options Structure containing the input parameters.
 * @param OutValue A pointer to an integer representing the POSIX Timestamp value for the inventory item.
 *
 * @return EOS_Success if the value is retrieved successfully and passed out in OutValue.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or the value stored for the inventory item is not a POSIX Timestamp type.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetValueAsTimestamp(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetValueAsTimestampOptions* Options, int64_t* OutValue);

/**
 * Fetch the value for this inventory item as a 64-bit integer Set.
 *
 * @param Options Structure containing the input parameters.
 * @param OutValue Out parameter used to receive the integer value for the element at the desired index in the set.
 *
 * @return EOS_Success if a valid value is found and returned in OutValue.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or any of the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetInt64SetValueByIndex(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetInt64SetValueByIndexOptions* Options, int64_t* OutValue);

/**
 * Fetch the value for this inventory item as a double Set.
 *
 * @param Options Structure containing the input parameters.
 * @param OutValue Out parameter used to receive the double value for the element at the desired index in the set.
 *
 * @return EOS_Success if a valid value is found and returned in OutValue.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or any of the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetDoubleSetValueByIndex(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetDoubleSetValueByIndexOptions* Options, double* OutValue);

/**
 * Fetch the value for this inventory item as a string Set.
 *
 * @param Options Structure containing the input parameters.
 * @param OutValue Out parameter used to receive the string value for the element at the desired index in the set.
 * @param InOutStringLength The size of the OutValue in characters.
 *                          The input buffer should include enough space to be null-terminated.
 *                          When the function returns, this parameter will be filled with the length of the string copied into OutValue.
 *                          EOS_LimitExceeded if the OutValue is not large enough to receive the value. InOutStringLength contains the required minimum length to perform the operation successfully.
 *
 * @return EOS_Success if a valid value is found and returned in OutValue.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or any of the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetStringSetValueByIndex(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetStringSetValueByIndexOptions* Options, char* OutValue, uint32_t* InOutStringLength);

/**
 * Get the item value count. Length of string, size of a set or zero otherwise.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return The item value count. Length of string, size of a set or zero otherwise.
 */
EOS_DECLARE_FUNC(uint32_t) EOS_InventoryItem_GetValueCount(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetValueCountOptions* Options);

/**
 * Set the value for this inventory item as a String.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if a valid value is set.
 *         EOS_InvalidParameters if the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the item has been previously deleted, the item will need to be re-created.
 *
 * @note Before writing the strings to the Inventory service control characters will be replaced with the escaped versions.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetValueAsString(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetValueAsStringOptions* Options);

/**
 * Set the value for this inventory item as a JSON document.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if a valid value is set.
 *         EOS_InvalidParameters if the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_LimitExceeded if the JSON string passed in the Options structure exceeds length EOS_INVENTORYITEM_STRING_VALUE_MAX_LENGTH.
 *         EOS_NotFound if the item has been previously deleted, the item will need to be re-created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetValueAsJson(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetValueAsJsonOptions* Options);

/**
 * Set the value for this inventory item as a Boolean.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if a valid value is set.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the item has been previously deleted, the item will need to be re-created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetValueAsBool(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetValueAsBoolOptions* Options);

/**
 * Set the value for this inventory item as a 64-bit integer number.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if a valid value is set.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the item has been previously deleted, the item will need to be re-created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetValueAsInt64(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetValueAsInt64Options* Options);

/**
 * Set the value for this inventory item as a double.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if a valid value is set.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the item has been previously deleted, the item will need to be re-created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetValueAsDouble(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetValueAsDoubleOptions* Options);

/**
 * Set the value for this inventory item as a POSIX timestamp.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if a valid value is set.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the item has been previously deleted, the item will need to be re-created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetValueAsTimestamp(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetValueAsTimestampOptions* Options);

/**
 * Set the value for this inventory item as a 64-bit integer Set.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if a valid value is set.
 *         EOS_InvalidParameters if the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the item has been previously deleted, the item will need to be re-created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetValueAsInt64Set(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetValueAsInt64SetOptions* Options);

/**
 * Set the value for this inventory item as a double Set.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if a valid value is set.
 *         EOS_InvalidParameters if the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the item has been previously deleted, the item will need to be re-created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetValueAsDoubleSet(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetValueAsDoubleSetOptions* Options);

/**
 * Set the value for this inventory item as a string Set.
 *
 * @param Options Structure containing the input parameters
 *
 * @return EOS_Success if a valid value is set.
 *         EOS_InvalidParameters if the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the item has been previously deleted, the item will need to be re-created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetValueAsStringSet(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetValueAsStringSetOptions* Options);

/**
 * Set the value for this inventory item as undefined.
 *
 * @param Options Structure containing the input parameters
 *
 * @return EOS_Success if a valid value is set.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the item has been previously deleted, the item will need to be re-created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_SetValueAsUndefined(EOS_HInventoryItem Handle, const EOS_InventoryItem_SetValueAsUndefinedOptions* Options);

/**
 * Compare the provided String value against the current String value set for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if the value provided is different from the current value.
 *         EOS_NoChange if the value provided is the same as the current value.
 *         EOS_InvalidParameters if the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CompareValueAsString(EOS_HInventoryItem Handle, const EOS_InventoryItem_CompareValueAsStringOptions* Options);

/**
 * Compare the provided JSON document value against the current JSON document value set for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if the value provided is different from the current value.
 *         EOS_NoChange if the value provided is the same as the current value.
 *         EOS_InvalidParameters if the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_LimitExceeded if the JSON string passed in the Options structure exceeds length EOS_INVENTORYITEM_STRING_VALUE_MAX_LENGTH.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CompareValueAsJson(EOS_HInventoryItem Handle, const EOS_InventoryItem_CompareValueAsJsonOptions* Options);

/**
 * Compare the provided Boolean value against the current Boolean value set for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if the value provided is different from the current value.
 *         EOS_NoChange if the value provided is the same as the current value.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CompareValueAsBool(EOS_HInventoryItem Handle, const EOS_InventoryItem_CompareValueAsBoolOptions* Options);

/**
 * Compare the provided 64-bit integer value against the current 64-bit integer value set for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if the value provided is different from the current value.
 *         EOS_NoChange if the value provided is the same as the current value.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CompareValueAsInt64(EOS_HInventoryItem Handle, const EOS_InventoryItem_CompareValueAsInt64Options* Options);

/**
 * Compare the provided Double value against the current Double value set for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if the value provided is different from the current value.
 *         EOS_NoChange if the value provided is the same as the current value.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CompareValueAsDouble(EOS_HInventoryItem Handle, const EOS_InventoryItem_CompareValueAsDoubleOptions* Options);

/**
 * Compare the provided POSIX timestamp value against the current POSIX timestamp value set for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if the value provided is different from the current value.
 *         EOS_NoChange if the value provided is the same as the current value.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CompareValueAsTimestamp(EOS_HInventoryItem Handle, const EOS_InventoryItem_CompareValueAsTimestampOptions* Options);

/**
 * Compare the provided 64-bit Integer Set value against the current 64-bit Integer Set value set for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if the value provided is different from the current value.
 *         EOS_NoChange if the value provided is the same as the current value.
 *         EOS_InvalidParameters if the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CompareValueAsInt64Set(EOS_HInventoryItem Handle, const EOS_InventoryItem_CompareValueAsInt64SetOptions* Options);

/**
 * Compare the provided Double Set value against the current Double Set value set for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if the value provided is different from the current value.
 *         EOS_NoChange if the value provided is the same as the current value.
 *         EOS_InvalidParameters if the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CompareValueAsDoubleSet(EOS_HInventoryItem Handle, const EOS_InventoryItem_CompareValueAsDoubleSetOptions* Options);

/**
 * Compare the provided String Set value against the current String Set value set for this inventory item.
 *
 * @param Options Structure containing the input parameters
 *
 * @return EOS_Success if the value provided is different from the current value.
 *         EOS_NoChange if the value provided is the same as the current value.
 *         EOS_InvalidParameters if the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CompareValueAsStringSet(EOS_HInventoryItem Handle, const EOS_InventoryItem_CompareValueAsStringSetOptions* Options);

/**
 * EOS_InventoryItem_CreateSubItem is used to create an inventory item.
 * If an item with the same key already exists it's handle is returned in OutInventoryItemHandle.
 *
 * @param Options Structure containing the input parameters.
 * @param OutInventoryItemHandle Out parameter used to receive the handle for the inventory item.
 *
 * @return EOS_Success if a new inventory item is created, a handle to the new item will be passed out in OutInventoryItemHandle.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or any of the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NoChange if a duplicate item with the same key is found, in which case a new item will not be created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CreateSubItem(EOS_HInventoryItem Handle, const EOS_InventoryItem_CreateSubItemOptions* Options, EOS_HInventoryItem* OutInventoryItemHandle);

/**
 * EOS_InventoryItem_CreateSubItemFromPath is used to create an inventory item.
 * If an item with the same path already exists it's handle is returned in OutInventoryItemHandle.
 *
 * @param Options Structure containing the input parameters.
 * @param OutInventoryItemHandle Out parameter used to receive the handle for the inventory item.
 *
 * @return EOS_Success if a new inventory item is created, a handle to the new item will be passed out in OutInventoryItemHandle.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or any of the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NoChange if a duplicate item with the same path is found, in which case a new item will not be created.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CreateSubItemFromPath(EOS_HInventoryItem Handle, const EOS_InventoryItem_CreateSubItemFromPathOptions* Options, EOS_HInventoryItem* OutInventoryItemHandle);

/**
 * EOS_InventoryItem_GetSubItem is used to retrieve an inventory item.
 *
 * @param Options Structure containing the input parameters.
 * @param OutInventoryItemHandle Out parameter used to receive the handle to the inventory item.
 *
 * @return EOS_Success if the inventory item is found and passed out in OutInventoryItemHandle.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or any of the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the inventory item is not found.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetSubItem(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetSubItemOptions* Options, EOS_HInventoryItem* OutInventoryItemHandle);

/**
 * EOS_InventoryItem_GetSubItemByIndex is used to retrieve a sub inventory items at a given index.
 *
 * @param Options Structure containing the input parameters.
 * @param OutInventoryItemHandle Out parameter used to receive a handle to the sub inventory item.
 *
 * @return EOS_Success if inventory item is available and passed out in OutInventoryItemHandle.
 *         EOS_InvalidParameters if you pass a null pointer for the out parameter or any of the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if no inventory item is found.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetSubItemByIndex(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetSubItemByIndexOptions* Options, EOS_HInventoryItem* OutInventoryItemHandle);

/**
 * Fetch the number of sub items.
 *
 * @param Options The Options associated with retrieving the sub item count.
 *
 * @return Number of sub items or 0 if there is an error.
 *
 * @see EOS_InventoryItem_GetSubItemByIndex
 */
EOS_DECLARE_FUNC(uint32_t) EOS_InventoryItem_GetSubItemsCount(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetSubItemsCountOptions* Options);

/**
 * EOS_InventoryItem_DeleteSubItem is used to delete an inventory item.
 *
 * @param Options Structure containing the input parameters.
 *
 * @return EOS_Success if inventory items are found and passed out in OutInventoryItemHandles.
 *         EOS_InvalidParameters if any of the options are incorrect.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 *         EOS_NotFound if the sub inventory item is not found.
 *
 * @note Sub items that have been deleted will be set to an undefined value and will be removed after the next update has been written to the backend.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_DeleteSubItem(EOS_HInventoryItem Handle, const EOS_InventoryItem_DeleteSubItemOptions* Options);

/**
 * Fetch the full path for this inventory item.
 *
 * @param Options Structure containing the input parameters.
 * @param OutFullPath The full path for the inventory item. The full path must be long enough to hold a string of EOS_INVENTORYITEM_KEY_MAX_LENGTH.
 * @param InOutFullPathLength The size of the OutFullPath in characters.
 *                            The input buffer should include enough space to be null-terminated.
 *                            When the function returns, this parameter will be filled with the length of the string copied into OutFullPath.
 *
 * @return EOS_Success if the full path is retrieved successfully and passed out in OutFullPath.
 *         EOS_InvalidParameters if you pass a null pointer for either out parameter.
 *         EOS_LimitExceeded if the OutFullPath is not large enough to receive the full path. InOutFullPathLength contains the required minimum length to perform the operation successfully.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_GetFullPath(EOS_HInventoryItem Handle, const EOS_InventoryItem_GetFullPathOptions* Options, char* OutFullPath, uint32_t* InOutFullPathLength);

/**
 * Clears existing sub items of DestinationHandle and performs a deep copy of all the sub items into DestinationHandle.
 *
 * @param Options Structure containing the input parameters.
 * @param DestinationHandle The destination inventory handle where all the items are copied to.
 *
 * @return EOS_Success if the copied successfully.
 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_InventoryItem_CopyHandle(EOS_HInventoryItem Handle, const EOS_InventoryItem_CopyHandleOptions* Options, EOS_HInventoryItem DestinationHandle);
