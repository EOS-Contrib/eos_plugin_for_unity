#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

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

#include "include/nlohmann/json.hpp"

namespace pew::eos::config
{
    using namespace nlohmann;

    /**
     * \brief Helper macro for parsing from a "json" object of type nlohmann::json
     * \param key The key parse the value from.
     * \param target Where to put the value that's been parsed.
     */
    #define PARSE_FROM_JSON(key, target) try_get_to(json, key, target)

    /**
     * \brief Used to describe the functions needed to make an object both
     * serializable and deserializable from JSON.
     */
    struct CONFIG_API Serializable
    {
    protected:
        ~Serializable() = default;

        virtual void from_json(const json& json) = 0;

        /**
         * \brief Helper method to try and get a value from a JSON object at the
         * specified key.
         * \tparam T The type to get from the JSON object
         * \param json The JSON object.
         * \param key The key at which the value should be extracted.
         * \param set_to The variable to set the value of.
         * \return True if the key exists and the value was retrieved, false
         * otherwise.
         */
        template<typename T>
        static bool try_get_to(const json& json, const char* key, T& set_to)
        {
            if (json.contains(key))
            {
                json[key].get_to(set_to);
                return true;
            }

            return false;
        }
    };
}

#endif
#pragma once
