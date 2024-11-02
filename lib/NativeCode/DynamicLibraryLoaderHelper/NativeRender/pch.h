// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>


#if _WIN32 || _WIN64
#define PLATFORM_WINDOWS 1
#endif

#if _WIN64
#define PLATFORM_64BITS 1
#define PLATFORM_BITS_DEBUG_STR "64-bits"
#else
#define PLATFORM_32BITS 1
#define PLATFORM_BITS_DEBUG_STR "32-bits"
#endif

#define STATIC_EXPORT(return_type) extern "C" return_type


#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING


#if PLATFORM_WINDOWS
#include "processenv.h"
#endif

#include "eos_sdk.h"
//#include "eos_logging.h"

// This define exists because UWP
// Originally, this would load the library with the name as shipped by the .zip file
#define USE_PLATFORM_WITHOUT_BITS 0

#if USE_PLATFORM_WITHOUT_BITS
#define DLL_PLATFORM "-Win"
#else
#if PLATFORM_64BITS
#define DLL_PLATFORM "-Win64"
#else
#define DLL_PLATFORM "-Win32"
#endif
#endif

#if PLATFORM_64BITS
#define STEAM_DLL_NAME "steam_api64.dll"
#else
#define STEAM_DLL_NAME "steam_api.dll"
#endif

#define DLL_SUFFIX "-Shipping.dll"

#define SHOW_DIALOG_BOX_ON_WARN 0
#define ENABLE_DLL_BASED_EOS_CONFIG 1
#define OVERLAY_DLL_NAME "EOSOVH" DLL_PLATFORM DLL_SUFFIX
#define SDK_DLL_NAME "EOSSDK" DLL_PLATFORM DLL_SUFFIX
#define XAUDIO2_DLL_NAME "xaudio2_9redist.dll"

#define EOS_SERVICE_CONFIG_FILENAME "EpicOnlineServicesConfig.json"
#define EOS_STEAM_CONFIG_FILENAME "eos_steam_config.json"
#define EOS_LOGLEVEL_CONFIG_FILENAME "log_level_config.json"

#define RESTRICT __restrict

#define DLL_EXPORT(return_value) extern "C" __declspec(dllexport) return_value  __stdcall

#endif //PCH_H
