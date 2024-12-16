#ifndef CLIENT_CREDENTIALS_HPP
#define CLIENT_CREDENTIALS_HPP

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

#include <string>

#include "Serializable.hpp"

namespace pew::eos::config
{
    /**
     * \brief Extends EOS_Platform_ClientCredentials to include the encryption
     * key value.
     */
    struct ClientCredentials final : Serializable
    {
        std::string client_id;
        std::string client_secret;
        std::string encryption_key;

    protected:
        void parse_json_element(const std::string& name, json_value_s& value) override
        {
            if (name == "Value")
            {
                from_json(value);
            }
            else if (name == "ClientId")
            {
                client_id = json_value_as_string(&value)->string;
            }
            else if (name == "ClientSecret")
            {
                client_secret = json_value_as_string(&value)->string;
            }
            else if (name == "EncryptionKey")
            {
                encryption_key = json_value_as_string(&value)->string;
            }
        }
    };
}
#endif
