// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

/** Handle to the inventory interface */
EOS_EXTERN_C typedef struct EOS_InventoryHandle* EOS_HInventory;

/** Handle to the calls responsible for managing inventory items */
EOS_EXTERN_C typedef struct EOS_InventoryItemHandle* EOS_HInventoryItem;

/** Max length of an inventory item key name */
#define EOS_INVENTORYITEM_KEY_MAX_LENGTH 2048

/** Max length of an inventory item value stored as a string in bytes. Note: Max Length is checked for the replaced string after escaping special characters which the SDK does internally. */
#define EOS_INVENTORYITEM_STRING_VALUE_MAX_LENGTH 400000

/**
 * Supported types of data that can be stored in inventory item values
 */
EOS_ENUM(EOS_EInventoryItem_ValueType,
	/** No type assigned for value */
	EOS_IIVT_UNDEFINED = 0,
	/** UTF8 String */
	EOS_IIVT_STRING = 1,
	/** Boolean value (true/false) */
	EOS_IIVT_BOOLEAN = 2,
	/** POSIX Timestamp (as 64-bit integer) */
	EOS_IIVT_TIMESTAMP = 3,
	/** 64-bit integer */
	EOS_IIVT_INTEGER = 4,
	/** Double precision floating point number */
	EOS_IIVT_DOUBLE = 5,
	/** A set of integer typed inventory items */
	EOS_IIVT_INTEGERSET = 6,
	/** A set of double typed inventory items */
	EOS_IIVT_DOUBLESET = 7,
	/** A set of string typed inventory items */
	EOS_IIVT_STRINGSET = 8,
	/** A JSON Object */
	EOS_IIVT_JSONOBJECT = 9
);

/** The most recent version of the EOS_Inventory_Open API. */
#define EOS_INVENTORY_OPEN_API_LATEST 1

/**
 * Input parameters for the EOS_Inventory_Open function.
 */
