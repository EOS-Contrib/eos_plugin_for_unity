// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "eos_common.h"

#pragma pack(push, 8)

EOS_EXTERN_C typedef struct EOS_K3SHandle* EOS_HK3S;

/** The maximum length of a K3S input string. Any strings longer than this will fail input validation. */
#define EOS_K3S_STRING_MAX_LENGTH 128

/** A playtime activity. */
EOS_ENUM(EOS_K3S_EPlaytimeActivity,
	/** The user is in a shared experience, such as in a settings menu. */
	EOS_K3SPA_Shared = 0,
	/** The user is playing. */
	EOS_K3SPA_Play = 1,
	/** The user is in a creative experience. */
	EOS_K3SPA_Create = 2,
	/** The user is on a full-screen takeover. */
	EOS_K3SPA_Lockout = 3
);

/** A playtime scope. */
EOS_ENUM(EOS_K3S_EPlaytimeScope,
	/** The remaining playtime for the current window. */
	EOS_K3SPS_RemainingCurrentWindow = 0,
	/** The remaining playtime for today. */
	EOS_K3SPS_RemainingToday = 1,
	/** The used playtime for today. */
	EOS_K3SPS_UsedToday = 2
);

/** A playtime schedule window scope. */
EOS_ENUM(EOS_K3S_EPlaytimeScheduleWindowScope,
	/** The current playtime schedule window. */
	EOS_K3SPSWS_Current = 0,
	/** The next playtime schedule window. This window may not actually be available if the user has no remaining playtime for the day. */
	EOS_K3SPSWS_Next = 1,
	/** The next available playtime schedule window. This window is guaranteed to be available based on the user's remaining playtime for the day. */
	EOS_K3SPSWS_NextAvailable = 2
);

/** A variant type. */
EOS_ENUM(EOS_K3S_EVariantType,
	/** The variant is a bool. */
	EOS_K3SVT_Boolean = 0,
	/** The variant is a float. */
	EOS_K3SVT_Float = 1,
	/** The variant is a string. */
	EOS_K3SVT_String = 2
);

/** An effective value source. */
EOS_ENUM(EOS_K3S_EEffectiveValueSource,
	/** The source is a default value. */
	EOS_K3SEVS_Default = 0,
	/** The source is a preference value. */
	EOS_K3SEVS_Preference = 1,
	/** The source is a parent limit value. */
	EOS_K3SEVS_ParentLimit = 2,
	/** The source is an enforced limit value. */
	EOS_K3SEVS_EnforcedLimit = 3
);

/* An allowed product overrides. */
EOS_ENUM(EOS_K3S_EAllowedProductOverrides,
	/** The setting cannot be inherited at the product level. */
	EOS_K3SAPO_BlockInheritance = 0,
	/** The setting is inherited at the product level, but nothing can be overridden.  */
	EOS_K3SAPO_None = 1,
	/** The setting is inherited at the product level, but only values can be overridden. */
	EOS_K3SAPO_Values = 2,
	/** The setting is inherited at the product level, but only values and translations can be overridden. */
	EOS_K3SAPO_Translations = 3,
	/** The setting is inherited at the product level, but only values, translations, and options can be overridden. */
	EOS_K3SAPO_Options = 4,
	/** The setting is inherited at the product level, but only values, translations, options, and regions can be overridden. */
	EOS_K3SAPO_Regions = 5,
	/** The setting is inherited at the product level, but only values, translations, options, regions, and age brackets can be overridden. */
	EOS_K3SAPO_AgeBrackets = 6
);

/** A setting type. */
EOS_ENUM(EOS_K3S_ESettingType,
	/** The setting is a bool. */
	EOS_K3SST_Boolean = 0,
	/** The setting is a float. */
	EOS_K3SST_Float = 1,
	/** The setting is an option. */
	EOS_K3SST_Option = 2,
	/** The setting is a date. */
	EOS_K3SST_Date = 3
);

/** A data storage. */
EOS_ENUM(EOS_K3S_EDataStorage,
	/** No data storage. */
	EOS_K3SDS_None = 0,
	/** The data storage is raw. */
	EOS_K3SDS_Raw = 1,
	/** The data storage is read only. */
	EOS_K3SDS_ReadOnly = 2
);

