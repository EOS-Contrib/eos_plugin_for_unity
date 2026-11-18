// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_types.h"
#include "eos_auth_types.h"

#pragma pack(push, 8)

/** The most recent version of the EOS_IOS_Auth_HandleRedirectURLOptions structure. */
#define EOS_IOS_AUTH_HANDLEREDIRECTURL_API_LATEST 1

/**
 * Input parameters for the EOS_IOS_Auth_HandleRedirectURL function.
 */
EOS_STRUCT(EOS_IOS_Auth_HandleRedirectURLOptions, (
	/** API Version: Set this to EOS_IOS_AUTH_HANDLEREDIRECTURL_API_LATEST. */
	int32_t ApiVersion;
	/** The redirect URL the application received via its iOS Universal Link handler, including the query string
	 *  produced by the Account Portal, for example "https://mygame.com/auth/1234?code=...".
	 *  The SDK parses the authorization code (or error) out of the query string.
	 */
	const char* RedirectURL;
));

/**
 * Complete an in-progress mobile Account Portal login by delivering the redirect URL the application received via its
 * iOS Universal Link system API (continueUserActivity / OnActivatedByProtocol).
 *
 * @note Call this after starting an EOS_Auth_Login with EOS_LCT_AccountPortal and an https AccountPortalRedirectURL (see
 *       EOS_IOS_Auth_CredentialsOptions). The login result itself is still reported through the EOS_Auth_Login completion
 *       delegate, this return value only indicates whether the redirect was accepted.
 *
 * @param Options structure containing the redirect URL to deliver to the in-progress login
 *
 * @see EOS_IOS_Auth_HandleRedirectURLOptions
 * @see EOS_IOS_Auth_CredentialsOptions
 *
 * @return EOS_EResult indicating whether the redirect was accepted
 * - EOS_Success if a login was awaiting a redirect and the URL was routed to it
 * - EOS_NotFound if no mobile Account Portal login is currently awaiting a redirect
 * - EOS_InvalidParameters if Handle, Options, or RedirectURL is null or invalid
 *
 * @see EOS_IOS_Auth_CredentialsOptions
 */
EOS_DECLARE_FUNC(EOS_EResult) EOS_IOS_Auth_HandleRedirectURL(EOS_HAuth Handle, const EOS_IOS_Auth_HandleRedirectURLOptions* Options);

#pragma pack(pop)
