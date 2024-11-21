#ifndef DLL_WRAPPER_H
#define DLL_WRAPPER_H
/*
 * Copyright (c) 2024 PlayEveryWare
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
#include <string>
#include <type_traits>
#include "headers/logging.h"
#include <iostream>
#include "headers/PEW_EOS_Defines.h"

namespace std::filesystem
{
    class path;
}

namespace pew::eos
{
    // Macro to define a typedef and associate it with a string name
    #define REGISTER_LIBRARY_FUNCTION(Name, Typedef, TypedefName) \
            typedef Typedef; \
            template <> \
            struct TypedefToString<TypedefName> { \
                static constexpr const char* value = Name; \
            }

    template<typename T>
    struct TypedefToString;

    class PEW_EOS_API DLLWrapper
    {
    private:
#pragma warning(push)
#pragma warning(disable: 4251) // Disable C4251 warning for this member
        std::string _library_name;
#pragma warning(pop)

    protected:
        DLLWrapper(const std::string& library_name);

        ~DLLWrapper();

        // Templated helper to load and call a DLL function
        template <typename FuncType, typename... Args>
        auto call_library_function(const std::string& function_name, Args&&... args) const
            -> std::invoke_result_t<FuncType, Args...> {
            logging::log_inform("Calling " + function_name);

            // Define the function pointer
            FuncType function_ptr = nullptr;

            // Try to load the function
            if (!try_load_function(library_handle, function_name.c_str(), function_ptr)) {
                logging::log_error("Unable to load pointer to " + function_name + " function.");
                throw std::runtime_error("Failed to load function: " + function_name);
            }

            // Log the call to the library
            logging::log_inform("Calling " + _library_name + ": " + function_name);

            // Invoke the function with arguments or none, depending on Args
            if constexpr (sizeof...(Args) > 0) {
                return function_ptr(std::forward<Args>(args)...);
            }
            else {
                return function_ptr();
            }
        }

        template <typename FuncType, typename... Args>
        auto call_library_function(Args&&... args) const
            -> std::invoke_result_t<FuncType, Args...>
        {
            return call_library_function<FuncType>(TypedefToString<FuncType>::value, std::forward<Args>(args)...);
        }

    protected:
        /**
         * \brief Stores a handle to the loaded library
         */
        void* library_handle;

    private:

        /**
         * @brief Loads a dynamic library from the specified file path.
         *
         * Attempts to load the library at the specified path and returns a handle to it.
         * On Windows, it uses `LoadLibrary` to perform the loading.
         *
         * @param library_path The file path to the library to load.
         * @return A handle to the loaded library, or `nullptr` if loading fails.
         */
        virtual void* load_library_at_path(const std::filesystem::path& library_path);

        /**
         * @brief This function helps take a typedef and infer the size of the
         * arguments in order to correctly mangle the function name.
         *
         * @tparam Args The arguments whose size is being calculated
         * @return The size of the arguments 
         */
        template <typename... Args>
        static constexpr size_t calculate_argument_size()
        {
            return (sizeof(Args) + ...); // Fold expression to sum sizes
        }

        /**
         * \brief This function mangles the name of a function so a pointer to
         * the function within a library can be retrieved.
         * \tparam FuncType The type to get the mangled name for.
         * \param base_name The unmangled name of the function.
         * \return The correctly mangled function name.
         */
        template <typename FuncType>
        static constexpr const char* infer_mangled_name(const char* base_name)
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
        static bool try_load_function(void* library_handle, const char* name, T& function_ptr)
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
                logging::log_error("Failed to load function \"" + std::string(mangled_name) + "\".");
                return false;
            }

            return true;
        }
    };
}
#endif