/** A restrictive order. */
EOS_ENUM(EOS_K3S_ERestrictiveOrder,
	/** The restrictive order is first restrictive. */
	EOS_K3SRO_FirstRestrictive = 0,
	/** The restrictive order is first permissive. */
	EOS_K3SRO_FirstPermissive = 1,
	/** The restrictive order is low restrictive. */
	EOS_K3SRO_LowRestrictive = 2,
	/** The restrictive order is low permissive. */
	EOS_K3SRO_LowPermissive = 3,
	/** The restrictive order is false restrictive. */
	EOS_K3SRO_FalseRestrictive = 4,
	/** The restrictive order is false permissive. */
	EOS_K3SRO_FalsePermissive = 5,
	/** The restrictive order is earlier restrictive. */
	EOS_K3SRO_EarlierRestrictive = 6,
	/** The restrictive order is earlier permissive. */
	EOS_K3SRO_EarlierPermissive = 7
);

/** An inheritance. */
EOS_ENUM(EOS_K3S_EInheritance,
	/** The inheritance is at the product level. */
	EOS_K3SI_Product = 0,
	/** The inheritance is at the organization level. */
	EOS_K3SI_Organization = 1
);

/** A consent. */
EOS_ENUM(EOS_K3S_EConsent,
	/** No consent. */
	EOS_K3SC_None = 0,
	/** The consent is opt out. */
	EOS_K3SC_OptOut = 1,
	/** The consent is opt in unverified. */
	EOS_K3SC_OptInUnverified = 2,
	/** The consent is opt in verified. */
	EOS_K3SC_OptInVerified = 3
);

/** A consent application. */
EOS_ENUM(EOS_K3S_EConsentApplication,
	/** The consent application is default. */
	EOS_K3SCA_Default = 0,
	/** The consent application is enforced users only. */
	EOS_K3SCA_EnforcedUsersOnly = 1
);

/** A schedule period. */
EOS_ENUM(EOS_K3S_ESchedulePeriod,
	/** The schedule period is 5 minutes. */
	EOS_K3SSP_Minutes5 = 0,
	/** The schedule period is 10 minutes. */
	EOS_K3SSP_Minutes10 = 1,
	/** The schedule period is 15 minutes. */
	EOS_K3SSP_Minutes15 = 2,
	/** The schedule period is 30 minutes. */
	EOS_K3SSP_Minutes30 = 3,
	/** The schedule period is 1 hour. */
	EOS_K3SSP_Hours1 = 4,
	/** The schedule period is 1 day. */
	EOS_K3SSP_Days1 = 5
);

/** A setting change. */
EOS_ENUM(EOS_K3S_ESettingChange,
	/** The setting change is all. */
	EOS_K3SSC_All = 0,
	/** The setting change is the effective value. */
	EOS_K3SSC_EffectiveValue = 1,
	/** The setting change is the parent limit value. */
	EOS_K3SSC_ParentLimitValue = 2,
	/** The setting change is the parent limit schedules. */
	EOS_K3SSC_ParentLimitSchedules = 3,
	/** The setting change is user info. */
	EOS_K3SSC_UserInfo = 4
);

/** The most recent version of the EOS_K3S_Variant struct. */
#define EOS_K3S_VARIANT_API_LATEST 1

/**
 * A variant.
 */
EOS_STRUCT(EOS_K3S_Variant, (
	/** API Version: Set this to EOS_K3S_VARIANT_API_LATEST. */
	int32_t ApiVersion;
	union
	{
		/** The value as a bool. */
		EOS_Bool AsBool;
		/** The value as a float. */
		float AsFloat;
		/** The value as a null terminated UTF8 string. */
		const char* AsUtf8;
	} Value;
	/** The type of the value. */
	EOS_K3S_EVariantType ValueType;
));

/** The most recent version of the EOS_K3S_Schedule struct. */
#define EOS_K3S_SCHEDULE_API_LATEST 1

/**
 * A schedule.
 */
EOS_STRUCT(EOS_K3S_Schedule, (
	/** API Version: Set this to EOS_K3S_SCHEDULE_API_LATEST. */
	int32_t ApiVersion;
	/** The window of the schedule. */
	const char* Window;
	/** The value of the schedule. */
	const EOS_K3S_Variant* Value;
));

