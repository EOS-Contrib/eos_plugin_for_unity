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

#include <pch.h>
#include <filesystem>
#include <string>

namespace pew::eos::io_helpers
{
    TCHAR* get_path_to_module(HMODULE module)
    {
        DWORD module_path_length = 128;
        TCHAR* module_path = static_cast<TCHAR*>(malloc(module_path_length * sizeof(TCHAR)));

        if (!module_path) {
            return nullptr; // Failed to allocate memory
        }

        while (true) {
            DWORD buffer_length = GetModuleFileName(module, module_path, module_path_length);

            if (buffer_length > 0 && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
                // Successfully retrieved path
                break;
            }

            // Handle insufficient buffer case
            module_path_length += 20;
            TCHAR* new_module_path = static_cast<TCHAR*>(realloc(module_path, module_path_length * sizeof(TCHAR)));
            if (!new_module_path) {
                free(module_path);
                return nullptr; // Memory allocation failure
            }
            module_path = new_module_path;
        }

        return module_path;
    }

    std::wstring get_path_to_module_as_string(HMODULE module)
    {
        wchar_t* module_path = get_path_to_module(module);

        std::wstring module_file_path_string(module_path);
        free(module_path);
        return module_file_path_string;
    }

    std::filesystem::path get_path_relative_to_current_module(const std::filesystem::path& relative_path)
    {
        HMODULE this_module = nullptr;
        if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCWSTR)&get_path_relative_to_current_module, &this_module) || !this_module) {
            return {};
        }

        std::wstring module_file_path_string = get_path_to_module_as_string(this_module);
        return std::filesystem::path(module_file_path_string).remove_filename() / relative_path;
    }

    std::string get_basename(const std::string& path)
    {
        std::string filename;
        filename.resize(path.length() + 1);
        _splitpath_s(path.c_str(), NULL, 0, NULL, 0, filename.data(), filename.size(), NULL, 0);

        return filename;
    }
} // namespace pew::eos::io_helpers
