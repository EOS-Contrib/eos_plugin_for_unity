#pragma once
// Define PEW_EOS_EXPORT for exporting/importing symbols
#ifdef _WIN32
// PEW_EOS_EXPORT is defined *only* in the pre-processing scripting directives
// for GfxPluginNativeRender's build process. When PEW_EOS_EXPORT is defined,
// using the PEW_EOS_API macro in class or struct definitions will affect the
// export of those objects. However, when other projects reference any of the
// files within this project - the PEW_EOS_EXPORT define will not be defined,
// and (as is appropriate for the context in which another project is
// referencing this one) the macro will specify that the struct or class should
// be *imported*, not *exported*.
// This is standard practice for C++ DLL projects.
#ifdef PEW_EOS_EXPORT
#define PEW_EOS_API __declspec(dllexport)
#else
#define PEW_EOS_API __declspec(dllimport)
#endif
#else
#define PEW_EOS_API // Empty for non-Windows platforms
#endif

/**
 * \brief Use this macro on the signature of a method that is to be exposed to
 * callers external to the dll. Used exclusively for static methods.
 * \param return_value Syntax to affect the exposure of the method to callers
 * external to the DLL.
 */
#define PEW_EOS_API_FUNC(return_value) extern "C" PEW_EOS_API return_value __stdcall
