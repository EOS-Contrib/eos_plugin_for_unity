#ifndef PEW_EOS_DEFINES_H
#define PEW_EOS_DEFINES_H
/*
 * Copyright (c) 2021 PlayEveryWare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
// Specify export
#define PEW_EOS_API __declspec(dllexport)
#else
// Specify import
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

#endif