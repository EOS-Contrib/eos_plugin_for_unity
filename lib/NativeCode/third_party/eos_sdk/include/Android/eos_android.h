// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_types.h"

#pragma pack(push, 8)
/** The most recent version of the EOS_Android_InitializeOptions structure. */
#define EOS_ANDROID_INITIALIZEOPTIONS_API_LATEST 2

/**
 * Options for initializing mount paths required for some platforms.
 */
EOS_STRUCT(EOS_Android_InitializeOptions, (
	/** API Version: Set this to EOS_ANDROID_INITIALIZEOPTIONS_API_LATEST. */
	int32_t ApiVersion;

	/** Reserved, set to null */
	void* Reserved;

	/** Full internal directory path. Can be null */
	const char* OptionalInternalDirectory;
	/** Full external directory path. Can be null */
	const char* OptionalExternalDirectory;
));

/** The most recent version of the EOS_Android_Auth_CredentialsOptions structure. */
#define EOS_ANDROID_AUTH_CREDENTIALSOPTIONS_API_LATEST 1

/**
 * Options for initializing login for Android.
 */
EOS_STRUCT(EOS_Android_Auth_CredentialsOptions, (
	/** API version of EOS_Android_Auth_CredentialsOptions. */
	int32_t ApiVersion;

	/** Optional App Link URL for Auth
	 *  
	 * By default, if null value is provided, EOS uses deeplinks.
	 * We recommend configuring Android App Links instead.
	 *
	 * An example value would be: "https://mygame.com/auth/`client_id".
	 * Replace the example "https://mygame.com/auth/" prefix to match your Android App Link configuration.
	 * Note the "`client_id", this special substring will be replaced by SDK with actual client id value.
	 *
	 * App Links require additional changes in the AndroidManifest.xml, on the website, and in Dev Portal.
	 * See EOS and Android documentation for more information.
	 */
	const char* AccountPortalRedirectURL;
));

#pragma pack(pop)
