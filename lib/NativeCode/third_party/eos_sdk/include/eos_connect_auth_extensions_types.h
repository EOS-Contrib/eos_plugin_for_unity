// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

/** The most recent version of the EOS_Connect_GetUserAuthTokenAsString API. */
#define EOS_CONNECT_GETUSERAUTHTOKENASSTRING_API_LATEST 1

/**
 * Input parameters for the EOS_Connect_GetUserAuthTokenAsString function.
 */
EOS_STRUCT(EOS_Connect_GetUserAuthTokenAsStringOptions, (
	/** API Version: Set this to EOS_CONNECT_GETUSERAUTHTOKENASSTRING_API_LATEST. */
	int32_t ApiVersion;
	/** The Product User ID for which we need the Auth Token for. */
	EOS_ProductUserId LocalUserId;
));

/** The most recent version of the EOS_Connect_GetClientAuthTokenAsString API. */
#define EOS_CONNECT_GETCLIENTAUTHTOKENASSTRING_API_LATEST 1

/**
 * Input parameters for the EOS_Connect_GetClientAuthTokenAsString function.
 */
EOS_STRUCT(EOS_Connect_GetClientAuthTokenAsStringOptions, (
	/** API Version: Set this to EOS_CONNECT_GETCLIENTAUTHTOKENASSTRING_API_LATEST. */
	int32_t ApiVersion;
));

/** The most recent version of the EOS_Connect_AddNotifyClientAuthTokenGenerated API. */
#define EOS_CONNECT_ADDNOTIFYCLIENTAUTHTOKENGENERATED_API_LATEST 1

/**
 * Structure containing information for the Client Auth Token  notification callback.
 */
