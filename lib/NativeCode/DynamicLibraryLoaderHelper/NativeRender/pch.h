// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
// Windows Header Files
#include <windows.h>

#if _WIN32 || _WIN64
#define PLATFORM_WINDOWS 1
#if _WIN64
#define PLATFORM_64BITS 1
#else
#define PLATFORM_32BITS 1
#endif
#endif

#define STATIC_EXPORT(return_type) extern "C" return_type

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#if PLATFORM_WINDOWS
#include "processenv.h"
#endif

#include "eos_sdk.h"
#include "Windows/eos_Windows.h"

#define SHOW_DIALOG_BOX_ON_WARN 0
#define ENABLE_DLL_BASED_EOS_CONFIG 1
#define XAUDIO2_DLL_NAME "xaudio2_9redist.dll"

#define EOS_WINDOWS_CONFIG_FILENAME "eos_windows_config.json"
#define EOS_PRODUCT_CONFIG_FILENAME "eos_product_config.json"

#define EOS_STEAM_CONFIG_FILENAME "eos_steam_config.json"
#define EOS_LOGLEVEL_CONFIG_FILENAME "log_level_config.json"

//#define RESTRICT __restrict

#endif //PCH_H
