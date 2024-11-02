#pragma once
#include <config.h>
#include <filesystem>

namespace playeveryware::eos::logging
{
    static FILE* log_file_s = nullptr;
    static std::vector<std::string> buffered_output;

    //-------------------------------------------------------------------------
    static const char* eos_loglevel_to_print_str(EOS_ELogLevel level)
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

    static const std::unordered_map<std::string, EOS_ELogLevel> loglevel_str_map =
    {
        {"Off",EOS_ELogLevel::EOS_LOG_Off},
        {"Fatal",EOS_ELogLevel::EOS_LOG_Fatal},
        {"Error",EOS_ELogLevel::EOS_LOG_Error},
        {"Warning",EOS_ELogLevel::EOS_LOG_Warning},
        {"Info",EOS_ELogLevel::EOS_LOG_Info},
        {"Verbose",EOS_ELogLevel::EOS_LOG_Verbose},
        {"VeryVerbose",EOS_ELogLevel::EOS_LOG_VeryVerbose},
    };

    inline static EOS_ELogLevel eos_loglevel_str_to_enum(const std::string& str)
    {
        auto it = loglevel_str_map.find(str);
        if (it != loglevel_str_map.end())
        {
            return it->second;
        }
        else
        {
            return EOS_ELogLevel::EOS_LOG_Verbose;
        }
    }

    //-------------------------------------------------------------------------
    inline static void show_log_as_dialog(const char* log_string)
    {
#if PLATFORM_WINDOWS
        MessageBoxA(NULL, log_string, "Warning", MB_ICONWARNING);
#endif
    }


    inline static void global_log_close()
    {
        if (log_file_s)
        {
            fclose(log_file_s);
            log_file_s = nullptr;
            buffered_output.clear();
        }
    }


    //-------------------------------------------------------------------------
    inline static void global_logf(const char* format, ...)
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


    //-------------------------------------------------------------------------
    inline static void global_log_open(const char* filename)
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

    //-------------------------------------------------------------------------
    inline static bool create_timestamp_str(char* final_timestamp, size_t final_timestamp_len)
    {
        constexpr size_t buffer_len = 32;
        char buffer[buffer_len];

        if (buffer_len > final_timestamp_len)
        {
            return false;
        }

        time_t raw_time = time(NULL);
        tm time_info = { 0 };

        timespec time_spec = { 0 };
        timespec_get(&time_spec, TIME_UTC);
        localtime_s(&time_info, &raw_time);

        strftime(buffer, buffer_len, "%Y-%m-%dT%H:%M:%S", &time_info);
        long milliseconds = (long)round(time_spec.tv_nsec / 1.0e6);
        snprintf(final_timestamp, final_timestamp_len, "%s.%03ld", buffer, milliseconds);

        return true;
    }

    //-------------------------------------------------------------------------
    inline static void log_base(const char* header, const char* message)
    {
        constexpr size_t final_timestamp_len = 32;
        char final_timestamp[final_timestamp_len] = { };
        if (create_timestamp_str(final_timestamp, final_timestamp_len))
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
    inline static void log_warn(const char* log_string)
    {
#if SHOW_DIALOG_BOX_ON_WARN
        show_log_as_dialog(log_string);
#endif
        log_base("WARNING", log_string);
    }

    //-------------------------------------------------------------------------
    inline static void log_inform(const char* log_string)
    {
        log_base("INFORM", log_string);
    }

    //-------------------------------------------------------------------------
    inline static void log_error(const char* log_string)
    {
        log_base("ERROR", log_string);
    }

}
