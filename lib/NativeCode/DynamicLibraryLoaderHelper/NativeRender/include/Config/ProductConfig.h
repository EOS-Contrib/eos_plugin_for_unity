#ifndef PRODUCT_CONFIG_H
#define PRODUCT_CONFIG_H

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
        std::string product_name;
        std::string product_id;
        std::string product_version;
        ProductionEnvironments environments;
        std::vector<ClientCredentials> clients;

    private:
        // Makes the ProductConfig constructor accessible to the Config class.
        friend struct Config;
        ProductConfig() : Config(get_config_path("eos_product_config.json")) {}
        ~ProductConfig() = default;

        void from_json(const json_value_s& json) override;
        std::filesystem::path get_config_path(const char* file_name) override;
    };
}

#endif
