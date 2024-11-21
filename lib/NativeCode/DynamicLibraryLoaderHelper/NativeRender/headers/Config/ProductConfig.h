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

#include "headers/Config/ConfigBase.h"
#include "headers/Config/ProductionEnvironments.h"
#include "headers/Config/ClientCredentials.h"
#include "include/nlohmann/json.hpp"

namespace pew::eos::config
{
    class ProductConfig final : public ConfigBase
    {
    private:

        // Makes the ProductConfig constructor accessible to the Config class.
        friend class ConfigBase;

        bool _imported;

    protected:
        void from_json(const nlohmann::json& json) override;
        std::filesystem::path get_config_path(const char* file_name) override;

    public:
        std::string product_name;
        std::string product_id;
        std::string product_version;

        ProductConfig() : ConfigBase(get_config_path("eos_product_config.json")), _imported(false) {};
        ~ProductConfig() = default;

        // Make ProductConfig movable
        ProductConfig(ProductConfig&&) noexcept = default;
        ProductConfig& operator=(ProductConfig&&) noexcept = default;

        // Prevent copying
        ProductConfig(const ProductConfig&) = delete;
        ProductConfig& operator=(const ProductConfig&) = delete;

    protected:
        

    public:
        ProductionEnvironments environments;

        std::vector<ClientCredentials> clients;
    };
}

#endif
