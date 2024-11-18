#ifndef EOS_JSON_CONVERTER_METHODS_H
#define EOS_JSON_CONVERTER_METHODS_H

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

#include "include/json.hpp"

#include "eos_auth_types.h"
#include "eos_integratedplatform_types.h"
#include "eos_init.h"
#include "eos_ui_types.h"
#include "string_helpers.h"

namespace nlohmann
{
    void from_json(const json& json, EOS_EAuthScopeFlags& auth_scope_flags);
    void from_json(const json& json, EOS_EIntegratedPlatformManagementFlags& integrated_platform_management_flags);
    void from_json(const json& json, EOS_Initialize_ThreadAffinity& initialize_thread_affinity);
    void from_json(const json& json, EOS_UI_EInputStateButtonFlags& input_state_button_flags);

    static inline std::string remove(std::string input, char target)
    {
        std::string modified;
        for (char& c : input)
        {
            if (c == target) continue;
            modified += c;
        }

        return modified;
    }

    template <typename EnumType>
    EnumType str_to_enum(const std::string& str, const std::map<EnumType, std::string>& map) 
    {
        static_assert(std::is_enum<EnumType>::value, "EnumType must be an enum");
        for (const auto& [key, value] : map) 
        {
            if (value != str) continue;
            return key;
        }

        // Try replacing " " with ""
        // TODO: This is because the C# that generates the JSON is replacing "_" with "".
        //       this should be fixed in the C#
        for (const auto& [key, value] : map)
        {
            auto test = remove(value, '_');
            if (test != str) continue;
            return key;
        }

        throw std::invalid_argument("Invalid flag string: " + str);
    }

    template <typename EnumType, typename std::enable_if<std::is_enum<EnumType>::value, int>::type = 0>
    void flags_enum_from_json(const json& json, const std::map<EnumType, std::string>& string_to_flags_map, EnumType& flags_enum)
    {
        std::string str_enum_values;
        json.get_to(str_enum_values);

        // Get the comma-delimited list of strings
        auto string_values = pew::eos::string_helpers::split_and_trim(str_enum_values);

        // Iterate through them and apply to the auth scope flags.
        for (const auto& str : string_values)
        {
            auto test = str_to_enum<EnumType>(str, string_to_flags_map);
            flags_enum |= test;
        }
    }
}

#endif