/** The most recent version of the EOS_K3S_Translation struct. */
#define EOS_K3S_TRANSLATION_API_LATEST 1

/**
 * A translation.
 */
EOS_STRUCT(EOS_K3S_Translation, (
	/** API Version: Set this to EOS_K3S_TRANSLATION_API_LATEST. */
	int32_t ApiVersion;
	/** The label of the translation. */
	const char* Label;
	/** The user notice of the translation. */
	const char* UserNotice;
	/** The parent notice of the translation. */
	const char* ParentNotice;
	/** The input title of the translation. */
	const char* InputTitle;
));

/** The most recent version of the EOS_K3S_Option struct. */
#define EOS_K3S_OPTION_API_LATEST 1

/**
 * An option.
 */
EOS_STRUCT(EOS_K3S_Option, (
	/** API Version: Set this to EOS_K3S_OPTION_API_LATEST. */
	int32_t ApiVersion;
	/** The value of the option. */
	const char* Value;
	/** The translation of the option. This field should always be valid. */
	const EOS_K3S_Translation* Translation;
));

/** The most recent version of the EOS_K3S_AgeBracket struct. */
#define EOS_K3S_AGEBRACKET_API_LATEST 1

/** An age bracket. */
EOS_STRUCT(EOS_K3S_AgeBracket, (
	/** API Version: Set this to EOS_K3S_AGEBRACKET_API_LATEST. */
	int32_t ApiVersion;
	/** The consent of the age bracket. */
	EOS_K3S_EConsent Consent;
	/** The consent application of the age bracket. This field is optional. */
	const EOS_K3S_EConsentApplication* ConsentApplication;
	/** The default preference value of the age bracket. This field should always be valid. */
	const EOS_K3S_Variant* DefaultPreferenceValue;
	/** The default parent limit value of the age bracket. This field is optional. */
	const EOS_K3S_Variant* DefaultParentLimitValue;
	/** The enforced limit value of the age bracket. This field is optional. */
	const EOS_K3S_Variant* EnforcedLimitValue;
	/** An array of enforced limit schedules of the age bracket. This field is optional. */
	const EOS_K3S_Schedule* EnforcedLimitSchedules;
	/** A count of the EnforcedLimitSchedules array. */
	uint32_t EnforcedLimitSchedulesCount;
));

/** The most recent version of the EOS_K3S_Definition struct. */
#define EOS_K3S_DEFINITION_API_LATEST 1

/**
 * A definition.
 */
EOS_STRUCT(EOS_K3S_Definition, (
	/** API Version: Set this to EOS_K3S_DEFINITION_API_LATEST. */
	int32_t ApiVersion;
	/** The type of the setting. */
	EOS_K3S_ESettingType SettingType;
	/** The number of decimals of a float setting. This field is optional. */
	const uint32_t* NumberOfDecimals;
	/** The minimum value of a float setting. This field is optional. */
	const float* MinimumValue;
	/** The maximum value of a float setting. This field is optional. */
	const float* MaximumValue;
	/** The increment or decrement value of a float setting. This field is optional. */
	const float* IncrementOrDecrementValue;
	/** The allowed product overrides of the setting. This field is optional. */
	const EOS_K3S_EAllowedProductOverrides* AllowedProductOverrides;
	/** The inheritance of the setting. This field is optional. */
	const EOS_K3S_EInheritance* Inheritance;
	/** The restrictive order of the setting.  */
	EOS_K3S_ERestrictiveOrder RestrictiveOrder;
	/** Whether the setting is visible. */
	EOS_Bool bIsVisible;
	/** Whether the setting is required. */
	EOS_Bool bIsRequired;
	/** The data storage of the setting. This field is optional. */
	const EOS_K3S_EDataStorage* DataStorage;
	/** The translation of the setting. This field should always be valid. */
	const EOS_K3S_Translation* Translation;
	/** The age bracket of the setting. This field should always be valid. */
	const EOS_K3S_AgeBracket* AgeBracket;
	/** The previous age bracket of the setting. This field is optional. */
	const EOS_K3S_AgeBracket* PreviousAgeBracket;
	/** An array of options of the setting. This field is optional. */
	const EOS_K3S_Option* Options;
	/** A count of the Options array. */
	uint32_t OptionsCount;
	/** The maximum schedules per day of the setting. This field is optional. */
	const uint32_t* MaximumSchedulesPerDay;
	/** The schedule period of the setting. This field is optional. */
	const EOS_K3S_ESchedulePeriod* SchedulePeriod;

));

