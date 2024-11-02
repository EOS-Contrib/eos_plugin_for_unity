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

 /**
  * @file io_helpers.h
  * @brief Helper functions for file I/O and path operations.
  *
  * This file provides utility functions for managing file paths, including
  * obtaining the path to the current module and resolving relative paths.
  */

namespace playeveryware::eos::io_helpers
{
    static TCHAR* get_path_to_module(HMODULE module)
    {
        DWORD module_path_length = 128;
        TCHAR* module_path = (TCHAR*)malloc(module_path_length * sizeof(TCHAR));

        DWORD buffer_length = 0;
        DWORD GetModuleFileName_last_error = 0;

        do
        {
            buffer_length = GetModuleFileName(module, module_path, module_path_length);
            GetModuleFileName_last_error = GetLastError();
            SetLastError(NOERROR);

            if (GetModuleFileName_last_error == ERROR_INSUFFICIENT_BUFFER)
            {
                buffer_length = 0;
                module_path_length += 20;
                module_path = (TCHAR*)realloc(module_path, module_path_length * sizeof(TCHAR));
            }
        } while (buffer_length == 0);

        return module_path;
    }

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
    static std::wstring get_path_to_module_as_string(HMODULE module)
    {
        wchar_t* module_path = get_path_to_module(module);

        std::wstring module_file_path_string(module_path);
        free(module_path);
        return module_file_path_string;
    }

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
    static std::filesystem::path get_path_relative_to_current_module(const std::filesystem::path& relative_path)
    {
        HMODULE this_module = nullptr;
        if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCWSTR)&get_path_relative_to_current_module, &this_module) || !this_module) {
            return {};
        }

        std::wstring module_file_path_string = get_path_to_module_as_string(this_module);
        return std::filesystem::path(module_file_path_string).remove_filename() / relative_path;
    }

    /**
     * @brief Extracts the filename from a given path.
     *
     * This function returns only the filename portion of a full path, excluding directories.
     *
     * @param path The full path as a string.
     * @return The filename component of the provided path.
     */
    static std::string get_basename(const std::string& path)
    {
        std::string filename;
        filename.resize(path.length() + 1);
        _splitpath_s(path.c_str(), NULL, 0, NULL, 0, filename.data(), filename.size(), NULL, 0);

        return filename;
    }
} // namespace playeveryware::eos::io_helpers
#endif
