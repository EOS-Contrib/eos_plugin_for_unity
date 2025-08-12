// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

/** Handle to the inventory interface */
EOS_EXTERN_C typedef struct EOS_WorldInventoryHandle* EOS_HWorldInventory;


/** The most recent version of the EOS_WorldInventory_LayerInfo struct. */
#define EOS_WORLDINVENTORY_LAYERINFO_API_LATEST 1

/**
 * The World Layer Inventory info.
 */
EOS_STRUCT(EOS_WorldInventory_LayerInfo, (
    /** API Version: Set this to EOS_WORLDINVENTORY_LAYERINFO_API_LATEST. */
	int32_t ApiVersion;
    /** The World ID for this inventory. */
	const char* WorldId;
	/** The Layer ID for this inventory. */
	const char* LayerId;
));

/** The most recent version of the EOS_WorldInventory_TileInfo struct. */
#define EOS_WORLDINVENTORY_TILEINFO_API_LATEST 1

/**
 * The World Tile Inventory info.
 */
EOS_STRUCT(EOS_WorldInventory_TileInfo, (
	/** API Version: Set this to EOS_WORLDINVENTORY_TILEINFO_API_LATEST. */
	int32_t ApiVersion;
    /** The World ID for this inventory. */
    const char* WorldId;
    /** The Layer ID for this inventory. */
    const char* LayerId;
    /** The Tile ID for this inventory. */
    const char* TileId;
));

/** The most recent version of the EOS_WorldInventory_PlayerInfo struct. */
#define EOS_WORLDINVENTORY_PLAYERINFO_API_LATEST 1

/**
 * The World Player Inventory info.
 */
EOS_STRUCT(EOS_WorldInventory_PlayerInfo, (
    /** API Version: Set this to EOS_WORLDINVENTORY_PLAYERINFO_API_LATEST. */
    int32_t ApiVersion;
    /** The World ID for this inventory. */
    const char* WorldId;
    /** The Player ID for this inventory. */
    EOS_ProductUserId PlayerId;
	/** The Inventory Name for this inventory. */
	const char* InventoryName;
));

/**
 * The supported World Inventory Types.
 */
EOS_ENUM(EOS_EWorldInventoryType,
	/** The World Layer Inventory Type. */
	EOS_WIT_LAYER = 0,
	/** The World Tile Inventory Type. */
	EOS_WIT_TILE = 1,
	/** The World Player Inventory Type. */
	EOS_WIT_PLAYER = 2
);

/** The most recent version of the EOS_WorldInventory_InventoryInfo struct. */
#define EOS_WORLDINVENTORY_INVENTORYINFO_API_LATEST 1

/**
 * The World Inventory info.
 */
EOS_STRUCT(EOS_WorldInventory_InventoryInfo, (
    /** API Version: Set this to EOS_WORLDINVENTORY_INVENTORYINFO_API_LATEST. */
    int32_t ApiVersion;
    /** The World Inventory type. */
    EOS_EWorldInventoryType InventoryType;
    /** The inventory information. */
	union
	{
		/** The World Layer Inventory info. */
		const EOS_WorldInventory_LayerInfo* AsLayer;
		/** The World Tile Inventory info. */
		const EOS_WorldInventory_TileInfo* AsTile;
		/** The World Player Inventory info. */
		const EOS_WorldInventory_PlayerInfo* AsPlayer;
	} Inventory;
));

/** The most recent version of the EOS_WorldInventory_Open API. */
#define EOS_WORLDINVENTORY_OPEN_API_LATEST 1

/**
 * Input parameters for the EOS_WorldInventory_Open function.
 */
