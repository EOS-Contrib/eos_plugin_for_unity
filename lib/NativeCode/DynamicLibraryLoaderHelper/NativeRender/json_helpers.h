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
#include <map>

#include "json.h"

namespace playeveryware::eos::json_helpers
{
    /**
     * \brief
     * Collects flag values from either a JSON array of strings, or a
     * comma-delimited list of values (like how Newtonsoft outputs things).
     *
     * \tparam T The type parameter (the enum type).
     *
     * \param
     * strings_to_enum_values A collection that maps string values to enum values.
     *
     * \param
     * default_value The default value to assign the flags if no matching string is
     * found.
     *
     * \param
     * iter The iterator into the json object element.
     *
     * \return A single flag value.
     */
    template<typename T>
    static T collect_flags(const std::map<std::string, T>* strings_to_enum_values, T default_value, json_object_element_s* iter)
    {
        T flags_to_return = static_cast<T>(0);
        bool flag_set = false;

        // Stores the string values that are within the JSON
        std::vector<std::string> string_values;

        // If the string values are stored as a JSON array of strings
        if (iter->value->type == json_type_array)
        {
            // Do things if the type is an array
            json_array_s* flags = json_value_as_array(iter->value);
            for (auto e = flags->start; e != nullptr; e = e->next)
            {
                string_values.emplace_back(json_value_as_string(e->value)->string);
            }
        }
        // If the string values are comma delimited
        else if (iter->value->type == json_type_string)
        {
            const std::string flags = json_value_as_string(iter->value)->string;
            string_values = string_helpers::split_and_trim(flags);
        }

        // Iterate through the string values
        for (const auto str : string_values)
        {
            // Skip if the string is not in the map
            if (strings_to_enum_values->find(str.c_str()) == strings_to_enum_values->end())
            {
                continue;
            }

            // Otherwise, append the enum value
            flags_to_return |= strings_to_enum_values->at(str.c_str());
            flag_set = true;
        }

        return flag_set ? flags_to_return : default_value;
    }

    //-------------------------------------------------------------------------
    double json_value_as_double(json_value_s* value, double default_value = 0.0)
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
    uint64_t json_value_as_uint64(json_value_s* value, uint64_t default_value = 0)
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
    uint32_t json_value_as_uint32(json_value_s* value, uint32_t default_value = 0)
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
    json_value_s* read_config_json_as_json_from_path(std::filesystem::path path_to_config_json)
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