EOS_STRUCT(EOS_Connect_AddNotifyClientAuthTokenGeneratedOptions, (
	/** API Version: Set this to EOS_CONNECT_ADDNOTIFYCLIENTAUTHTOKENGENERATED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_Connect_AddNotifyClientAuthTokenGenerated function.
 */
EOS_STRUCT(EOS_Connect_ClientAuthTokenGeneratedCallbackInfo, (
	/** Context that was passed into EOS_Connect_AddNotifyClientAuthTokenGenerated. */
	void* ClientData;
	/** EOS_Success if the Client Auth Token was successfully generated, else the error code indicating the failure. */
	EOS_EResult ResultCode;
));

/**
 * Function prototype definition for notifications that come from EOS_Connect_AddNotifyClientAuthTokenGenerated.
 *
 * @param Data A EOS_Connect_ClientAuthTokenGeneratedCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Connect_OnClientAuthTokenGeneratedCallback, const EOS_Connect_ClientAuthTokenGeneratedCallbackInfo* Data);

/** The most recent version of the EOS_Connect_ExchangeSubjectToken struct. */
#define EOS_CONNECT_EXCHANGESUBJECTTOKEN_API_LATEST 1

/**
 * A structure containing Exchange Subject Token details.
 */
EOS_STRUCT(EOS_Connect_ExchangeSubjectToken, (
	/** API Version: Set this to EOS_CONNECT_EXCHANGESUBJECTTOKEN_API_LATEST. */
	int32_t ApiVersion;
	/** The subject token type to be used in the token exchange request. */
	const char* SubjectTokenType;
	/** The verifiable Subject token as a Json Web Token. */
	const char* SubjectJsonWebToken;
));

/** The most recent version of the EOS_Connect_ExchangeTokenFlowConfig struct. */
#define EOS_CONNECT_EXCHANGETOKENFLOWCONFIG_API_LATEST 1

/**
 * Struct containing the exchange token flow configuration.
 */
EOS_STRUCT(EOS_Connect_ExchangeTokenFlowConfig, (
	/** API Version: Set this to EOS_CONNECT_EXCHANGETOKENFLOWCONFIG_API_LATEST. */
	int32_t ApiVersion;
	/** The type of the exchange token being requested. */
	const char* RequestTokenType;
	/** The UTF8 audience value configured for the token exchange flow in the EOS Dev Portal. */
	const char* Audience;
));

EOS_EXTERN_C typedef uint32_t EOS_ConnectExchangeTokenHandle;

/**
 * Enum describing states for the exchange token.
 */
EOS_ENUM(EOS_Connect_EExchangeBearerTokenType,
	/** Enum value indicating whether the connect client token should be used as the bearer auth token. */
	EOS_CEBTT_Client = 0,
	/** Enum value indicating whether the connect user token should be used as the bearer auth token. */
	EOS_CEBTT_User = 1,
	/** Enum value indicating whether the connect exchagne token should be used as the bearer auth token. */
	EOS_CEBTT_Exchange = 2
);

/** The most recent version of the EOS_Connect_ExchangeBearerToken struct. */
#define EOS_CONNECT_EXCHANGEBEARERTOKEN_API_LATEST 1

/**
 * Struct containing the exchange bearer token.
 */
EOS_STRUCT(EOS_Connect_ExchangeBearerToken, (
	/** API Version: Set this to EOS_CONNECT_EXCHANGEBEARERTOKEN_API_LATEST. */
	int32_t ApiVersion;
	/** Type of token to use as the bearer auth in the exchange token request. */
	EOS_Connect_EExchangeBearerTokenType BearerTokenType;
	/** Should be set to a valid user id if bearer token type is EOS_CEBTT_User, else NULL. */
	EOS_ProductUserId BearerUserId;
	/** Should be set to a valid exchange token handle if the bearer token type is EOS_CEBTT_Exchange, else 0. */
	EOS_ConnectExchangeTokenHandle BearerExchangeTokenHandle;
));

/** The most recent version of the EOS_Connect_RequestExchangeToken API. */
#define EOS_CONNECT_REQUESTEXCHANGETOKEN_API_LATEST 1

/**
 * Input parameters to the EOS_Connect_RequestExchangeToken API.
 */
EOS_STRUCT(EOS_Connect_RequestExchangeTokenOptions, (
	/** API Version: Set this to EOS_CONNECT_REQUESTEXCHANGETOKEN_API_LATEST. */
	int32_t ApiVersion;
	/** Pointer to the structure containing the bearer token to be used in the exchange request. */
	const EOS_Connect_ExchangeBearerToken* BearerToken;
	/** Pointer to the structure containing the exchange token flow configuration. */
	const EOS_Connect_ExchangeTokenFlowConfig* ExchangeTokenFlowConfig;
	/** The subject token to use in the exchange request. */
	const EOS_Connect_ExchangeSubjectToken* SubjectToken;
));

/**
 * Output parameters to the EOS_Connect_RequestExchangeToken API.
 */
EOS_STRUCT(EOS_Connect_RequestExchangeTokenCallbackInfo, (
	/**
	 * The result code for the operation.
	 * - EOS_Success:                 The request was successful.
	 * - EOS_IncompatibleVersion:     The API version passed in is incorrect.
	 * - EOS_InvalidParameters:       An options parameter is incorrect.
	 * - EOS_InvalidAuth:             The specified bearer token type has invalid auth.
	 * - EOS_NotFound:                The specified bearer exchange token handle is not found.
	 * - EOS_InvalidUser:             The specified bearer user id is invalid.
	 * - EOS_NotImplemented:          If the specified bearer token type is invalid.
	 * - EOS_TooManyRequests:         There are too many requests in progress at this time.
	 */
	EOS_EResult ResultCode;
	/** Context that was passed into EOS_Connect_RequestExchangeToken. */
	void* ClientData;
	/** The handle to the exchange token if the request exchange token call is successful, else 0. */
	EOS_ConnectExchangeTokenHandle ExchangeTokenHandle;
));

/**
 * Function prototype definition for callbacks passed into EOS_Connect_RequestExchangeToken.
 *
 * @param Data A EOS_Connect_RequestExchangeTokenCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Connect_OnRequestExchangeTokenCallback, const EOS_Connect_RequestExchangeTokenCallbackInfo* Data);


/** The most recent version of the EOS_Connect_RemoveExchangeToken API. */
#define EOS_CONNECT_REMOVEEXCHANGETOKEN_API_LATEST 1

/**
 * Input parameters to the EOS_Connect_RemoveExchangeTokenOptions API.
 */
EOS_STRUCT(EOS_Connect_RemoveExchangeTokenOptions, (
	/** API Version: Set this to EOS_CONNECT_REMOVEEXCHANGETOKEN_API_LATEST. */
	int32_t ApiVersion;
	/** The the exchange token handle to remove. */
	EOS_ConnectExchangeTokenHandle ExchangeTokenHandle;
));

/**
 * Enum describing states for the exchange token.
 */
EOS_ENUM(EOS_Connect_EExchangeTokenState,
	/** State for when the exchange token is added and and ready to use. */
	EOS_CETS_Valid = 0,
	/** State for when an exchange token is about to expire. Note: Requests still succeed in this state but the user should remove this exchange token and request a new one. */
	EOS_CETS_AboutToExpire = 1,
	/** State for when an exchange token is expired. */
	EOS_CETS_Invalid = 2, 
	/** State for when an exchange token is removed. */
	EOS_CETS_Removed = 3
);

/** API to register for Exchange Auth Token state change notifications. */
#define EOS_CONNECT_ADDNOTIFYEXCHANGETOKENSTATECHANGED_API_LATEST 1

/**
 * Structure containing input parameters for the exchange token state change notification callback.
 */
EOS_STRUCT(EOS_Connect_AddNotifyExchangeTokenStateChangedOptions, (
	/** API Version: Set this to EOS_CONNECT_ADDNOTIFYEXCHANGETOKENSTATECHANGED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Parameters for the EOS_Connect_OnExchangeTokenStateChangedCallback function.
 */
EOS_STRUCT(EOS_Connect_ExchangeTokenStateChangedCallbackInfo, (
	/** Context that was passed into EOS_Connect_AddNotifyExchangeTokenStateChanged. */
	void* ClientData;
	/** The handle to the exchange token whose Auth is expired. */
	EOS_ConnectExchangeTokenHandle ExchangeTokenHandle;
	/** The exchange token state */
	EOS_Connect_EExchangeTokenState State;
));

/**
 * Function prototype definition for notifications that come from EOS_Connect_AddNotifyExchangeTokenStateChanged API.
 *
 * @param Data A EOS_Connect_ExchangeTokenStateChangedCallbackInfo containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_Connect_OnExchangeTokenStateChangedCallback, const EOS_Connect_ExchangeTokenStateChangedCallbackInfo* Data);

/** The most recent version of the EOS_Connect_UserTokenOverride structure */
#define EOS_CONNECT_USERTOKENOVERRIDE_API_LATEST 1

/**
 * The Connect User Auth Token overrides.
 */
EOS_STRUCT(EOS_Connect_UserTokenOverride, (
	/** API Version: Set this to EOS_CONNECT_USERTOKENOVERRIDE_API_LATEST. */
	int32_t ApiVersion;
	/** The User ID to associate the Exchange Token with. */
	EOS_ProductUserId UserId;
	/** The Exchange Token to override the Connect User Auth token with. */
	EOS_ConnectExchangeTokenHandle OverrideExchangeTokenHandle;
));

/** The most recent version of the EOS_Connect_ExecuteWithOverriddenTokens API. */
#define EOS_CONNECT_EXECUTEWITHOVERRIDDENTOKENS_API_LATEST 1

/**
 * Input parameters to the EOS_Connect_ExecuteWithOverriddenTokens API.
 */
EOS_STRUCT(EOS_Connect_ExecuteWithOverriddenTokensOptions, (
	/** API Version: Set this to EOS_CONNECT_EXECUTEWITHOVERRIDDENTOKENS_API_LATEST. */
	int32_t ApiVersion;
	/** The exchange auth token to override the Connect Client Auth Token with.  */
	EOS_ConnectExchangeTokenHandle ClientTokenOverride;
	/** The array of optional Connect User Token overrides. */
	const EOS_Connect_UserTokenOverride* UserTokenOverrides;
	/** The count of the UserTokenOverrides array. */
	uint32_t UserTokenOverridesCount;
));

/**
 * The parameters to the EOS_Connect_WithOverriddenTokenCallback function.
 */
EOS_STRUCT(EOS_Connect_WithOverriddenTokenCallbackInfo, (
	/** The context passed in EOS_Connect_ExecuteWithOverriddenTokens. */
	void* ClientData;
));

/**
 * Function prototype for the callback that will be executed with the overridden auth tokens. This function will be synchronously executed.
 *
 * @param Data A EOS_Connect_WithOverriddenTokenCallbackInfo,
 */
EOS_DECLARE_CALLBACK(EOS_Connect_WithOverriddenTokenCallback, const EOS_Connect_WithOverriddenTokenCallbackInfo* Data);

/** The most recent version of the EOS_Connect_ExchangeToken structure */
#define EOS_CONNECT_EXCHANGETOKEN_API_LATEST 1

/**
 * The exchange token data. 
 */
EOS_STRUCT(EOS_Connect_ExchangeToken, (
	/** API Version: Set this to EOS_CONNECT_EXCHANGETOKEN_API_LATEST. */
	int32_t ApiVersion;
	/** The Access Token. */
	const char* AccessToken;
	/**
	 * The Issued Token type for the exchange token.
	 * Note: The issued token type is a URI that may contain one of the Token Type Identifiers defined by RFC 8693 or a custom value.
	 */
	const char* IssuedTokenType;
));

/**
 * Release the memory associated with an EOS_Connect_ExchangeToken structure. This must be called on data retrieved from EOS_Connect_CopyExchangeToken.
 *
 * @param IdToken The ID token structure to be released.
 *
 * @see EOS_Connect_ExchangeToken
 * @see EOS_Connect_CopyExchangeToken
 */
EOS_DECLARE_FUNC(void) EOS_Connect_ExchangeToken_Release(EOS_Connect_ExchangeToken* ExchangeToken);

/** The most recent version of the EOS_Connect_CopyExchangeToken API. */
#define EOS_CONNECT_COPYEXCHANGETOKEN_API_LATEST 1

/**
 * Input parameters to the EOS_Connect_CopyExchangeToken API.
 */
EOS_STRUCT(EOS_Connect_CopyExchangeTokenOptions, (
	/** API Version: Set this to EOS_CONNECT_COPYEXCHANGETOKEN_API_LATEST. */
	int32_t ApiVersion;
	/** The exchange token handle from which the exchange token is copied. */
	EOS_ConnectExchangeTokenHandle ExchangeTokenHandle;
));

#pragma pack(pop)
