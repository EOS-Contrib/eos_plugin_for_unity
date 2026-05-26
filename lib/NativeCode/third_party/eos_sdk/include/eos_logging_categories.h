// Copyright Epic Games, Inc. All Rights Reserved.

// This file is not intended to be included directly. Include eos_logging.h instead.

/**
 * Logging Categories
 * To use:
 *	1. #define PROCESS_CATEGORY(CategoryName, EOSCategoryLabel, EOSCategoryValue)
 *	2. #define PROCESS_CATEGORY_LAST(CategoryName, EOSCategoryLabel, EOSCategoryValue)
 *	3. #include "eos_logging_categories.h"
 *	4. #undef PROCESS_CATEGORY_LAST
 *	5. #undef PROCESS_CATEGORY
 */

#if defined(PROCESS_CATEGORY)

/** Low level logs unrelated to specific services */
PROCESS_CATEGORY(LogEOS, EOS_LC_Core, 0)
/** Logs related to the Auth service */
PROCESS_CATEGORY(LogEOSAuth, EOS_LC_Auth, 1)
/** Logs related to the Friends service */
PROCESS_CATEGORY(LogEOSFriends, EOS_LC_Friends, 2)
/** Logs related to the Presence service */
PROCESS_CATEGORY(LogEOSPresence, EOS_LC_Presence, 3)
/** Logs related to the UserInfo service */
PROCESS_CATEGORY(LogEOSUserInfo, EOS_LC_UserInfo, 4)
/** Logs related to HTTP serialization */
PROCESS_CATEGORY(LogHttpSerialization, EOS_LC_HttpSerialization, 5)
/** Logs related to the Ecommerce service */
PROCESS_CATEGORY(LogEOSEcom, EOS_LC_Ecom, 6)
/** Logs related to the P2P service */
PROCESS_CATEGORY(LogEOSP2P, EOS_LC_P2P, 7)
/** Logs related to the Sessions service */
PROCESS_CATEGORY(LogEOSSessions, EOS_LC_Sessions, 8)
/** Logs related to rate limiting */
PROCESS_CATEGORY(LogEOSRateLimiter, EOS_LC_RateLimiter, 9)
/** Logs related to the PlayerDataStorage service */
PROCESS_CATEGORY(LogEOSPlayerDataStorage, EOS_LC_PlayerDataStorage, 10)
/** Logs related to sdk analytics */
PROCESS_CATEGORY(LogEOSAnalytics, EOS_LC_Analytics, 11)
/** Logs related to the messaging service */
PROCESS_CATEGORY(LogEOSMessaging, EOS_LC_Messaging, 12)
/** Logs related to the Connect service */
PROCESS_CATEGORY(LogEOSConnect, EOS_LC_Connect, 13)
/** Logs related to the Overlay */
PROCESS_CATEGORY(LogEOSOverlay, EOS_LC_Overlay, 14)
/** Logs related to the Achievements service */
PROCESS_CATEGORY(LogEOSAchievements, EOS_LC_Achievements, 15)
/** Logs related to the Stats service */
PROCESS_CATEGORY(LogEOSStats, EOS_LC_Stats, 16)
/** Logs related to the UI service */
PROCESS_CATEGORY(LogEOSUI, EOS_LC_UI, 17)
/** Logs related to the lobby service */
PROCESS_CATEGORY(LogEOSLobby, EOS_LC_Lobby, 18)
/** Logs related to the Leaderboards service */
PROCESS_CATEGORY(LogEOSLeaderboards, EOS_LC_Leaderboards, 19)
/** Logs related to an internal Keychain feature that the authentication interfaces use */
PROCESS_CATEGORY(LogEOSKeychain, EOS_LC_Keychain, 20)
/** Logs related to integrated platforms */
PROCESS_CATEGORY(LogEOSIntegratedPlatform, EOS_LC_IntegratedPlatform, 21)
/** Logs related to Title Storage */
PROCESS_CATEGORY(LogEOSTitleStorage, EOS_LC_TitleStorage, 22)
/** Logs related to the Mods service */
PROCESS_CATEGORY(LogEOSMods, EOS_LC_Mods, 23)
/** Logs related to the Anti-Cheat service */
PROCESS_CATEGORY(LogEOSAntiCheat, EOS_LC_AntiCheat, 24)
/** Logs related to reports client. */
PROCESS_CATEGORY(LogEOSReports, EOS_LC_Reports, 25)
/** Logs related to the Sanctions service */
PROCESS_CATEGORY(LogEOSSanctions, EOS_LC_Sanctions, 26)
/** Logs related to the Progression Snapshot service */
PROCESS_CATEGORY(LogEOSProgressionSnapshots, EOS_LC_ProgressionSnapshots, 27)
/** Logs related to the Kids Web Services integration */
PROCESS_CATEGORY(LogEOSKWS, EOS_LC_KWS, 28)
/** Logs related to the RTC API */
PROCESS_CATEGORY(LogEOSRTC, EOS_LC_RTC, 29)
/** Logs related to the RTC Admin API */
PROCESS_CATEGORY(LogEOSRTCAdmin, EOS_LC_RTCAdmin, 30)
/** Logs related to the Custom Invites API */
PROCESS_CATEGORY(LogEOSCustomInvites, EOS_LC_CustomInvites, 31)
/** Logs related to EOS HTTP activity */
PROCESS_CATEGORY(LogEOSHTTP, EOS_LC_HTTP, 41)

#endif // defined(PROCESS_CATEGORY)

#if defined(PROCESS_CATEGORY_LAST)

/** Not a real log category. Used by EOS_Logging_SetLogLevel to set the log level for all categories at the same time */
PROCESS_CATEGORY_LAST(Invalid, EOS_LC_ALL_CATEGORIES, 0x7fffffff)

#endif // PROCESS_CATEGORY_LAST

#if !defined(PROCESS_CATEGORY) && !defined(PROCESS_CATEGORY_LAST)
#error "eos_logging_categories.h requires PROCESS_CATEGORY(CategoryName, EOSCategoryLabel, EOSCategoryValue) and/or PROCESS_CATEGORY_LAST(CategoryName, EOSCategoryLabel, EOSCategoryValue) to be defined before inclusion."
#endif // !defined(PROCESS_CATEGORY) && !defined(PROCESS_CATEGORY_LAST)
