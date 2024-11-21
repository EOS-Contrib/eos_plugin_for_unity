#ifndef COMMON_HPP
#define COMMON_HPP

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
#include <codecvt>
#include <filesystem>
#include <sstream>
#include <string>
#include <iostream>

#include "headers/Config/PlatformConfig.h"
#include "headers/Config/ProductConfig.h"

namespace pew::eos::common
{
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
        std::vector<std::string> flag_options = { args... };

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

    /**
     * \brief Applies any command line arguments that may have been provided.
     * \param platform_config The platform config whose values may need to be
     * overridden by command line arguments.
     * \param product_config The product config. This is used to warn the user if
     * the provided sandbox id or deployment id is not defined in the product
     * config. If they are not defined, they will still be applied.
     */
    static void apply_cli_arguments(config::PlatformConfig& platform_config, const config::ProductConfig& product_config)
    {
        //support sandbox and deployment id override via command line arguments
        auto argument_stream = std::stringstream(GetCommandLineA());
        const std::istream_iterator<std::string> argument_stream_begin(argument_stream);
        const std::istream_iterator<std::string> argument_stream_end;
        const std::vector argument_strings(argument_stream_begin, argument_stream_end);

        std::string sandbox_id_override;
        if (try_get_command_line_argument(argument_strings, sandbox_id_override, "epicsandboxid", "eossandboxid"))
        {
            if (!product_config.environments.is_sandbox_defined(sandbox_id_override))
            {
                std::cerr << "Sandbox Id \"" << sandbox_id_override << "\" was provided on the command line, but is not found in the product config. Attempting to use it regardless." << std::endl;
            }
            platform_config.deployment.sandbox.id = sandbox_id_override;
        }

        std::string deployment_id_override;
        if (try_get_command_line_argument(argument_strings, deployment_id_override, "eosdeploymentid", "epicdeploymentid"))
        {
            if (!product_config.environments.is_deployment_defined(deployment_id_override))
            {
                std::cerr << "Deployment Id \"" << deployment_id_override << "\" was provided on the command line, but is not found in the product config. Attempting to use it regardless." << std::endl;
            }
            platform_config.deployment.id = deployment_id_override;
        }
    }

    inline std::filesystem::path get_path_relative_to_current_module(const std::filesystem::path& relative_path)
    {
        wchar_t module_path[MAX_PATH];
        HMODULE this_module = nullptr;

        // Retrieve the handle to the current module
        if (!GetModuleHandleEx(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&get_path_relative_to_current_module),
            &this_module) || !this_module) {
            return {}; // Return empty path on failure
        }

        // Retrieve the module file path
        if (GetModuleFileNameW(this_module, module_path, MAX_PATH) == 0) 
        {
            return {}; // Return empty path if GetModuleFileNameW fails
        }

        // Construct the full path and combine with the relative path
        return std::filesystem::path(module_path).remove_filename() / relative_path;
    }

    inline std::vector<std::string> split_and_trim(const std::string& input, const char delimiter = ',')
    {
        std::vector<std::string> result;
        std::stringstream ss(input);
        std::string item;

        while (std::getline(ss, item, delimiter)) {
            // Trim item in-place
            auto start = std::find_if_not(item.begin(), item.end(), ::isspace);
            auto end = std::find_if_not(item.rbegin(), item.rend(), ::isspace).base();

            if (start < end) {
                result.emplace_back(start, end); // Efficiently construct and add to result
            }
        }

        return result;
    }

    // wide_str must be null terminated if wide_str_len is passed
    inline char* create_utf8_str_from_wide_str(const wchar_t* wide_str)
    {
        if (!wide_str) return nullptr;

        const int wide_str_len = static_cast<int>(wcslen(wide_str)) + 1;
        const int bytes_required = WideCharToMultiByte(CP_UTF8, 0, wide_str, wide_str_len, nullptr, 0, nullptr, nullptr);

        if (bytes_required <= 0) return nullptr;

        const auto utf8_str = static_cast<char*>(malloc(bytes_required));
        if (!utf8_str || WideCharToMultiByte(CP_UTF8, 0, wide_str, wide_str_len, utf8_str, bytes_required, nullptr, nullptr) == 0) 
        {
            free(utf8_str);
            return nullptr;
        }

        return utf8_str;
    }

    inline std::string to_utf8_str(const std::wstring& wide_str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string utf8_str = converter.to_bytes(wide_str);

        return utf8_str;
    }

    inline std::string to_utf8_str(const std::filesystem::path& path)
    {
        return to_utf8_str(path.native());
    }

    static char* get_cache_directory()
    {
        static char* s_tempPathBuffer = NULL;

        if (s_tempPathBuffer == NULL)
        {
            WCHAR tmp_buffer = 0;
            DWORD buffer_size = GetTempPathW(1, &tmp_buffer) + 1;
            auto lpTempPathBuffer = (TCHAR*)malloc(buffer_size * sizeof(TCHAR));
            GetTempPathW(buffer_size, lpTempPathBuffer);

            s_tempPathBuffer = create_utf8_str_from_wide_str(lpTempPathBuffer);
            free(lpTempPathBuffer);
        }

        return s_tempPathBuffer;
    }

}

#endif