/** The most recent version of the EOS_K3S_Setting struct. */
#define EOS_K3S_SETTING_API_LATEST 1

/**
 * A setting.
 */
EOS_STRUCT(EOS_K3S_Setting, (
	/** API Version: Set this to EOS_K3S_SETTING_API_LATEST. */
	int32_t ApiVersion;
	/** The namespace of the setting. */
	const char* Namespace;
	/** The name of the setting. */
	const char* Name;
	/** The preferred value of the setting. This field should always be valid. */
	const EOS_K3S_Variant* PreferredValue;
	/** The inheritance of the preferred value. */
	EOS_K3S_EInheritance PreferredValueInheritance;
	/** The unix timestamp in milliseconds when the preferred value of the setting was last updated. This field is optional. */
	const int64_t* PreferredValueUpdatedAt;
	/** The parent limit value of the setting. This field is optional. */
	const EOS_K3S_Variant* ParentLimitValue;
	/** The parent limit value inheritance of the setting. This field is optional. */
	const EOS_K3S_EInheritance* ParentLimitValueInheritance;
	/** The unix timestamp in milliseconds when the parent limit value was last updated. This field is optional. */
	const int64_t* ParentLimitValueUpdatedAt;
	/** An array of parent limit schedules of the setting. This field is optional. */
	const EOS_K3S_Schedule* ParentLimitSchedules;
	/** A count of the ParentLimitSchedules array. */
	uint32_t ParentLimitSchedulesCount;
	/** Whether the parent limit schedules is enabled. If enabled, the parent limit schedules are used to calculate the effective value. This field is optional. */
	const EOS_Bool* bIsParentLimitSchedulesEnabled;
	/** The parent limit schedules time zone. If parent limit schedules are enabled, the time zone is used to calculate the effective value. This field is optional. */
	const char* ParentLimitSchedulesTimeZone;
	/** The effective value of the setting. This field should always be valid. */
	const EOS_K3S_Variant* EffectiveValue;
	/** The effective value source of the setting. */
	EOS_K3S_EEffectiveValueSource EffectiveValueSource;
	/** The unix timestamp in milliseconds when the user last graduated to a different age bracket. This field is optional. */
	const int64_t* LastGraduatedAt;
	/** The inheritance of the setting. */
	EOS_K3S_EInheritance Inheritance;
	/** The definition of the setting. This field should always be valid. */
	const EOS_K3S_Definition* Definition;
));

/**
 * Release the memory associated with an EOS_K3S_Setting structure. This must be called on data retrieved from EOS_K3S_CopySettingByIndex.
 *
 * @param Setting The setting structure to release.
 */
EOS_DECLARE_FUNC(void) EOS_K3S_Setting_Release(EOS_K3S_Setting* Setting);

/** The most recent version of the EOS_K3S_ScheduleUpdate struct. */
#define EOS_K3S_SCHEDULEUPDATE_API_LATEST 1

/**
 * A schedule update.
 */
EOS_STRUCT(EOS_K3S_ScheduleUpdate, (
	/** API Version: Set this to EOS_K3S_SCHEDULEUPDATE_API_LATEST. */
	int32_t ApiVersion;
	/** The default value of the schedules. */
	const EOS_K3S_Variant* DefaultValue;
	/** An array of schedules of the update. */
	const EOS_K3S_Schedule* Schedules;
	/** A count of the Schedules array. */
	uint32_t SchedulesCount;
	/** Whether the schedules are enabled or not. */
	EOS_Bool bIsEnabled;
));

/** The most recent version of the EOS_K3S_SettingUpdate struct. */
#define EOS_K3S_SETTINGUPDATE_API_LATEST 1

/**
 * A setting update.
 */
