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
#include "json_helpers.h"
#include <filesystem>
#include "logging.h"

namespace pew::eos::json_helpers
{
    double json_value_as_double(json_value_s* value, double default_value)
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

    uint64_t json_value_as_uint64(json_value_s* value, uint64_t default_value)
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

    uint32_t json_value_as_uint32(json_value_s* value, uint32_t default_value)
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

    json_value_s* read_config_json_as_json_from_path(std::filesystem::path path_to_config_json)
    {
        logging::log_inform(("json path" + string_helpers::to_utf8_str(path_to_config_json)).c_str());
        const uintmax_t config_file_size = std::filesystem::file_size(path_to_config_json);
        if (config_file_size > SIZE_MAX)
        {
            throw std::filesystem::filesystem_error("File is too large", std::make_error_code(std::errc::file_too_large));
        }

        FILE* file = nullptr;
        errno_t config_file_error = _wfopen_s(&file, path_to_config_json.wstring().c_str(), L"r");
        char* buffer = static_cast<char*>(calloc(1, config_file_size));

        const size_t bytes_read = fread(buffer, 1, config_file_size, file);
        fclose(file);
        struct json_value_s* config_json = json_parse(buffer, bytes_read);
        free(buffer);

        return config_json;
    }
}