EOS_STRUCT(EOS_Inventory_OpenOptions, (
	/** API Version: Set this to EOS_INVENTORY_OPEN_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * Product User ID for user who is opening the inventory and querying for inventory items.
	 * Must be set when using a client policy that requires a valid logged in user.
	 * Not used for Dedicated Server where no local user is available.
	 */
	EOS_ProductUserId LocalUserId;
	/** Product User ID for the user whose inventory items are being queried for. */
	EOS_ProductUserId TargetUserId;
	/** Name of inventory to open. */
	const char* InventoryName;
	/** Prefix for inventory to open. Only inventory items beneath this prefix path will be included in the queried items. Set to nullptr to query the full inventory. */
	const char* Prefix;
	/**
	 * Session Lock ID for Session Locking.
	 * Only used for Dedicated Server when writing the updated inventory items for the specified user.
	 * Note: Using the same Session Lock ID for all EOS_Inventory_Open calls is recommended for optimal updates when writing updates for multiple users.
	 */
	const char* SessionLockId;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for opening inventory and querying for player inventory items request.
 */
EOS_STRUCT(EOS_Inventory_OnOpenCompleteCallbackInfo, (
	/**
	 * The result code for the operation.
	 * - EOS_Success:                 The request was successful.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_Inventory_AlreadyOpen    If the specified inventory has already been opened.
	 * - EOS_InvalidUser:             If required (not a trusted server), the local user does not exist or is not authenticated.
	 * - EOS_InvalidAuth:             If required (not a trusted server), the local user is not logged in.
	 * - EOS_Invalid_ProductUserID:   An invalid target user is specified.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 * - EOS_Inventory_InvalidLockId: The inventory failed to be locked.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Inventory_Open. */
	void* ClientData;
	/** A pointer to the root handle of the inventory items (on Success). Null on failure. */
	EOS_HInventoryItem InventoryItemHandle;
	/**
	 * Product User ID for the user who closed the inventory.
	 * Will be nulled for Dedicated Server where no local user is available.
	 */
	EOS_ProductUserId LocalUserId;
	/** Product User ID for the user whose inventory was opened. */
	EOS_ProductUserId TargetUserId;
	/** Name of inventory that was opened. */
	const char* InventoryName;
));

/**
 * Function prototype definition for callbacks passed to EOS_Inventory_Open.
 * @param Data A EOS_Inventory_OnOpenCompleteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Inventory_OnOpenCompleteCallback, const EOS_Inventory_OnOpenCompleteCallbackInfo* Data);

/** The most recent version of the EOS_Inventory_Close API. */
#define EOS_INVENTORY_CLOSE_API_LATEST 2

/**
 * Input parameters for the EOS_Inventory_Close function.
 */
EOS_STRUCT(EOS_Inventory_CloseOptions, (
	/** API Version: Set this to EOS_INVENTORY_CLOSE_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * Product User ID for user who is closing the inventory and writing updates to inventory items.
	 * Must be set when using a client policy that requires a valid logged in user.
	 * Not used for Dedicated Server where no local user is available.
	 */
	EOS_ProductUserId LocalUserId;
	/** Product User ID for the user whose inventory is being closed. */
	EOS_ProductUserId TargetUserId;
	/** A pointer to the root handle of the inventory items. */
	EOS_HInventoryItem InventoryItemHandle;
	/** A flag indicating if any pending changes should be written prior to close. */
	EOS_Bool bWritePendingChangesBeforeClose;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for closing the inventory and writing updates to player inventory items.
 */
EOS_STRUCT(EOS_Inventory_OnCloseCompleteCallbackInfo, (
	/**
	 * The result code for the operation.
	 * - EOS_Success:                 The request was successful.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_InvalidUser:             If required (not a trusted server), the local user does not exist or is not authenticated.
	 * - EOS_InvalidAuth:             If required (not a trusted server), the local user is not logged in.
	 * - EOS_Invalid_ProductUserID:   An invalid target user is specified.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 * - EOS_Inventory_InvalidLockId: The inventory failed to be unlocked.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Inventory_Close. */
	void* ClientData;
	/**
	 * Product User ID for the user who closed the inventory.
	 * Will be empty for Dedicated Server where no local user is available.
	 */
	EOS_ProductUserId LocalUserId;
	/** Product User ID for the user whose inventory was closed. */
	EOS_ProductUserId TargetUserId;
	/** Name of inventory that was closed. */
	const char* InventoryName;
));

/**
 * Function prototype definition for callbacks passed to EOS_Inventory_Close.
 * @param Data A EOS_Inventory_OnCloseCompleteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Inventory_OnCloseCompleteCallback, const EOS_Inventory_OnCloseCompleteCallbackInfo* Data);

/** The most recent version of the EOS_Inventory_Write API. */
#define EOS_INVENTORY_WRITE_API_LATEST 1

/**
 * Input parameters for the EOS_Inventory_Write function.
 */
EOS_STRUCT(EOS_Inventory_WriteOptions, (
	/** API Version: Set this to EOS_INVENTORY_WRITE_API_LATEST. */
	int32_t ApiVersion;
	/** Product User ID for the user whose inventory items are being written. */
	EOS_ProductUserId TargetUserId;
	/** A pointer to the root handle of the inventory items. */
	EOS_HInventoryItem InventoryItemHandle;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for updating the inventory.
 */
EOS_STRUCT(EOS_Inventory_OnWriteCompleteCallbackInfo, (
	/**
	 * The result code for the operation.
	 * - EOS_Success:                 The request was successful.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_Invalid_ProductUserID:   An invalid target user is specified.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 * - EOS_Inventory_InvalidLockId: The inventory failed to be written.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Inventory_Write. */
	void* ClientData;
	/** Product User ID for the user whose inventory items were written. */
	EOS_ProductUserId TargetUserId;
	/** Name of user's inventory that was written. */
	const char* InventoryName;
));

/**
 * Function prototype definition for callbacks passed to EOS_Inventory_Write.
 * @param Data A EOS_Inventory_OnWriteCompleteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Inventory_OnWriteCompleteCallback, const EOS_Inventory_OnWriteCompleteCallbackInfo* Data);

/** The most recent version of the EOS_InventoryItem_GetKey API. */
#define EOS_INVENTORYITEM_GETKEY_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetKey function.
 */
EOS_STRUCT(EOS_InventoryItem_GetKeyOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETKEY_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_SetKey API. */
#define EOS_INVENTORYITEM_SETKEY_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetKey function.
 */
EOS_STRUCT(EOS_InventoryItem_SetKeyOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETKEY_API_LATEST. */
	int32_t ApiVersion;
	/** Key. */
	const char* Key;
));

/** The most recent version of the EOS_InventoryItem_GetValueType API. */
#define EOS_INVENTORYITEM_GETVALUETYPE_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetValueType function.
 */
EOS_STRUCT(EOS_InventoryItem_GetValueTypeOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETVALUETYPE_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_GetValueAsString API. */
#define EOS_INVENTORYITEM_GETVALUEASSTRING_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetValueAsString function.
 */
EOS_STRUCT(EOS_InventoryItem_GetValueAsStringOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETVALUEASSTRING_API_LATEST. */
	int32_t ApiVersion;
));


/** The most recent version of the EOS_InventoryItem_GetValueAsString API. */
#define EOS_INVENTORYITEM_GETVALUEASJSONSTRING_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetValueAsJsonString function.
 */
EOS_STRUCT(EOS_InventoryItem_GetValueAsJsonStringOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETVALUEASJSONSTRING_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_GetValueAsBool API. */
#define EOS_INVENTORYITEM_GETVALUEASBOOL_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetValueAsBool function.
 */
EOS_STRUCT(EOS_InventoryItem_GetValueAsBoolOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETVALUEASBOOL_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_GetValueAsInt64 API. */
#define EOS_INVENTORYITEM_GETVALUEASINT64_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetValueAsInt64 function.
 */
EOS_STRUCT(EOS_InventoryItem_GetValueAsInt64Options, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETVALUEASINT64_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_GetValueAsDouble API. */
#define EOS_INVENTORYITEM_GETVALUEASDOUBLE_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetValueAsDouble function.
 */
EOS_STRUCT(EOS_InventoryItem_GetValueAsDoubleOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETVALUEASDOUBLE_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_GetValueAsTimestamp API. */
#define EOS_INVENTORYITEM_GETVALUEASTIMESTAMP_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetValueAsTimestamp function.
 */
EOS_STRUCT(EOS_InventoryItem_GetValueAsTimestampOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETVALUEASTIMESTAMP_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_GetInt64SetValueByIndex API. */
#define EOS_INVENTORYITEM_GETINT64SETVALUEBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetInt64SetValueByIndex function.
 */
EOS_STRUCT(EOS_InventoryItem_GetInt64SetValueByIndexOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETINT64SETVALUEBYINDEX_API_LATEST. */
	int32_t ApiVersion;
	/** Index of the set value to retrieve. */
	uint32_t SetValueIndex;
));

/** The most recent version of the EOS_InventoryItem_GetDoubleSetValueByIndex API. */
#define EOS_INVENTORYITEM_GETDOUBLESETVALUEBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetDoubleSetValueByIndex function.
 */
EOS_STRUCT(EOS_InventoryItem_GetDoubleSetValueByIndexOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETDOUBLESETVALUEBYINDEX_API_LATEST. */
	int32_t ApiVersion;
	/** Index of the set value to retrieve. */
	uint32_t SetValueIndex;
));

/** The most recent version of the EOS_InventoryItem_GetStringSetValueByIndex API. */
#define EOS_INVENTORYITEM_GETSTRINGSETVALUEBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetStringSetValueByIndex function.
 */
EOS_STRUCT(EOS_InventoryItem_GetStringSetValueByIndexOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETSTRINGSETVALUEBYINDEX_API_LATEST. */
	int32_t ApiVersion;
	/** Index of the set value to retrieve. */
	uint32_t SetValueIndex;
));

/** The most recent version of the EOS_InventoryItem_GetValueCount API. */
#define EOS_INVENTORYITEM_GETVALUECOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetValueCount function.
 */
EOS_STRUCT(EOS_InventoryItem_GetValueCountOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETVALUECOUNT_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_SetValueAsString API. */
#define EOS_INVENTORYITEM_SETVALUEASSTRING_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetValueAsString function.
 */
EOS_STRUCT(EOS_InventoryItem_SetValueAsStringOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETVALUEASSTRING_API_LATEST. */
	int32_t ApiVersion;
	/** String value to set for inventory item. */
	const char* Value;
));

/** The most recent version of the EOS_InventoryItem_SetValueAsJson API. */
#define EOS_INVENTORYITEM_SETVALUEASJSON_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetValueAsJson function.
 */
EOS_STRUCT(EOS_InventoryItem_SetValueAsJsonOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETVALUEASJSON_API_LATEST. */
	int32_t ApiVersion;
	/** String value to set for inventory item. */
	const char* Value;
));

/** The most recent version of the EOS_InventoryItem_SetValueAsBool API. */
#define EOS_INVENTORYITEM_SETVALUEASBOOL_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetValueAsBool function.
 */
EOS_STRUCT(EOS_InventoryItem_SetValueAsBoolOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETVALUEASBOOL_API_LATEST. */
	int32_t ApiVersion;
	/** Boolean value to set for inventory item. */
	EOS_Bool Value;
));

/** The most recent version of the EOS_InventoryItem_SetValueAsInt64 API. */
#define EOS_INVENTORYITEM_SETVALUEASINT64_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetValueAsInt64 function.
 */
EOS_STRUCT(EOS_InventoryItem_SetValueAsInt64Options, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETVALUEASINT64_API_LATEST. */
	int32_t ApiVersion;
	/** 64-bit Integer value to set for inventory item. */
	int64_t Value;
));

/** The most recent version of the EOS_InventoryItem_SetValueAsDouble API. */
#define EOS_INVENTORYITEM_SETVALUEASDOUBLE_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetValueAsDouble function.
 */
EOS_STRUCT(EOS_InventoryItem_SetValueAsDoubleOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETVALUEASDOUBLE_API_LATEST. */
	int32_t ApiVersion;
	/** Double value to set for inventory item. */
	double Value;
));

/** The most recent version of the EOS_InventoryItem_SetValueAsTimestamp API. */
#define EOS_INVENTORYITEM_SETVALUEASTIMESTAMP_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetValueAsTimestamp function.
 */
EOS_STRUCT(EOS_InventoryItem_SetValueAsTimestampOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETVALUEASTIMESTAMP_API_LATEST. */
	int32_t ApiVersion;
	/** POSIX Timestamp value to set for inventory item. */
	int64_t Value;
));

/** The most recent version of the EOS_InventoryItem_SetValueAsInt64Set API. */
#define EOS_INVENTORYITEM_SETVALUEASINT64SET_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetValueAsInt64Set function.
 */
EOS_STRUCT(EOS_InventoryItem_SetValueAsInt64SetOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETVALUEASINT64SET_API_LATEST. */
	int32_t ApiVersion;
	/** Array of 64-bit Integer values to set for inventory item. */
	const int64_t* Value;
	/** Number of values in the set. */
	uint32_t ValueCount;
));

/** The most recent version of the EOS_InventoryItem_SetValueAsDoubleSet API. */
#define EOS_INVENTORYITEM_SETVALUEASDOUBLESET_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetValueAsDoubleSet function.
 */
EOS_STRUCT(EOS_InventoryItem_SetValueAsDoubleSetOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETVALUEASDOUBLESET_API_LATEST. */
	int32_t ApiVersion;
	/** Array of Double values to set for inventory item. */
	const double* Value;
	/** Number of values in the set. */
	uint32_t ValueCount;
));

/** The most recent version of the EOS_InventoryItem_SetValueAsStringSet API. */
#define EOS_INVENTORYITEM_SETVALUEASSTRINGSET_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetValueAsStringSet function.
 */
EOS_STRUCT(EOS_InventoryItem_SetValueAsStringSetOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETVALUEASSTRINGSET_API_LATEST. */
	int32_t ApiVersion;
	/** Array of String values to set for inventory item. */
	const char** Value;
	/** Number of values in the set. */
	uint32_t ValueCount;
));