EOS_STRUCT(EOS_K3S_SettingUpdate, (
	/** API Version: Set this to EOS_K3S_SETTINGUPDATE_API_LATEST. */
	int32_t ApiVersion;
	/** The namespace of the setting. */
	const char* Namespace;
	/** The name of the setting. */
	const char* Name;
	/** The preferred value of the setting. If a parent pin is set, this will be the parent limit value of the setting. */
	const EOS_K3S_Variant* PreferredValue;
	/** The parent limit schedule update of the setting. */
	const EOS_K3S_ScheduleUpdate* ParentLimitScheduleUpdate;
));

/** The most recent version of the EOS_K3S_PlaytimeScheduleWindow struct. */
#define EOS_K3S_PLAYTIMESCHEDULEWINDOW_API_LATEST 1

/**
 * A playtime schedule window.
 */
EOS_STRUCT(EOS_K3S_PlaytimeScheduleWindow, (
	/** API Version: Set this to EOS_K3S_PLAYTIMESCHEDULEWINDOW_API_LATEST. */
	int32_t ApiVersion;
	/** The start of the schedule window. */
	const char* Start;
	/** The end of the schedule window. */
	const char* End;
));

/**
 * Release the memory associated with an EOS_K3S_PlaytimeScheduleWindow structure. This must be called on data retrieved from EOS_K3S_CopyPlaytimeScheduleWindow.
 *
 * @param PlaytimeScheduleWindow The playtime schedule window to release.
 */
EOS_DECLARE_FUNC(void) EOS_K3S_PlaytimeScheduleWindow_Release(EOS_K3S_PlaytimeScheduleWindow* PlaytimeScheduleWindow);

/** The most recent version of the EOS_K3S_QuerySettingsOptions struct. */
#define EOS_K3S_QUERYSETTINGS_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_QuerySettings function.
 */
