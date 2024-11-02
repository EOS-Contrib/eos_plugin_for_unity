#include <pch.h>

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

#include "logging.h"
#include "string_helpers.h"

namespace playeveryware::eos::logging
{
    //-------------------------------------------------------------------------
    const char* eos_loglevel_to_print_str(EOS_ELogLevel level)
    {
        switch (level)
        {
        case EOS_ELogLevel::EOS_LOG_Off:
            return "Off";
            break;
        case EOS_ELogLevel::EOS_LOG_Fatal:
            return "Fatal";
            break;
        case EOS_ELogLevel::EOS_LOG_Error:
            return "Error";
            break;
        case EOS_ELogLevel::EOS_LOG_Warning:
            return "Warning";
            break;
        case EOS_ELogLevel::EOS_LOG_Info:
            return "Info";
            break;
        case EOS_ELogLevel::EOS_LOG_Verbose:
            return "Verbose";
            break;
        case EOS_ELogLevel::EOS_LOG_VeryVerbose:
            return "VeryVerbose";
            break;
        default:
            return nullptr;
        }
    }

    DLL_EXPORT(void) global_log_flush_with_function(log_flush_function_t log_flush_function)
    {
        if (buffered_output.size() > 0)
        {
            for (const std::string& str : buffered_output)
            {
                log_flush_function(str.c_str());
            }
            buffered_output.clear();
        }
    }

    EOS_ELogLevel eos_loglevel_str_to_enum(const std::string& str)
    {
        auto it = LOGLEVEL_STR_MAP.find(str);
        if (it != LOGLEVEL_STR_MAP.end())
        {
            return it->second;
        }
        else
        {
            return EOS_ELogLevel::EOS_LOG_Verbose;
        }
    }

    void show_log_as_dialog(const char* log_string)
    {
#if PLATFORM_WINDOWS
        MessageBoxA(NULL, log_string, "Warning", MB_ICONWARNING);
#endif
    }

    void global_log_close()
    {
        if (log_file_s)
        {
            fclose(log_file_s);
            log_file_s = nullptr;
            buffered_output.clear();
        }
    }

    void global_logf(const char* format, ...)
    {
        if (log_file_s != nullptr)
        {
            va_list arg_list;
            va_start(arg_list, format);
            vfprintf(log_file_s, format, arg_list);
            va_end(arg_list);

            fprintf(log_file_s, "\n");
            fflush(log_file_s);
        }
        else
        {
            va_list arg_list;
            va_start(arg_list, format);
            va_list arg_list_copy;
            va_copy(arg_list_copy, arg_list);
            const size_t printed_length = vsnprintf(nullptr, 0, format, arg_list) + 1;
            va_end(arg_list);

            std::vector<char> buffer(printed_length);
            vsnprintf(buffer.data(), printed_length, format, arg_list_copy);
            va_end(arg_list_copy);
            buffered_output.emplace_back(std::string(buffer.data(), printed_length));
        }
    }

    EXTERN_C void EOS_CALL eos_log_callback(const EOS_LogMessage* message)
    {
        constexpr size_t final_timestamp_len = 32;
        char final_timestamp[final_timestamp_len] = { 0 };

        if (string_helpers::create_timestamp_str(final_timestamp, final_timestamp_len))
        {
            global_logf("%s %s (%s): %s", final_timestamp, message->Category, eos_loglevel_to_print_str(message->Level), message->Message);
        }
        else
        {
            global_logf("%s (%s): %s", message->Category, eos_loglevel_to_print_str(message->Level), message->Message);
        }

    }

    void global_log_open(const char* filename)
    {
        if (log_file_s != nullptr)
        {
            fclose(log_file_s);
            log_file_s = nullptr;
        }
        fopen_s(&log_file_s, filename, "w");

        if (buffered_output.size() > 0)
        {
            for (const std::string& str : buffered_output)
            {
                global_logf(str.c_str());
            }
            buffered_output.clear();
        }
    }

    void log_base(const char* header, const char* message)
    {
        constexpr size_t final_timestamp_len = 32;
        char final_timestamp[final_timestamp_len] = { };
        if (string_helpers::create_timestamp_str(final_timestamp, final_timestamp_len))
        {
            global_logf("%s NativePlugin (%s): %s", final_timestamp, header, message);
        }
        else
        {
            global_logf("NativePlugin (%s): %s", header, message);
        }
    }

    //-------------------------------------------------------------------------
    // TODO: If possible, hook this up into a proper logging channel.s
    void log_warn(const char* log_string)
    {
#if SHOW_DIALOG_BOX_ON_WARN
        show_log_as_dialog(log_string);
#endif
        log_base("WARNING", log_string);
    }

    void log_inform(const char* log_string)
    {
        log_base("INFORM", log_string);
    }

    void log_error(const char* log_string)
    {
        log_base("ERROR", log_string);
    }
}
