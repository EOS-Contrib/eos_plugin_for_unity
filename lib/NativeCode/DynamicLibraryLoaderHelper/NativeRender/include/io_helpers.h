#ifndef IO_HELPERS_H
#define IO_HELPERS_H
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

#include <tchar.h>
#include <wchar.h>
#include <wtypes.h>

 /**
  * @file io_helpers.h
  * @brief Helper functions for file I/O and path operations.
  *
  * This file provides utility functions for managing file paths, including
  * obtaining the path to the current module and resolving relative paths.
  */

namespace pew::eos::io_helpers
{
    /**
     * @brief Retrieves the file path of a specified module as a dynamically allocated string.
     *
     * This function attempts to get the full file path of the module specified by the `module` handle.
     * If the buffer size is insufficient, it reallocates with a larger size and retries until the path
     * is successfully retrieved. The caller is responsible for freeing the returned string to avoid memory leaks.
     *
     * @param module The handle to the module whose path is to be retrieved.
     * @return A dynamically allocated `TCHAR*` containing the path to the specified module, or `nullptr` if memory allocation fails.
     *
     * @note The caller must free the returned `TCHAR*` using `free()` to prevent memory leaks.
     * @warning If the `module` handle is invalid, the function may fail or produce undefined behavior.
     */
    TCHAR* get_path_to_module(HMODULE module);

    /**
     * @brief Retrieves a path relative to the current module.
     *
     * Given a relative path, this function resolves it based on the directory
     * of the current module, allowing for easy access to resources.
     *
     * @param relative_path The relative path to resolve.
     * @return The resolved absolute path.
     *
     * @note If the module handle cannot be obtained, the function returns an empty path.
     */
    std::filesystem::path get_path_relative_to_current_module(const std::filesystem::path& relative_path);

    /**
     * @brief Extracts the filename from a given path.
     *
     * This function returns only the filename portion of a full path, excluding directories.
     *
     * @param path The full path as a string.
     * @return The filename component of the provided path.
     */
    std::string get_basename(const std::string& path);

    /**
     * @brief Retrieves the full path to a module as a wide string.
     *
     * This function fetches the absolute path of a specified module,
     * typically used for resolving file locations relative to the module.
     *
     * @param module The handle to the module.
     * @return A wide string containing the full path to the specified module.
     * @note The caller must free the returned string if dynamically allocated.
     */
    std::wstring get_path_to_module_as_string(HMODULE module);

    /**
     * \brief Gets the value of a command line argument specified by one or more possible flags.
     * \tparam Flags Options for what flags might be permissible for the value. Exclude the leading dash and the following "=" from the flags provided.
     * \param arguments All of the command-line arguments passed in.
     * \param value The value for the indicated command line argument.
     * \param args The labels that can be used to specify the command argument.
     * \return True if the argument was provided, false otherwise.
     */
    template <typename... Flags>
    bool try_get_command_line_argument(const std::vector<std::string>& arguments, std::string& value, const Flags&... args)
    {
        // This gathers the variadic parameters which represent parameter flags, any
        // of which indicate the same value that is being passed in on the command
        // line.
        const std::vector<std::string> flag_options = { args... };

        for (const auto& argument : arguments)
        {
            const std::string* match = nullptr;

            // See if the argument matches any of the flag options provided.
            for (const std::string& flag : flag_options)
            {
                // If the argument doesn't start with the flag, move to the next.
                if (!argument._Starts_with("-" + flag + "="))
                {
                    continue;
                }

                // Indicate the match and exit the loop.
                match = &flag;
                break;
            }

            // If there was a match
            if (match != nullptr)
            {
                // extract the value.
                std::string arg_value;
                arg_value = argument.substr(match->length());

                if (!arg_value.empty())
                {
                    value = arg_value;
                    return true;
                }
            }
        }

        // Return false if the value wasn't set on the command line.
        return false;
    }
} // namespace pew::eos::io_helpers
#endif