/** The most recent version of the EOS_InventoryItem_SetValueAsUndefined API. */
#define EOS_INVENTORYITEM_SETVALUEASUNDEFINED_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_SetValueAsUndefined function.
 */
EOS_STRUCT(EOS_InventoryItem_SetValueAsUndefinedOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_SETVALUEASUNDEFINED_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_CompareValueAsString API. */
#define EOS_INVENTORYITEM_COMPAREVALUEASSTRING_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CompareValueAsString function.
 */
EOS_STRUCT(EOS_InventoryItem_CompareValueAsStringOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_COMPAREVALUEASSTRING_API_LATEST. */
	int32_t ApiVersion;
	/** String value to compare with value currently set for inventory item. */
	const char* Value;
));

/** The most recent version of the EOS_InventoryItem_CompareValueAsJson API. */
#define EOS_INVENTORYITEM_COMPAREVALUEASJSON_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CompareValueAsJson function.
 */
EOS_STRUCT(EOS_InventoryItem_CompareValueAsJsonOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_COMPAREVALUEASJSON_API_LATEST. */
	int32_t ApiVersion;
	/** String value to compare with value currently set for inventory item. */
	const char* Value;
));

/** The most recent version of the EOS_InventoryItem_CompareValueAsBool API. */
#define EOS_INVENTORYITEM_COMPAREVALUEASBOOL_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CompareValueAsBool function.
 */
