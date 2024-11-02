#ifndef EOS_LIBRARY_HELPERS_H
#define EOS_LIBRARY_HELPERS_H
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

#include <eos_logging.h>
#include <filesystem>

enum class EOS_ELogLevel;
enum class EOS_ELogCategory;

namespace playeveryware::eos::eos_library_helpers
{
    typedef EOS_EResult(EOS_CALL* EOS_Initialize_t)(const EOS_InitializeOptions* Options);
    typedef EOS_EResult(EOS_CALL* EOS_Shutdown_t)();
    typedef EOS_HPlatform(EOS_CALL* EOS_Platform_Create_t)(const EOS_Platform_Options* Options);
    typedef EOS_EResult(EOS_CALL* EOS_Logging_SetCallback_t)(EOS_LogMessageFunc Callback);
    typedef EOS_EResult(EOS_CALL* EOS_Logging_SetLogLevel_t)(EOS_ELogCategory LogCategory, EOS_ELogLevel LogLevel);
    typedef EOS_EResult(*EOS_IntegratedPlatformOptionsContainer_Add_t)(EOS_HIntegratedPlatformOptionsContainer Handle, const EOS_IntegratedPlatformOptionsContainer_AddOptions* InOptions);
    typedef EOS_EResult(*EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t)(const EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions* Options, EOS_HIntegratedPlatformOptionsContainer* OutIntegratedPlatformOptionsContainerHandle);
    typedef void (*EOS_IntegratedPlatformOptionsContainer_Release_t)(EOS_HIntegratedPlatformOptionsContainer IntegratedPlatformOptionsContainerHandle);

    inline EOS_Initialize_t EOS_Initialize_ptr;
    inline EOS_Shutdown_t EOS_Shutdown_ptr;
    inline EOS_Platform_Create_t EOS_Platform_Create_ptr;
    inline EOS_Logging_SetCallback_t EOS_Logging_SetCallback_ptr;
    inline EOS_Logging_SetLogLevel_t EOS_Logging_SetLogLevel_ptr;
    inline EOS_IntegratedPlatformOptionsContainer_Add_t EOS_IntegratedPlatformOptionsContainer_Add_ptr;
    inline EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_ptr;
    inline EOS_IntegratedPlatformOptionsContainer_Release_t EOS_IntegratedPlatformOptionsContainer_Release_ptr;

    inline void* s_eos_sdk_lib_handle;
    inline void* s_eos_sdk_overlay_lib_handle;

    inline EOS_HPlatform eos_platform_handle;

    /**
     * @brief Loads a dynamic library from the specified file path.
     *
     * Attempts to load the library at the specified path and returns a handle to it.
     * On Windows, it uses `LoadLibrary` to perform the loading.
     *
     * @param library_path The file path to the library to load.
     * @return A handle to the loaded library, or `nullptr` if loading fails.
     */
    void* load_library_at_path(const std::filesystem::path& library_path);

    /**
     * @brief Retrieves a function pointer by name from a loaded library.
     *
     * Uses the provided library handle to obtain the address of a specified function.
     * On Windows, it uses `GetProcAddress` to retrieve the function pointer.
     *
     * @param library_handle A handle to the loaded library.
     * @param function The name of the function to retrieve.
     * @return A pointer to the specified function, or `nullptr` if not found.
     */
    void* load_function_with_name(void* library_handle, const char* function);

    /**
     * @brief Retrieves a function pointer of a specified type from a loaded library.
     *
     * This templated function casts the retrieved function pointer to the specified type.
     *
     * @tparam T The type of the function pointer.
     * @param library_handle A handle to the loaded library.
     * @param function The name of the function to retrieve.
     * @return A pointer to the specified function cast to the specified type.
     */
    template<typename T>
    T load_function_with_name(void* library_handle, const char* function)
    {
        return reinterpret_cast<T>(load_function_with_name(library_handle, function));
    }

    /**
     * @brief Chooses a string based on the platform's bitness.
     *
     * Returns `choice_if_32bit` if the platform is 32-bit; otherwise, returns `choice_if_else`.
     *
     * @param choice_if_32bit The string to return if the platform is 32-bit.
     * @param choice_if_else The string to return otherwise.
     * @return The appropriate string based on the platform bitness.
     */
    const char* pick_if_32bit_else(const char* choice_if_32bit, const char* choice_if_else);

    /**
     * @brief Loads EOS SDK function pointers from the loaded EOS SDK library.
     *
     * Maps specific function names from the loaded EOS SDK library to internal pointers, allowing
     * the library to call various EOS SDK functions.
     */
    void FetchEOSFunctionPointers();

    /**
     * @brief Queries a registry key for a specific value on Windows.
     *
     * Attempts to read a value from the specified registry key and subkey. It supports both
     * 32-bit and 64-bit registry views.
     *
     * @param InKey The registry key handle.
     * @param InSubKey The name of the subkey to query.
     * @param InValueName The name of the value to retrieve.
     * @param OutData The output parameter to store the retrieved data.
     * @return `true` if the value was successfully retrieved, `false` otherwise.
     */
    bool QueryRegKey(const HKEY InKey, const TCHAR* InSubKey, const TCHAR* InValueName, std::wstring& OutData);

    /**
     * @brief Unloads a previously loaded dynamic library.
     *
     * Frees the handle to a loaded library, releasing associated resources.
     * On Windows, it uses `FreeLibrary` to unload the library.
     *
     * @param library_handle The handle to the library to unload.
     */
    void unload_library(void* library_handle);

}
#endif