EOS_STRUCT(EOS_WorldInventory_OpenOptions, (
	/** API Version: Set this to EOS_WORLDINVENTORY_OPEN_API_LATEST. */
	int32_t ApiVersion;
	/** World ID for the world whose inventory items are being queried for. */
	const char* WorldId;
	/** Version ID for the world whose inventory items are being queried for. */
	const char* VersionId;
	/** Layer ID for the world whose inventory items are being queried for. */
	const char* LayerId;
	/** Prefix for inventory to open. Only inventory items beneath this prefix path will be included in the queried items. Set to nullptr to query the full inventory. */
	const char* Prefix;
	/**
	 * Session Lock ID for Session Locking.
	 * Only used for Dedicated Server when writing the updated inventory items.
	 * Note: Using the same Session Lock ID for all EOS_WorldInventory_Open calls is recommended for optimal updates.
	 */
	const char* SessionLockId;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for opening inventory and querying for world inventory items request.
 */
EOS_STRUCT(EOS_WorldInventory_OnOpenCompleteCallbackInfo, (
	/**
	 * The result code for the operation.
	 * - EOS_Success:                 The request was successful.
	 * - EOS_MissingPermissions:      Server permission is required.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 * - EOS_Inventory_InvalidLockId: The inventory failed to be locked.
	 * - EOS_Inventory_AlreadyOpen:   The inventory has already been opened previously.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_WorldInventory_Open. */
	void* ClientData;
	/** A pointer to the root handle of the inventory items (on Success). Null on failure. */
	EOS_HInventoryItem InventoryItemHandle;
	/** World ID for the world whose inventory was opened. */
	const char* WorldId;
	/** Version ID for the world whose inventory was opened. */
	const char* VersionId;
	/** Layer ID for the world whose inventory was opened. */
	const char* LayerId;
	/** The optional binary data associated with the inventory. This value is only valid within the callback and must be copied. */
	const void* BinaryData;
	/** The size of the binary data in bytes. */
	uint32_t BinaryDataSize;
));

/**
 * Function prototype definition for callbacks passed to EOS_WorldInventory_Open.
 * @param Data A EOS_WorldInventory_OnOpenCompleteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_WorldInventory_OnOpenCompleteCallback, const EOS_WorldInventory_OnOpenCompleteCallbackInfo* Data);

/** The most recent version of the EOS_WorldInventory_OpenTile API. */
#define EOS_WORLDINVENTORY_OPENTILE_API_LATEST 1

/**
 * Input parameters for the EOS_WorldInventory_OpenTile function.
 */
EOS_STRUCT(EOS_WorldInventory_OpenTileOptions, (
	/** API Version: Set this to EOS_WORLDINVENTORY_OPENTILE_API_LATEST. */
	int32_t ApiVersion;
	/** World ID for the world whose inventory items are being queried for. */
	const char* WorldId;
	/** Version ID for the world whose inventory items are being queried for. */
	const char* VersionId;
	/** Layer ID for the world tile whose inventory items are being queried for. */
	const char* LayerId;
	/** Tile ID for the world tile whose inventory items are being queried for. */
	const char* TileId;
	/** Prefix for inventory to open. Only inventory items beneath this prefix path will be included in the queried items. Set to nullptr to query the full inventory. */
	const char* Prefix;
	/**
	 * Session Lock ID for Session Locking.
	 * Only used for Dedicated Server when writing the updated inventory items.
	 * Note: Using the same Session Lock ID for all EOS_WorldInventory_OpenTile calls is recommended for optimal updates.
	 */
	const char* SessionLockId;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for opening inventory and querying for world tile inventory items request.
 */
EOS_STRUCT(EOS_WorldInventory_OnOpenTileCompleteCallbackInfo, (
	/**
	 * The result code for the operation.
	 * - EOS_Success:                 The request was successful.
	 * - EOS_MissingPermissions:      Server permission is required.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 * - EOS_Inventory_InvalidLockId: The inventory failed to be locked.
	 * - EOS_Inventory_AlreadyOpen:   The inventory has already been opened previously.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_WorldInventory_OpenTile. */
	void* ClientData;
	/** A pointer to the root handle of the inventory items (on Success). Null on failure. */
	EOS_HInventoryItem InventoryItemHandle;
	/** World ID for the world whose inventory was opened. */
	const char* WorldId;
	/** Version ID for the world whose inventory was opened. */
	const char* VersionId;
	/** Layer ID for the world tile whose inventory was opened. */
	const char* LayerId;
	/** Tile ID for the world tile whose inventory was opened. */
	const char* TileId;
	/** The optional binary data associated with the inventory. This value is only valid within the callback and must be copied. */
	const void* BinaryData;
	/** The size of the binary data in bytes. */
	uint32_t BinaryDataSize;
));

/**
 * Function prototype definition for callbacks passed to EOS_WorldInventory_OpenTile.
 * @param Data A EOS_WorldInventory_OnOpenTileCompleteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_WorldInventory_OnOpenTileCompleteCallback, const EOS_WorldInventory_OnOpenTileCompleteCallbackInfo* Data);

/** The most recent version of the EOS_WorldInventory_OpenPlayer API. */
#define EOS_WORLDINVENTORY_OPENPLAYER_API_LATEST 1

/**
 * Input parameters for the EOS_WorldInventory_OpenPlayer function.
 */
EOS_STRUCT(EOS_WorldInventory_OpenPlayerOptions, (
	/** API Version: Set this to EOS_WORLDINVENTORY_OPENPLAYER_API_LATEST. */
	int32_t ApiVersion;
	/** World ID for the world whose inventory items are being queried for. */
	const char* WorldId;
	/** The Optional Version ID of the inventory being queried for. */
	const char* VersionId;
	/** Product User ID for the user whose inventory items are being queried for. */
	EOS_ProductUserId TargetUserId;
	/** Name of inventory to open. */
	const char* InventoryName;
	/** Prefix for inventory to open. Only inventory items beneath this prefix path will be included in the queried items. Set to nullptr to query the full inventory. */
	const char* Prefix;
	/**
	 * Session Lock ID for Session Locking.
	 * Only used for Dedicated Server when writing the updated inventory items.
	 * Note: Using the same Session Lock ID for all EOS_WorldInventory_OpenPlayer calls is recommended for optimal updates.
	 */
	const char* SessionLockId;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for opening inventory and querying for world player inventory items request.
 */
EOS_STRUCT(EOS_WorldInventory_OnOpenPlayerCompleteCallbackInfo, (
	/**
	 * The result code for the operation.
	 * - EOS_Success:                 The request was successful.
	 * - EOS_MissingPermissions:      Server permission is required.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_InvalidUser:             If required (not a trusted server), the local user does not exist or is not authenticated.
	 * - EOS_InvalidAuth:             If required (not a trusted server), the local user is not logged in.
	 * - EOS_Invalid_ProductUserID:   An invalid target user is specified.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 * - EOS_Inventory_InvalidLockId: The inventory failed to be locked.
	 * - EOS_Inventory_AlreadyOpen:   The inventory has already been opened previously.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_WorldInventory_OpenPlayer. */
	void* ClientData;
	/** A pointer to the root handle of the inventory items (on Success). Null on failure. */
	EOS_HInventoryItem InventoryItemHandle;
	/** World ID for the world whose inventory was opened. */
	const char* WorldId;
	/** Version ID for the world whose inventory was opened. */
	const char* VersionId;
	/** Product User ID for the user whose inventory was opened. */
	EOS_ProductUserId TargetUserId;
	/** Name of inventory that was opened. */
	const char* InventoryName;
	/** The optional binary data associated with the inventory. This value is only valid within the callback and must be copied. */
	const void* BinaryData;
	/** The size of the binary data in bytes. */
	uint32_t BinaryDataSize;
));

/**
 * Function prototype definition for callbacks passed to EOS_WorldInventory_OpenPlayer.
 * @param Data A EOS_WorldInventory_OnOpenPlayerCompleteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_WorldInventory_OnOpenPlayerCompleteCallback, const EOS_WorldInventory_OnOpenPlayerCompleteCallbackInfo* Data);

/** The most recent version of the EOS_WorldInventory_Close API. */
#define EOS_WORLDINVENTORY_CLOSE_API_LATEST 1

/**
 * Input parameters for the EOS_WorldInventory_Close function.
 */
EOS_STRUCT(EOS_WorldInventory_CloseOptions, (
	/** API Version: Set this to EOS_WORLDINVENTORY_CLOSE_API_LATEST. */
	int32_t ApiVersion;
	/** A pointer to the root handle of the inventory items. */
	EOS_HInventoryItem InventoryItemHandle;
	/** A flag indicating if any pending changes should be written prior to close. */
	EOS_Bool bWritePendingChangesBeforeClose;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for closing the inventory.
 */
EOS_STRUCT(EOS_WorldInventory_OnCloseCompleteCallbackInfo, (
	/**
	 * The result code for the operation.
	 * - EOS_Success:                 The request was successful.
	 * - EOS_MissingPermissions:      Server permission is required.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 * - EOS_Inventory_InvalidLockId: The inventory failed to be unlocked.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_WorldInventory_Close. */
	void* ClientData;
	/** World ID for the world whose inventory was closed. */
	const char* WorldId;
	/** Version ID for the world whose inventory was closed. */
	const char* VersionId;
));

/**
 * Function prototype definition for callbacks passed to EOS_WorldInventory_Close.
 * @param Data A EOS_WorldInventory_OnCloseCompleteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_WorldInventory_OnCloseCompleteCallback, const EOS_WorldInventory_OnCloseCompleteCallbackInfo* Data);

/** Timestamp value representing an undefined time for ExpirationTime for a EOS_WorldInventory_LedgerEntry */
#define EOS_WORLDINVENTORY_LEDGERENTRY_EXPIRATION_TIME_UNDEFINED -1

/** The most recent version of the EOS_WorldInventory_LedgerEntry struct. */
#define EOS_WORLDINVENTORY_LEDGERENTRY_API_LATEST 1

/**
 * Contains information about a single Ledger Entry.
 */
EOS_STRUCT(EOS_WorldInventory_LedgerEntry, (
	/** API Version: Set this to EOS_WORLDINVENTORY_LEDGERENTRY_API_LATEST. */
	int32_t ApiVersion;
	/** World ID for this ledger entry. */
	const char* WorldId;
	/** Version ID for this ledger entry. */
	const char* VersionId;
	/** ID for the ledger entry. */
	const char* Id;
	/** The ledger name. */
	const char* LedgerName;
	/** The POSIX timestamp when the ledger entry was created. */
	int64_t CreateTime;
	/**
	 * The POSIX timestamp when the ledger entry should expire.
	 * If the expiration time is not defined the default will be used, this value should be set as EOS_WORLDINVENTORY_LEDGERENTRY_EXPIRATION_TIME_UNDEFINED.
	 */
	int64_t ExpirationTime;
	/** The data associated with the ledger entry. */
	const char* Data;
));

/** The most recent version of the EOS_WorldInventory_InventoryWriteInfo structure. */
#define EOS_WORLDINVENTORY_INVENTORYWRITEINFO_API_LATEST 1

/**
 * Contains information needed to write inventories to the backend.
 */
EOS_STRUCT(EOS_WorldInventory_InventoryWriteInfo, (
	/** API Version: Set this to EOS_WORLDINVENTORY_INVENTORYWRITEINFO_API_LATEST. */
	int32_t ApiVersion;
	/** The root inventory item handle to be written. */
	EOS_HInventoryItem InventoryHandle;
	/** The optional binary data associated with the inventory.*/
	const void* BinaryData;
	/** The size of the binary data in bytes. */
	uint32_t BinaryDataSize;
));

/** The most recent version of the EOS_WorldInventory_WriteTransaction API. */
#define EOS_WORLDINVENTORY_WRITETRANSACTION_API_LATEST 1

/**
 * Input parameters for the EOS_WorldInventory_WriteTransaction function.
 */
EOS_STRUCT(EOS_WorldInventory_WriteTransactionOptions, (
	/** API Version: Set this to EOS_WORLDINVENTORY_WRITETRANSACTION_API_LATEST. */
	int32_t ApiVersion;
	/** Transaction ID for the transaction whose inventory items are being written. */
	const char* TransactionId;
	/** The number of inventories to be included in this transaction. */
	uint32_t InventoryWriteInfoListCount;
	/** Array of inventory write info structs to be included in the transaction. */
	const EOS_WorldInventory_InventoryWriteInfo* InventoryWriteInfoList;
	/** Ledger entry. */
	const EOS_WorldInventory_LedgerEntry* LedgerEntry;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for updating the inventory.
 */
EOS_STRUCT(EOS_WorldInventory_OnWriteTransactionCompleteCallbackInfo, (
	/**
	 * The result code for the operation.
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
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_WorldInventory_WriteTransaction. */
	void* ClientData;
	/** Transaction ID for the transaction where inventory items were written. */
	const char* TransactionId;
));

/**
 * Function prototype definition for callbacks passed to EOS_WorldInventory_WriteTransaction.
 * @param Data A EOS_WorldInventory_OnWriteTransactionCompleteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_WorldInventory_OnWriteTransactionCompleteCallback, const EOS_WorldInventory_OnWriteTransactionCompleteCallbackInfo* Data);

/** The most recent version of the EOS_WorldInventory_VersionMetadata struct. */
#define EOS_WORLDINVENTORY_VERSIONMETADATA_API_LATEST 1

/**
 * World Inventory Version Metadata.
 */
EOS_STRUCT(EOS_WorldInventory_VersionMetadata, (
	/** API Version: Set this to EOS_WORLDINVENTORY_VERSIONMETADATA_API_LATEST. */
	int32_t ApiVersion;
	/** Version ID for the queried inventory. */
	const char* VersionId;
));

/** The most recent version of the EOS_WorldInventory_QueryVersionMetadata API. */
#define EOS_WORLDINVENTORY_QUERYVERSIONMETADATA_API_LATEST 2

/**
 * Input parameters for the EOS_WorldInventory_QueryVersionMetadata function.
 */
EOS_STRUCT(EOS_WorldInventory_QueryVersionMetadataOptions, (
	/** API Version: Set this to EOS_WORLDINVENTORY_QUERYVERSIONMETADATA_API_LATEST. */
    int32_t ApiVersion;
    /** World ID to query version data for. Deprecated in ApiVersion: 2, use the InventoryInfo field instead. */
	const char* WorldId;
    /** Layer ID to query version data for. Deprecated in ApiVersion: 2, use the InventoryInfo field instead. */
    const char* LayerId;
    /** A reserved field that should always be nulled. */
    void* Reserved;
	/**
	 * The Inventory to query the version metadata for.
	 * Currently supported types are EOS_WIT_LAYER or EOS_WIT_PLAYER.
	 */
	const EOS_WorldInventory_InventoryInfo* InventoryInfo;
));

/**
 * Data containing the result information for querying inventory version metadata.
 */
EOS_STRUCT(EOS_WorldInventory_OnQueryVersionMetadataCompleteCallbackInfo, (
	/**
	 * The result code for the operation.
	 * - EOS_Success:                 The request was successful.
	 * - EOS_MissingPermissions:      Server permission is required.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_Invalid_ProductUserID:   An invalid playerId in inventory info was specified.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_WorldInventory_QueryVersionMetadata. */
	void* ClientData;
	/** Deprecated: World ID for the world whose version data was queried. Will be set to NULL when ApiVersion 2 is used. */
	const char* WorldId;
	/** Deprecated: Layer ID for the world tile whose version data was queried. Will be set to NULL when ApiVersion 2 is used. */
	const char* LayerId;
	/**
	 * The Inventory whose version data was queried.
	 * Note: The contents will be valid only within the scope of the EOS_WorldInventory_OnQueryVersionMetadataCompleteCallback callback. 
	 */
	const EOS_WorldInventory_InventoryInfo* InventoryInfo;
));

/**
 * Function prototype definition for callbacks passed to EOS_WorldInventory_QueryVersionMetadata.
 * @param Data A EOS_WorldInventory_OnQueryVersionMetadataCompleteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_WorldInventory_OnQueryVersionMetadataCompleteCallback, const EOS_WorldInventory_OnQueryVersionMetadataCompleteCallbackInfo* Data);

/** The most recent version of the EOS_WorldInventory_GetVersionMetadataCount API. */
#define EOS_WORLDINVENTORY_GETVERSIONMETADATACOUNTOPTIONS_API_LATEST 2

/**
 * Input parameters for the EOS_WorldInventory_GetVersionMetadataCount function.
 */
EOS_STRUCT(EOS_WorldInventory_GetVersionMetadataCountOptions, (
	/** API Version: Set this to EOS_WORLDINVENTORY_GETVERSIONMETADATACOUNTOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/**
	 * The Inventory to get the version metadata count for.
	 */
	const EOS_WorldInventory_InventoryInfo* InventoryInfo;
));

/** The most recent version of the EOS_WorldInventory_CopyVersionMetadataByIndex API. */
#define EOS_WORLDINVENTORY_COPYVERSIONMETADATABYINDEXOPTIONS_API_LATEST 2

/**
 * Input parameters for the EOS_WorldInventory_CopyVersionMetadataByIndex function.
 */
EOS_STRUCT(EOS_WorldInventory_CopyVersionMetadataByIndexOptions, (
	/** API Version: Set this to EOS_WORLDINVENTORY_COPYVERSIONMETADATABYINDEXOPTIONS_API_LATEST. */
	int32_t ApiVersion;
	/** Index of the inventory version metadata to retrieve from the cache. */
	uint32_t MetadataIndex;
	/**
	 * The Inventory to copy the version metadata for.
	 */
	const EOS_WorldInventory_InventoryInfo* InventoryInfo;
));

/**
 * Release the memory associated with an EOS_WorldInventory_VersionMetadata structure. This must be called on
 * data retrieved from EOS_WorldInventory_CopyVersionMetadataByIndex.
 *
 * @param VersionMetadata - The version metadata structure to be released.
 *
 * @see EOS_WorldInventory_VersionMetadata
 * @see EOS_WorldInventory_CopyVersionMetadataByIndex
 */
EOS_DECLARE_FUNC(void) EOS_WorldInventory_VersionMetadata_Release(EOS_WorldInventory_VersionMetadata* VersionMetadata);

/** The most recent version of the EOS_WorldInventory_Write API. */
#define EOS_WORLDINVENTORY_WRITE_API_LATEST 1

/**
 * Input parameters for the EOS_WorldInventory_Write function.
 */
EOS_STRUCT(EOS_WorldInventory_WriteOptions, (
	/** API Version: Set this to EOS_WORLDINVENTORY_WRITE_API_LATEST. */
	int32_t ApiVersion;
	/** A pointer to the root handle of the inventory items. */
	EOS_HInventoryItem InventoryItemHandle;
	/** The optional binary data associated with the inventory. */
	const void* BinaryData;
	/** The size of the binary data in bytes. */
	uint32_t BinaryDataSize;
	/** A reserved field that should always be nulled. */
	void* Reserved;
));

/**
 * Data containing the result information for updating the inventory.
 */
EOS_STRUCT(EOS_WorldInventory_OnWriteCompleteCallbackInfo, (
	/**
	 * The result code for the operation.
	 * - EOS_Success:                 The request was successful.
	 * - EOS_MissingPermissions:      Server permission is required.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 * - EOS_Inventory_InvalidLockId: The inventory failed to be written.
	 * - EOS_NotFound:                An inventory was not found matching a supplied inventory handle.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_WorldInventory_Write. */
	void* ClientData;
));

/**
 * Function prototype definition for callbacks passed to EOS_WorldInventory_Write.
 * @param Data A EOS_WorldInventory_OnWriteCompleteCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_WorldInventory_OnWriteCompleteCallback, const EOS_WorldInventory_OnWriteCompleteCallbackInfo* Data);


#pragma pack(pop)