EOS_STRUCT(EOS_InventoryItem_CompareValueAsBoolOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_COMPAREVALUEASBOOL_API_LATEST. */
	int32_t ApiVersion;
	/** Boolean value to compare with value currently set for inventory item. */
	EOS_Bool Value;
));

/** The most recent version of the EOS_InventoryItem_CompareValueAsInt64 API. */
#define EOS_INVENTORYITEM_COMPAREVALUEASINT64_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CompareValueAsInt64 function.
 */
EOS_STRUCT(EOS_InventoryItem_CompareValueAsInt64Options, (
	/** API Version: Set this to EOS_INVENTORYITEM_COMPAREVALUEASINT64_API_LATEST. */
	int32_t ApiVersion;
	/** 64-bit Integer value to compare with value currently set for inventory item. */
	int64_t Value;
));

/** The most recent version of the EOS_InventoryItem_CompareValueAsDouble API. */
#define EOS_INVENTORYITEM_COMPAREVALUEASDOUBLE_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CompareValueAsDouble function.
 */
EOS_STRUCT(EOS_InventoryItem_CompareValueAsDoubleOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_COMPAREVALUEASDOUBLE_API_LATEST. */
	int32_t ApiVersion;
	/** Double value to compare with value currently set for inventory item. */
	double Value;
));

