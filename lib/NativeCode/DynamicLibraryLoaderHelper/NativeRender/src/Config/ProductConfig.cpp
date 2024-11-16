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

#include <pch.h>
#include "../Config/ProductConfig.h"
#include "include/json.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace pew::eos::config
{
    bool ProductConfig::needs_migration()
    {
        return Config::needs_migration() || !_imported;
    }

    void ProductConfig::migrate()
    {
        // Migrate the values from EpicOnlineServicesConfig.json if needed
        std::cout << "Migrating ProductConfig from EpicOnlineServicesConfig.json" << std::endl;
    }

    ProductConfig::ProductConfig() : Config("eos_product_config.json")
    {
        
    }

    ProductConfig::~ProductConfig() = default;

    void ProductConfig::read()
    {
        if (!std::filesystem::exists(_file_path))
        {
            // TODO: Possibly log error or take other action.
            return;
        }

        std::ifstream file(_file_path);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: \"" << _file_path << "\"\n";
            // TODO: Possibly log error or take other action.
            return;
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string json_content = buffer.str();

        nlohmann::json json = nlohmann::json::parse(json_content);
        auto json_dumped = json.dump(4);
        std::string versionString;
        auto test = json["schemaVersion"].get_to(versionString);

        json["ProductId"].get_to(product_id);
        json["ProductName"].get_to(product_name);
        json["ProductVersion"].get_to(product_version);


        json["Clients"].get_to(clients);

        file.close();

        // If the product config file does not exist
        if(!std::filesystem::exists(_file_path) || needs_migration())
        {
            migrate();
        }
    }
}
