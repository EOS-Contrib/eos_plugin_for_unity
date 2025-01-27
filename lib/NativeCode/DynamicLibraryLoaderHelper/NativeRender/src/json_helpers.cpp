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

    json_value_s* read_config_json_as_json_from_path(const std::filesystem::path& path_to_config_json)
    {
        logging::log_inform("Reading json from file \"" + path_to_config_json.string() + "\".");

        const auto config_file_size_uintmax = file_size(path_to_config_json);
        if (config_file_size_uintmax > static_cast<std::uintmax_t>(std::numeric_limits<size_t>::max())) 
        {
            throw std::filesystem::filesystem_error("File is too large", std::make_error_code(std::errc::file_too_large));
        }

        // Safe cast to size_t after bounds check
        const auto config_file_size = static_cast<size_t>(config_file_size_uintmax);

        FILE* file = nullptr;
        const errno_t config_file_error = _wfopen_s(&file, path_to_config_json.wstring().c_str(), L"r");
        if (config_file_error != 0 || file == nullptr) 
        {
            throw std::runtime_error("Failed to open file: " + path_to_config_json.string());
        }

        // Use a std::vector to manage the buffer memory
        std::vector buffer(config_file_size, '\0');

        const size_t bytes_read = fread(buffer.data(), 1, config_file_size, file);

        // Close the file and check for errors
        if (fclose(file) != 0) 
        {
            logging::log_error(
                "There was an unspecified error trying to close "
                "file \"" + path_to_config_json.string() + "\".");
        }

        // Parse the JSON using the buffer
        json_value_s* config_json = json_parse(buffer.data(), bytes_read);

        return config_json;  // Return the parsed JSON object
    }
}
