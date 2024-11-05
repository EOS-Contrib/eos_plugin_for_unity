#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H
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
#include <vector>

#include "json.h"
#include "string_helpers.h"

namespace std
{
    namespace filesystem
    {
        class path;
    }
}

namespace pew::eos::json_helpers
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

    /**
     * @brief Parses a JSON value as a `double`, with a specified default.
     *
     * Attempts to interpret the provided JSON value as a `double`. If the value cannot be parsed as a number,
     * it is treated as a string and parsed. If both attempts fail, the function returns the specified default.
     *
     * @param value The JSON value to interpret.
     * @param default_value The value to return if parsing fails.
     * @return The parsed `double` value, or `default_value` if parsing fails.
     */
    double json_value_as_double(json_value_s* value, double default_value = 0.0);

    /**
     * @brief Parses a JSON value as an unsigned 64-bit integer (`uint64_t`), with a specified default.
     *
     * Attempts to interpret the provided JSON value as a `uint64_t`. If the value cannot be parsed as a number,
     * it is treated as a string and parsed. If both attempts fail, the function returns the specified default.
     *
     * @param value The JSON value to interpret.
     * @param default_value The value to return if parsing fails.
     * @return The parsed `uint64_t` value, or `default_value` if parsing fails.
     */
    uint64_t json_value_as_uint64(json_value_s* value, uint64_t default_value = 0);

    /**
     * @brief Parses a JSON value as an unsigned 32-bit integer (`uint32_t`), with a specified default.
     *
     * Attempts to interpret the provided JSON value as a `uint32_t`. If the value cannot be parsed as a number,
     * it is treated as a string and parsed. If both attempts fail, the function returns the specified default.
     *
     * @param value The JSON value to interpret.
     * @param default_value The value to return if parsing fails.
     * @return The parsed `uint32_t` value, or `default_value` if parsing fails.
     */
    uint32_t json_value_as_uint32(json_value_s* value, uint32_t default_value = 0);

    /**
     * @brief Reads a JSON configuration file from a specified path and parses it.
     *
     * Opens and reads the specified configuration file, loads its contents into a buffer,
     * parses the buffer as JSON, and returns the parsed JSON object. If the file is too large or
     * cannot be opened, an exception is thrown.
     *
     * @param path_to_config_json The path to the JSON configuration file.
     * @return A pointer to a `json_value_s` representing the parsed JSON structure, or `nullptr` on failure.
     *
     * @throws std::filesystem::filesystem_error If the file is too large to be processed.
     *
     * @note The caller is responsible for handling and freeing the parsed JSON structure as needed.
     */
    json_value_s* read_config_json_as_json_from_path(std::filesystem::path path_to_config_json);
}
#endif
