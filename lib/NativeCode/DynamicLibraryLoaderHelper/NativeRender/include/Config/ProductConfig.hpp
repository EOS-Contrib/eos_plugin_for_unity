#ifndef PRODUCT_CONFIG_HPP
#define PRODUCT_CONFIG_HPP

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

#include <vector>

#include "include/Config/Config.hpp"
#include "include/Config/ProductionEnvironments.hpp"
#include "include/Config/ClientCredentials.hpp"

namespace pew::eos::config
{
    struct ProductConfig final : Config
    {
        /**
         * \brief Name of the product.
         */
        std::string product_name;

        /**
         * \brief Product id.
         */
        std::string product_id;

        /**
         * \brief Version of the product.
         */
        std::string product_version;

        /**
         * \brief Contains information regarding all of the production
         * environments defined for the product.
         */
        ProductionEnvironments environments;

        /**
         * \brief Contains a list of all the client definitions that have been
         * configured for the product.
         */
        std::vector<ClientCredentials> clients;

        ~ProductConfig() = default;
    private:
        // Makes the ProductConfig constructor accessible to the Config class.
        friend struct Config;
        ProductConfig() : Config("eos_product_config.json") {}
        
        void parse_json_element(const std::string& name, json_value_s& value) override
        {
            if (name == "ProductName")
            {
                product_name = json_value_as_string(&value)->string;
            }
            else if (name == "ProductId")
            {
                product_id = json_value_as_string(&value)->string;
            }
            else if (name == "ProductVersion")
            {
                product_version = json_value_as_string(&value)->string;
            }
            else if (name == "Clients")
            {
                clients = parse_json_array<ClientCredentials>(value);
            }
            else if (name == "Environments")
            {
                // Parse environments
                auto parsed_environments = ProductionEnvironments();
                parsed_environments.from_json(value);
            }
        }
    };
}

#endif