/** The most recent version of the EOS_InventoryItem_CompareValueAsTimestamp API. */
#define EOS_INVENTORYITEM_COMPAREVALUEASTIMESTAMP_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CompareValueAsTimestamp function.
 */
EOS_STRUCT(EOS_InventoryItem_CompareValueAsTimestampOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_COMPAREVALUEASTIMESTAMP_API_LATEST. */
	int32_t ApiVersion;
	/** POSIX Timestamp value to compare with value currently set for inventory item. */
	int64_t Value;
));

/** The most recent version of the EOS_InventoryItem_CompareValueAsInt64Set API. */
#define EOS_INVENTORYITEM_COMPAREVALUEASINT64SET_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CompareValueAsInt64Set function.
 */
EOS_STRUCT(EOS_InventoryItem_CompareValueAsInt64SetOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_COMPAREVALUEASINT64SET_API_LATEST. */
	int32_t ApiVersion;
	/** Array of 64-bit Integer values to compare with value currently set for inventory item. */
	const int64_t* Value;
	/** Number of values in the set. */
	uint32_t ValueCount;
));

/** The most recent version of the EOS_InventoryItem_CompareValueAsDoubleSet API. */
#define EOS_INVENTORYITEM_COMPAREVALUEASDOUBLESET_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CompareValueAsDoubleSet function.
 */
