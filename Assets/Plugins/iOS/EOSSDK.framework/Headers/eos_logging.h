// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"

/**
 * The Logging Interface grants access to log output coming from the SDK at various levels of detail.
 * Unlike other interfaces, the Logging Interface does not require a handle from the Platform Interface,
 * as it functions entirely on the local system.
 */

#pragma pack(push, 8)

/**
 * Logging levels. When a log message is output, it has an associated log level.
 * Messages will only be sent to the callback function if the message's associated log level is less than or equal to the configured log level for that category.
 *
 * @see EOS_Logging_SetCallback
 * @see EOS_Logging_SetLogLevel
 */
EOS_ENUM(EOS_ELogLevel,
	EOS_LOG_Off = 0,
	EOS_LOG_Fatal = 100,
	EOS_LOG_Error = 200,
	EOS_LOG_Warning = 300,
	EOS_LOG_Info = 400,
	EOS_LOG_Verbose = 500,
	EOS_LOG_VeryVerbose = 600
);

/**
 * Logging Categories
 */

#define PROCESS_CATEGORY(CategoryName, EOSCategoryLabel, EOSCategoryValue) EOSCategoryLabel = EOSCategoryValue,
#define PROCESS_CATEGORY_LAST(CategoryName, EOSCategoryLabel, EOSCategoryValue) EOSCategoryLabel = EOSCategoryValue

EOS_ENUM_START(EOS_ELogCategory)
#include "eos_logging_categories.h"
EOS_ENUM_END(EOS_ELogCategory);

#undef PROCESS_CATEGORY
#undef PROCESS_CATEGORY_LAST

/** A structure representing a log message */
EOS_STRUCT(EOS_LogMessage, (
	/** A string representation of the log message category, encoded in UTF-8. Only valid during the life of the callback, so copy the string if you need it later. */
	const char* Category;
	/** The log message, encoded in UTF-8. Only valid during the life of the callback, so copy the string if you need it later. */
	const char* Message;
	/** The log level associated with the message */
	EOS_ELogLevel Level;
));

/**
 * Function prototype definition for functions that receive log messages.
 *
 * @param Message A EOS_LogMessage containing the log category, log level, and message.
 * @see EOS_LogMessage
 */
EOS_EXTERN_C typedef void (EOS_CALL * EOS_LogMessageFunc)(const EOS_LogMessage* Message);

/**
 * Set the callback function to use for SDK log messages. Any previously set callback will no longer be called.
 *
 * @param Callback the function to call when the SDK logs messages
 * @return EOS_Success is returned if the callback will be used for future log messages.
 *         EOS_NotConfigured is returned if the SDK has not yet been initialized, or if it has been shut down
 *
 * @see EOS_Initialize
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_Logging_SetCallback(EOS_LogMessageFunc Callback);

/**
 * Set the logging level for the specified logging category. By default all log categories will callback for Warnings, Errors, and Fatals.
 *
 * @param LogCategory the specific log category to configure. Use EOS_LC_ALL_CATEGORIES to configure all categories simultaneously to the same log level.
 * @param LogLevel the log level to use for the log category
 *
 * @return EOS_Success is returned if the log levels are now in use.
 *         EOS_NotConfigured is returned if the SDK has not yet been initialized, or if it has been shut down.
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_Logging_SetLogLevel(EOS_ELogCategory LogCategory, EOS_ELogLevel LogLevel);

#pragma pack(pop)
