#ifndef COMMON_HPP
#define COMMON_HPP

#pragma once
#include <codecvt>
#include <filesystem>
#include <sstream>
#include <string>
#include <iostream>

#include "PlatformConfig.h"
#include "ProductConfig.h"

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
    inline bool try_get_command_line_argument(const std::vector<std::string>& arguments, std::string& value, const Flags&... args)
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
    inline static void apply_cli_arguments(config::PlatformConfig& platform_config, const config::ProductConfig& product_config)
    {
        //support sandbox and deployment id override via command line arguments
        std::stringstream argument_stream = std::stringstream(GetCommandLineA());
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

    inline TCHAR* get_path_to_module(HMODULE module)
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

    inline std::wstring get_path_to_module_as_string(HMODULE module)
    {
        wchar_t* module_path = get_path_to_module(module);

        std::wstring module_file_path_string(module_path);
        free(module_path);
        return module_file_path_string;
    }

    inline std::filesystem::path get_path_relative_to_current_module(const std::filesystem::path& relative_path)
    {
        HMODULE this_module = nullptr;
        if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCWSTR)&get_path_relative_to_current_module, &this_module) || !this_module) {
            return {};
        }

        std::wstring module_file_path_string = get_path_to_module_as_string(this_module);
        return std::filesystem::path(module_file_path_string).remove_filename() / relative_path;
    }

    inline std::string trim(const std::string& str)
    {
        const auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
        const auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();

        if (start < end)
        {
            return std::basic_string<char>(start, end);
        }
        else
        {
            return "";
        }
    }

    inline std::vector<std::string> split_and_trim(const std::string& input, char delimiter = ',')
    {
        std::vector<std::string> result;
        std::stringstream ss(input);
        std::string item;

        while (std::getline(ss, item, delimiter))
        {
            std::string trimmedItem = trim(item);
            if (!trimmedItem.empty())
            {
                result.push_back(trimmedItem);
            }
        }

        return result;
    }

    inline bool create_timestamp_str(char* final_timestamp, size_t final_timestamp_len)
    {
        constexpr size_t buffer_len = 32;
        char buffer[buffer_len];

        if (buffer_len > final_timestamp_len)
        {
            return false;
        }

        const time_t raw_time = time(nullptr);
        tm time_info = { 0 };

        timespec time_spec = { 0 };
        timespec_get(&time_spec, TIME_UTC);
        localtime_s(&time_info, &raw_time);

        strftime(buffer, buffer_len, "%Y-%m-%dT%H:%M:%S", &time_info);
        const long milliseconds = static_cast<long>(round(time_spec.tv_nsec / 1.0e6));
        snprintf(final_timestamp, final_timestamp_len, "%s.%03ld", buffer, milliseconds);

        return true;
    }

    inline size_t utf8_str_bytes_required_for_wide_str(const wchar_t* wide_str, int wide_str_len)
    {
        const int bytes_required = WideCharToMultiByte(CP_UTF8, 0, wide_str, wide_str_len, NULL, 0, NULL, NULL);

        if (bytes_required < 0)
        {
            return 0;
        }

        return bytes_required;
    }

    // wide_str must be null terminated if wide_str_len is passed
    inline bool copy_to_utf8_str_from_wide_str(char* RESTRICT utf8_str, size_t utf8_str_len, const wchar_t* RESTRICT wide_str, int wide_str_len)
    {
        if (utf8_str_len > INT_MAX)
        {
            return false;
        }

        WideCharToMultiByte(CP_UTF8, 0, wide_str, wide_str_len, utf8_str, static_cast<int>(utf8_str_len), NULL, NULL);

        return true;
    }

    inline char* create_utf8_str_from_wide_str(const wchar_t* wide_str)
    {
        const int wide_str_len = static_cast<int>(wcslen(wide_str)) + 1;
        const int bytes_required = static_cast<int>(utf8_str_bytes_required_for_wide_str(wide_str, wide_str_len));
        auto to_return = static_cast<char*>(malloc(bytes_required));

        if (!copy_to_utf8_str_from_wide_str(to_return, bytes_required, wide_str, wide_str_len))
        {
            free(to_return);
            to_return = NULL;
        }

        return to_return;
    }

    inline wchar_t* create_wide_str_from_utf8_str(const char* utf8_str)
    {
        const int chars_required = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
        auto* to_return = static_cast<wchar_t*>(malloc(chars_required * sizeof(wchar_t)));
        const int utf8_str_len = static_cast<int>(strlen(utf8_str));

        MultiByteToWideChar(CP_UTF8, 0, utf8_str, utf8_str_len, to_return, chars_required);

        return to_return;
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

    inline static char* get_cache_directory()
    {
        static char* s_tempPathBuffer = NULL;

        if (s_tempPathBuffer == NULL)
        {
            WCHAR tmp_buffer = 0;
            DWORD buffer_size = GetTempPathW(1, &tmp_buffer) + 1;
            WCHAR* lpTempPathBuffer = (TCHAR*)malloc(buffer_size * sizeof(TCHAR));
            GetTempPathW(buffer_size, lpTempPathBuffer);

            s_tempPathBuffer = create_utf8_str_from_wide_str(lpTempPathBuffer);
            free(lpTempPathBuffer);
        }

        return s_tempPathBuffer;
    }

}

#endif
