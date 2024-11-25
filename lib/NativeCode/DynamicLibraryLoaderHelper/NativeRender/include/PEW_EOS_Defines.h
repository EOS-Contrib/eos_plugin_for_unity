#pragma once
// Define PEW_EOS_EXPORT for exporting/importing symbols
#ifdef _WIN32
#ifdef PEW_EOS_EXPORT
#define PEW_EOS_API __declspec(dllexport)
#else
#define PEW_EOS_API __declspec(dllimport)
#endif
#else
#define PEW_EOS_API // Empty for non-Windows platforms
#endif

// Define a macro for exported functions with calling convention
#define PEW_EOS_API_FUNC(return_value) extern "C" PEW_EOS_API return_value __stdcall

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

// Strip surrounding quotes
#define REMOVE_QUOTES(x) x[0] == '\"' ? (x + 1) : x
#define STRIP_QUOTES(s) (REMOVE_QUOTES(s)[strlen(REMOVE_QUOTES(s)) - 1] == '\"' ? REMOVE_QUOTES(s) : s)

