#ifndef EOS_HELPERS_H
#define EOS_HELPERS_H
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
#include <iostream>

#include "logging.h"

namespace std
{
    namespace filesystem
    {
        class path;
    }
}

namespace pew::eos
{
    namespace config
    {
        class PlatformConfig;
        class WindowsConfig;
        class ProductConfig;
        struct EOSConfig;
    }

    typedef EOS_EResult(EOS_CALL* EOS_Initialize_t)(const EOS_InitializeOptions* Options);
    typedef EOS_EResult(EOS_CALL* EOS_Shutdown_t)();
    typedef EOS_HPlatform(EOS_CALL* EOS_Platform_Create_t)(const EOS_Platform_Options* Options);
    typedef EOS_EResult(EOS_CALL* EOS_Logging_SetCallback_t)(EOS_LogMessageFunc Callback);
    typedef EOS_EResult(EOS_CALL* EOS_Logging_SetLogLevel_t)(EOS_ELogCategory LogCategory, EOS_ELogLevel LogLevel);
    typedef EOS_EResult(*EOS_IntegratedPlatformOptionsContainer_Add_t)(EOS_HIntegratedPlatformOptionsContainer Handle, const EOS_IntegratedPlatformOptionsContainer_AddOptions* InOptions);
    typedef EOS_EResult(*EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t)(const EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions* Options, EOS_HIntegratedPlatformOptionsContainer* OutIntegratedPlatformOptionsContainerHandle);
    typedef void (*EOS_IntegratedPlatformOptionsContainer_Release_t)(EOS_HIntegratedPlatformOptionsContainer IntegratedPlatformOptionsContainerHandle);

    //extern EOS_Initialize_t EOS_Initialize_ptr;
    extern EOS_Shutdown_t EOS_Shutdown_ptr;
    //extern EOS_Platform_Create_t EOS_Platform_Create_ptr;
    extern EOS_Logging_SetCallback_t EOS_Logging_SetCallback_ptr;
    extern EOS_Logging_SetLogLevel_t EOS_Logging_SetLogLevel_ptr;
    extern EOS_IntegratedPlatformOptionsContainer_Add_t EOS_IntegratedPlatformOptionsContainer_Add_ptr;
    extern EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_ptr;
    extern EOS_IntegratedPlatformOptionsContainer_Release_t EOS_IntegratedPlatformOptionsContainer_Release_ptr;

    extern void* s_eos_sdk_lib_handle;
    extern void* s_eos_sdk_overlay_lib_handle;

    extern EOS_HPlatform eos_platform_handle;

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

    // Helper to calculate the total size of all function arguments
    template <typename... Args>
    constexpr size_t calculate_argument_size()
    {
        return (sizeof(Args) + ...); // Fold expression to sum sizes
    }

    // Function to infer mangling from a typedef
    template <typename FuncType>
    constexpr const char* infer_mangled_name(const char* base_name)
    {
#if defined(_WIN32) && !defined(_WIN64)
        if constexpr (std::is_same_v<std::remove_pointer_t<FuncType>, int __stdcall(int, float)>)
        {
            constexpr size_t stack_size = calculate_argument_size<int, float>();
            static char mangled_name[256];
            snprintf(mangled_name, sizeof(mangled_name), "_%s@%zu", base_name, stack_size);
            return mangled_name;
        }
        else
        {
            return base_name; // Default for non-__stdcall or unhandled cases
        }
#else
        return base_name; // Non-32-bit Windows platforms
#endif
    }

    /**
     * @brief Retrieves a function pointer of a specified type from a loaded library.
     *
     * This templated function casts the retrieved function pointer to the specified type.
     *
     * @tparam T The type of the function pointer.
     * @param library_handle A handle to the loaded library.
     * @param name The name of the function to retrieve.
     * @return A pointer to the specified function cast to the specified type.
     */
    template<typename T>
    bool try_load_function(void* library_handle, const char* name, T& function_ptr)
    {
        const auto mangled_name = infer_mangled_name<T>(name);
        void* to_return = nullptr;
#if PLATFORM_WINDOWS
        const auto handle = static_cast<HMODULE>(library_handle);
        to_return = static_cast<void*>(GetProcAddress(handle, mangled_name));
#endif
        function_ptr = reinterpret_cast<T>(to_return);

        if (function_ptr == nullptr)
        {
            std::cerr << "Failed to load function \"" << mangled_name << "\"." << std::endl;
            return false;
        }

        return true;
    }

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
     * Attempts to try_read a value from the specified registry key and subkey. It supports both
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

