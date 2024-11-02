#pragma once
#include "json.h"
#include "logging.h"
#include "string_helpers.h"

namespace playeveryware::eos::json_helpers
{
    //-------------------------------------------------------------------------
    inline static double json_value_as_double(json_value_s* value, double default_value = 0.0)
    {
        double val = 0.0;
        json_number_s* n = json_value_as_number(value);

        if (n != nullptr)
        {
            char* end = nullptr;
            val = strtod(n->number, &end);
        }
        else
        {
            // try to treat it as a string, then parse as long
            char* end = nullptr;
            json_string_s* val_as_str = json_value_as_string(value);

            if (val_as_str == nullptr || strlen(val_as_str->string) == 0)
            {
                val = default_value;
            }
            else
            {
                val = strtod(val_as_str->string, &end);
            }
        }

        return val;
    }

    //-------------------------------------------------------------------------
    inline static uint64_t json_value_as_uint64(json_value_s* value, uint64_t default_value = 0)
    {
        uint64_t val = 0;
        json_number_s* n = json_value_as_number(value);

        if (n != nullptr)
        {
            char* end = nullptr;
            val = strtoull(n->number, &end, 10);
        }
        else
        {
            // try to treat it as a string, then parse as long
            char* end = nullptr;
            json_string_s* val_as_str = json_value_as_string(value);
            if (val_as_str == nullptr || strlen(val_as_str->string) == 0)
            {
                val = default_value;
            }
            else
            {
                val = strtoull(val_as_str->string, &end, 10);
            }
        }

        return val;
    }


    //-------------------------------------------------------------------------
    inline static uint32_t json_value_as_uint32(json_value_s* value, uint32_t default_value = 0)
    {
        uint32_t val = 0;
        json_number_s* n = json_value_as_number(value);

        if (n != nullptr)
        {
            char* end = nullptr;
            val = strtoul(n->number, &end, 10);
        }
        else
        {
            // try to treat it as a string, then parse as long
            char* end = nullptr;
            json_string_s* val_as_str = json_value_as_string(value);

            if (val_as_str == nullptr || strlen(val_as_str->string) == 0)
            {
                val = default_value;
            }
            else
            {
                val = strtoul(val_as_str->string, &end, 10);
            }
        }

        return val;
    }

    //-------------------------------------------------------------------------
    inline static json_value_s* read_config_json_as_json_from_path(std::filesystem::path path_to_config_json)
    {
        logging::log_inform(("json path" + string_helpers::to_utf8_str(path_to_config_json)).c_str());
        uintmax_t config_file_size = std::filesystem::file_size(path_to_config_json);
        if (config_file_size > SIZE_MAX)
        {
            throw std::filesystem::filesystem_error("File is too large", std::make_error_code(std::errc::file_too_large));
        }

        FILE* file = nullptr;
        errno_t config_file_error = _wfopen_s(&file, path_to_config_json.wstring().c_str(), L"r");
        char* buffer = (char*)calloc(1, static_cast<size_t>(config_file_size));

        size_t bytes_read = fread(buffer, 1, static_cast<size_t>(config_file_size), file);
        fclose(file);
        struct json_value_s* config_json = json_parse(buffer, bytes_read);
        free(buffer);

        return config_json;
    }
}