EOS_STRUCT(EOS_K3S_QuerySettingsOptions, (
	/** API Version: Set this to EOS_K3S_QUERYSETTINGS_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the settings to query. */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Output parameters for the EOS_K3S_QuerySettings function.
 * 
 * Possible result codes:
 * - EOS_Success:             The query was successful.
 * - EOS_IncompatibleVersion: The API version passed in is incorrect.
 * - EOS_InvalidParameters:   An options parameter is incorrect.
 * - EOS_InvalidUser:         The local user does not exist.
 * - EOS_InvalidAuth:         The local user exists but is not logged in.
 * - EOS_TooManyRequests:     There are too many requests in progress at this time.
 */
EOS_STRUCT(EOS_K3S_QuerySettingsCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** The context that was passed into EOS_K3S_QuerySettings. */
	void* ClientData;
	/** The Epic Account ID of the settings that were queried. */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Function prototype definition for callbacks that come from EOS_K3S_QuerySettings.
 * @param Data Callback info containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_K3S_OnQuerySettings, const EOS_K3S_QuerySettingsCallbackInfo* Data);

/** The most recent version of the EOS_K3S_GetSettingCountOptions struct. */
#define EOS_K3S_GETSETTINGCOUNT_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_GetSettingCount function.
 */
EOS_STRUCT(EOS_K3S_GetSettingCountOptions, (
	/** API Version: Set this to EOS_K3S_GETSETTINGCOUNT_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the setting count to get. */
	EOS_EpicAccountId LocalUserId;
));

/** The most recent version of the EOS_K3S_CopySettingByIndexOptions struct. */
#define EOS_K3S_COPYSETTINGBYINDEX_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_CopySettingByIndex function.
 */
EOS_STRUCT(EOS_K3S_CopySettingByIndexOptions, (
	/** API Version: Set this to EOS_K3S_COPYSETTINGBYINDEX_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the setting to copy. */
	EOS_EpicAccountId LocalUserId;
	/**
	 * The index of the setting to copy.
	 * @see EOS_K3S_GetSettingCount
	 */
	uint32_t Index;
));

/** The most recent version of the EOS_K3S_CopySettingByNameOptions struct. */
#define EOS_K3S_COPYSETTINGBYNAME_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_CopySettingByName function.
 */
EOS_STRUCT(EOS_K3S_CopySettingByNameOptions, (
	/** API Version: Set this to EOS_K3S_COPYSETTINGBYNAME_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the setting to copy. */
	EOS_EpicAccountId LocalUserId;
	/** The namespace of the setting to copy. */
	const char* Namespace;
	/** The name of the setting to copy. */
	const char* Name;
));

/** The most recent version of the EOS_K3S_UpdateSettingsOptions struct. */
#define EOS_K3S_UPDATESETTINGS_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_UpdateSettings function.
 */
EOS_STRUCT(EOS_K3S_UpdateSettingsOptions, (
	/** API Version: Set this to EOS_K3S_UPDATESETTINGS_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the settings to update. */
	EOS_EpicAccountId LocalUserId;
	/** An array of updates. */
	const EOS_K3S_SettingUpdate* Updates;
	/** A count of the Updates array. */
	uint32_t UpdatesCount;
	/** The parent pin of the settings to update. If this is NULL, the update will be for preferred values. If this is set, the update will be for parent limit values. */
	const char* ParentPin;
	/** Whether this is the initial sync of a user's previously locally-stored preferred values. */
	EOS_Bool bIsInitialSync;
	/** The IANA timezone of the update. This field is optional, but is only relevant if ParentPin is set. */
	const char* TimeZone;
));

/**
 * Output parameters for the EOS_K3S_UpdateSettings function.
 * 
 * Possible result codes:
 * - EOS_Success:             The request was successful.
 * - EOS_IncompatibleVersion: The API version passed in is incorrect.
 * - EOS_InvalidParameters:   An options parameter is incorrect.
 * - EOS_InvalidUser:         The local user does not exist.
 * - EOS_InvalidAuth:         The local user exists but is not logged in.
 *   EOS_LimitExceeded:       An input parameter string is too long.
 * - EOS_TooManyRequests:     There are too many requests in progress at this time.
 */
EOS_STRUCT(EOS_K3S_UpdateSettingsCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** The context that was passed into EOS_K3S_UpdateSettings. */
	void* ClientData;
	/** The Epic Account ID of the settings that were updated. */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Function prototype definition for callbacks that come from EOS_K3S_UpdateSettings.
 * @param Data Callback info containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_K3S_OnUpdateSettings, const EOS_K3S_UpdateSettingsCallbackInfo* Data);

/** The most recent version of the EOS_K3S_SetCurrentPlaytimeActivityOptions struct. */
#define EOS_K3S_SETCURRENTPLAYTIMEACTIVITY_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_SetCurrentPlaytimeActivity function.
 */
EOS_STRUCT(EOS_K3S_SetCurrentPlaytimeActivityOptions, (
	/** API Version: Set this to EOS_K3S_SETCURRENTPLAYTIMEACTIVITY_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the current playtime activity to set. */
	EOS_EpicAccountId LocalUserId;
	/** The current playtime activity to set. */
	EOS_K3S_EPlaytimeActivity CurrentPlaytimeActivity;
	/** Whether the current playtime activity is idle. */
	EOS_Bool bIsIdle;
));

/** The most recent version of the EOS_K3S_RequestAdditionalPlaytimeOptions struct. */
#define EOS_K3S_REQUESTADDITIONALPLAYTIME_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_RequestAdditionalPlaytime function.
 */
EOS_STRUCT(EOS_K3S_RequestAdditionalPlaytimeOptions, (
	/** API Version: Set this to EOS_K3S_REQUESTADDITIONALPLAYTIME_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the additional playtime to request. */
	EOS_EpicAccountId LocalUserId;
	/** The playtime activity of the additional playtime to request. */
	EOS_K3S_EPlaytimeActivity PlaytimeActivity;
));

/**
 * Output parameters for the EOS_K3S_RequestAdditionalPlaytime function.
 *
 * Possible result codes:
 * - EOS_Success:             The request was successful.
 * - EOS_IncompatibleVersion: The API version passed in is incorrect.
 * - EOS_InvalidParameters:   An options parameter is incorrect.
 * - EOS_InvalidUser:         The local user does not exist.
 * - EOS_InvalidAuth:         The local user exists but is not logged in.
 * - EOS_TooManyRequests:     There are too many requests in progress at this time.
 */
EOS_STRUCT(EOS_K3S_RequestAdditionalPlaytimeCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** The context that was passed into EOS_K3S_RequestAdditionalPlaytime. */
	void* ClientData;
	/** The Epic Account ID of the additional playtime that was requested. */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Function prototype definition for callbacks that come from EOS_K3S_RequestAdditionalPlaytime.
 * @param Data Callback info containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_K3S_OnRequestAdditionalPlaytime, const EOS_K3S_RequestAdditionalPlaytimeCallbackInfo* Data);

/** The most recent version of the EOS_K3S_GrantAdditionalPlaytimeOptions struct. */
#define EOS_K3S_GRANTADDITIONALPLAYTIME_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_GrantAdditionalPlaytime function.
 */
EOS_STRUCT(EOS_K3S_GrantAdditionalPlaytimeOptions, (
	/** API Version: Set this to EOS_K3S_GRANTADDITIONALPLAYTIME_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the additional playtime to grant. */
	EOS_EpicAccountId LocalUserId;
	/** The playtime activity of the additional playtime to grant. */
	EOS_K3S_EPlaytimeActivity PlaytimeActivity;
	/** The seconds of the additional playtime to grant. */
	uint32_t Seconds;
	/** The parent pin of the additional playtime to grant. */
	const char* ParentPin;
));

/**
 * Output parameters for the EOS_K3S_GrantAdditionalPlaytime function.
 * 
 * Possible result codes:
 * - EOS_Success:             The grant was successful.
 * - EOS_IncompatibleVersion: The API version passed in is incorrect.
 * - EOS_InvalidParameters:   An options parameter is incorrect.
 * - EOS_InvalidUser:         The local user does not exist.
 * - EOS_InvalidAuth:         The local user exists but is not logged in.
 * - EOS_TooManyRequests:     There are too many requests in progress at this time.
 */
EOS_STRUCT(EOS_K3S_GrantAdditionalPlaytimeCallbackInfo, (
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	EOS_EResult ResultCode;
	/** The context that was passed into EOS_K3S_GrantAdditionalPlaytime. */
	void* ClientData;
	/** The Epic Account ID of the additional playtime that was granted. */
	EOS_EpicAccountId LocalUserId;
));

/**
 * Function prototype definition for callbacks that come from EOS_K3S_GrantAdditionalPlaytime.
 * @param Data Callback info containing the output information and result.
 */
EOS_DECLARE_CALLBACK(EOS_K3S_OnGrantAdditionalPlaytime, const EOS_K3S_GrantAdditionalPlaytimeCallbackInfo* Data);

/** The most recent version of the EOS_K3S_GetPlaytimeOptions struct. */
#define EOS_K3S_GETPLAYTIME_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_GetPlaytime function.
 */
EOS_STRUCT(EOS_K3S_GetPlaytimeOptions, (
	/** API Version: Set this to EOS_K3S_GETPLAYTIME_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the playtime to get. */
	EOS_EpicAccountId LocalUserId;
	/** The playtime activity of the playtime to get. */
	EOS_K3S_EPlaytimeActivity PlaytimeActivity;
	/** The playtime scope of the playtime to get. */
	EOS_K3S_EPlaytimeScope PlaytimeScope;
));

/** The most recent version of the EOS_K3S_CopyPlaytimeScheduleWindowOptions struct. */
#define EOS_K3S_COPYPLAYTIMESCHEDULEWINDOW_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_CopyPlaytimeScheduleWindow function.
 */
EOS_STRUCT(EOS_K3S_CopyPlaytimeScheduleWindowOptions, (
	/** API Version: Set this to EOS_K3S_COPYPLAYTIMESCHEDULEWINDOW_API_LATEST. */
	int32_t ApiVersion;
	/** The Epic Account ID of the playtime schedule window to copy. */
	EOS_EpicAccountId LocalUserId;
	/** The activity of the playtime schedule window to copy. */
	EOS_K3S_EPlaytimeActivity PlaytimeActivity;
	/** The scope of the playtime schedule window to copy. */
	EOS_K3S_EPlaytimeScheduleWindowScope PlaytimeScheduleWindowScope;
));

/** The most recent version of the EOS_K3S_AddNotifySettingChangedOptions struct. */
#define EOS_K3S_ADDNOTIFYSETTINGCHANGED_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_AddNotifySettingChanged function.
 */
EOS_STRUCT(EOS_K3S_AddNotifySettingChangedOptions, (
	/** API Version: Set this to EOS_K3S_ADDNOTIFYSETTINGCHANGED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_K3S_OnSettingChangedCallback Function.
 */
EOS_STRUCT(EOS_K3S_SettingChangedCallbackInfo, (
	/** The context that was passed into EOS_K3S_AddNotifySettingChanged. */
	void* ClientData;
	/** The Epic Account ID of the setting that was changed. */
	EOS_EpicAccountId LocalUserId;
	/** The namespace of the setting that was changed. */
	const char* Namespace;
	/** The name of the setting that was changed. */
	const char* Name;
	/** The change of the setting. */
	EOS_K3S_ESettingChange Change;
));

/**
 * Function prototype definition for notifications that come from EOS_K3S_AddNotifySettingChanged.
 * @param Data Callback info containing the output information.
 */
EOS_DECLARE_CALLBACK(EOS_K3S_OnSettingChangedCallback, const EOS_K3S_SettingChangedCallbackInfo* Data);

/** The most recent version of the EOS_K3S_AddNotifyAdditionalPlaytimeGrantedOptions struct. */
#define EOS_K3S_ADDNOTIFYADDITIONALPLAYTIMEGRANTED_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_AddNotifyAdditionalPlaytimeGranted function.
 */
EOS_STRUCT(EOS_K3S_AddNotifyAdditionalPlaytimeGrantedOptions, (
	/** API Version: Set this to EOS_K3S_ADDNOTIFYADDITIONALPLAYTIMEGRANTED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_K3S_OnAdditionalPlaytimeGrantedCallback function.
 */
EOS_STRUCT(EOS_K3S_AdditionalPlaytimeGrantedCallbackInfo, (
	/** The context that was passed into EOS_K3S_AddNotifyAdditionalPlaytimeGranted. */
	void* ClientData;
	/** The Epic Account ID that additional playtime that was granted. */
	EOS_EpicAccountId LocalUserId;
	/** The playtime activity of the additional playtime that was granted. */
	EOS_K3S_EPlaytimeActivity PlaytimeActivity;
));

/**
 * Function prototype definition for notifications that come from EOS_K3S_AddNotifyAdditionalPlaytimeGranted.
 * @param Data Callback info containing the output information.
 */
EOS_DECLARE_CALLBACK(EOS_K3S_OnAdditionalPlaytimeGrantedCallback, const EOS_K3S_AdditionalPlaytimeGrantedCallbackInfo* Data);

/** The most recent version of the EOS_K3S_AddNotifyRemainingPlaytimeLimitReachedOptions struct. */
#define EOS_K3S_ADDNOTIFYREMAININGPLAYTIMELIMITREACHED_API_LATEST 1

/**
 * Input parameters for the EOS_K3S_AddNotifyRemainingPlaytimeLimitReached function.
 */
EOS_STRUCT(EOS_K3S_AddNotifyRemainingPlaytimeLimitReachedOptions, (
	/** API Version: Set this to EOS_K3S_ADDNOTIFYREMAININGPLAYTIMELIMITREACHED_API_LATEST. */
	int32_t ApiVersion;
));

/**
 * Output parameters for the EOS_K3S_OnRemainingPlaytimeLimitReachedCallback function.
 */
EOS_STRUCT(EOS_K3S_RemainingPlaytimeLimitReachedCallbackInfo, (
	/** Context that was passed into EOS_K3S_AddNotifyRemainingPlaytimeLimitReached. */
	void* ClientData;
	/** The Epic Account ID of the remaining playtime limit that was reached. */
	EOS_EpicAccountId LocalUserId;
	/** The playtime activity of the remaining playtime limit that was reached. */
	EOS_K3S_EPlaytimeActivity PlaytimeActivity;
	/** The remaining playtime limit that was reached. */
	uint32_t RemainingPlaytime;
));

/**
 * Function prototype definition for notifications that come from EOS_K3S_AddNotifyRemainingPlaytimeLimitReached.
 * @param Data Callback info containing the output information.
 */
EOS_DECLARE_CALLBACK(EOS_K3S_OnRemainingPlaytimeLimitReachedCallback, const EOS_K3S_RemainingPlaytimeLimitReachedCallbackInfo* Data);

#pragma pack(pop)