    /**
     * @brief Retrieves the path to the overlay DLL.
     *
     * Attempts to retrieve the overlay DLL path from the system registry on Windows.
     *
     * @param[out] OutDllPath The output parameter where the overlay DLL path is stored.
     * @return `true` if the DLL path was found and exists; otherwise, `false`.
     */
    static bool get_overlay_dll_path(std::filesystem::path* OutDllPath);

    /**
     * @brief Retrieves the EOS platform interface handle.
     *
     * Provides access to the EOS platform interface handle managed by the library.
     *
     * @return A pointer to the EOS platform interface handle.
     */
    DLL_EXPORT(void*) EOS_GetPlatformInterface();

    /**
     * @brief Sets the log level for the EOS SDK based on the configuration file.
     *
     * Reads the log level configuration file and applies specified log levels to each
     * log category in the EOS SDK. If the configuration file is missing or the entries are
     * invalid, default log levels are used.
     */
    void eos_set_loglevel_via_config();

    /**
     * @brief Logs EOS platform options for debugging purposes.
     *
     * Logs various properties of the EOS platform options structure, including version,
     * server mode, flags, cache directory, and client credentials, for debugging.
     *
     * @param platform_options The EOS platform options structure to log.
     */
    void EOS_Platform_Options_debug_log(const EOS_Platform_Options& platform_options);

    /**
     * @brief Initializes the EOS SDK and creates an EOS platform using the specified configurations.
     *
     * Sets up and initializes the EOS SDK using the provided configuration. Sets log levels and
     * a logging callback if configured. If initialization fails, an error is logged.
     *
     * @param platform_config The configuration for Windows.
     * @param product_config The configuration for the product.
     */
    CONFIG_API void load_eos(const config::PlatformConfig& platform_config, const config::ProductConfig& product_config);

    /**
     * @brief Initializes the EOS SDK with the provided configuration.
     *
     * Sets up and initializes the EOS SDK using the provided configuration. Sets log levels and
     * a logging callback if configured. If initialization fails, an error is logged.
     *
     * @param platform_config The configuration for Windows.
     * @param product_config The configuration for the product.
     */
    void eos_init(const config::PlatformConfig& platform_config, const config::ProductConfig& product_config);

    /**
     * @brief Creates an EOS platform using the specified configuration.
     *
     * Configures and creates an EOS platform instance. This includes setting up RTC options,
     * integrated platform options, and other settings defined in the configuration.
     *
     * @param platform_config The configuration object containing EOS platform settings.
     * @param product_config The configuration object containing product settings.
     */
    void eos_create(const config::PlatformConfig& platform_config, const config::ProductConfig& product_config);

    // Templated helper to load and call a DLL function
    template <typename FuncType, typename... Args>
    auto call_library_function(const std::string& function_name, void* library_handle, Args&&... args)
        -> std::invoke_result_t<FuncType, Args...> {
        logging::log_inform("Calling " + function_name);

        // Define the function pointer
        FuncType function_ptr = nullptr;

        // Try to load the function
        if (!try_load_function(library_handle, function_name.c_str(), function_ptr)) {
            logging::log_error("Unable to load pointer to " + function_name + " function.");
            throw std::runtime_error("Failed to load function: " + function_name);
        }

        // Call the loaded function and store the result
        auto result = function_ptr(std::forward<Args>(args)...);

        // Clean up the function pointer
        function_ptr = nullptr;

        return result;
    }

    template <typename FuncType>
    auto call_library_function(const std::string& function_name, void* library_handle)
        -> std::invoke_result_t<FuncType> {
        logging::log_inform("Calling " + function_name);

        // Define the function pointer
        FuncType function_ptr = nullptr;

        // Try to load the function
        if (!try_load_function(library_handle, function_name.c_str(), function_ptr)) {
            logging::log_error("Unable to load pointer to " + function_name + " function.");
            throw std::runtime_error("Failed to load function: " + function_name);
        }

        // Call the loaded function and store the result
        auto result = function_ptr();

        // Clean up the function pointer
        function_ptr = nullptr;

        return result;
    }
}
#endif