EOS_STRUCT(EOS_InventoryItem_CompareValueAsDoubleSetOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_COMPAREVALUEASDOUBLESET_API_LATEST. */
	int32_t ApiVersion;
	/** Array of Double values to compare with value currently set for inventory item. */
	const double* Value;
	/** Number of values in the set. */
	uint32_t ValueCount;
));

/** The most recent version of the EOS_InventoryItem_CompareValueAsStringSet API. */
#define EOS_INVENTORYITEM_COMPAREVALUEASSTRINGSET_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CompareValueAsStringSet function.
 */
EOS_STRUCT(EOS_InventoryItem_CompareValueAsStringSetOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_COMPAREVALUEASSTRINGSET_API_LATEST. */
	int32_t ApiVersion;
	/** Array of String values to compare with value currently set for inventory item. */
	const char** Value;
	/** Number of values in the set. */
	uint32_t ValueCount;
));

/** The most recent version of the EOS_InventoryItem_CreateSubItem API. */
#define EOS_INVENTORYITEM_CREATESUBITEM_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CreateSubItem function.
 */
EOS_STRUCT(EOS_InventoryItem_CreateSubItemOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_CREATESUBITEM_API_LATEST. */
	int32_t ApiVersion;
	/** Key. */
	const char* Key;
));

/** The most recent version of the EOS_InventoryItem_CreateSubItemFromPath API. */
#define EOS_INVENTORYITEM_CREATESUBITEMFROMPATH_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CreateSubItemFromPath function.
 */
EOS_STRUCT(EOS_InventoryItem_CreateSubItemFromPathOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_CREATESUBITEMFROMPATH_API_LATEST. */
	int32_t ApiVersion;
	/** Path. */
	const char* Path;
));

/** The most recent version of the EOS_InventoryItem_GetSubItem API. */
#define EOS_INVENTORYITEM_GETSUBITEM_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetSubItem function.
 */
EOS_STRUCT(EOS_InventoryItem_GetSubItemOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETSUBITEM_API_LATEST. */
	int32_t ApiVersion;
	/** Key. */
	const char* Key;
));

/** The most recent version of the EOS_InventoryItem_GetSubItemByIndex API. */
#define EOS_INVENTORYITEM_GETSUBITEMBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetSubItemByIndex function.
 */
EOS_STRUCT(EOS_InventoryItem_GetSubItemByIndexOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETSUBITEMBYINDEX_API_LATEST. */
	int32_t ApiVersion;
	/** Index of the sub item to retrieve. */
	uint32_t SubItemIndex;
));

/** The most recent version of the EOS_InventoryItem_GetSubItemsCount API. */
#define EOS_INVENTORYITEM_GETSUBITEMSCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetSubItemsCount function.
 */
EOS_STRUCT(EOS_InventoryItem_GetSubItemsCountOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETSUBITEMSCOUNT_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_GetSubItem API. */
#define EOS_INVENTORYITEM_DELETESUBITEM_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_DeleteSubItem function.
 */
EOS_STRUCT(EOS_InventoryItem_DeleteSubItemOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_DELETESUBITEM_API_LATEST. */
	int32_t ApiVersion;
	/** Key. */
	const char* Key;
));

/** The most recent version of the EOS_InventoryItem_GetFullPath API. */
#define EOS_INVENTORYITEM_GETFULLPATH_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_GetFullPath function.
 */
EOS_STRUCT(EOS_InventoryItem_GetFullPathOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_GETFULLPATH_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_InventoryItem_CopyHandle API. */
#define EOS_INVENTORYITEM_COPYHANDLE_API_LATEST 1

/**
 * Input parameters for the EOS_InventoryItem_CopyHandle function.
 */
EOS_STRUCT(EOS_InventoryItem_CopyHandleOptions, (
	/** API Version: Set this to EOS_INVENTORYITEM_COPYHANDLE_API_LATEST. */
	int32_t ApiVersion;
));

#pragma pack(pop)
