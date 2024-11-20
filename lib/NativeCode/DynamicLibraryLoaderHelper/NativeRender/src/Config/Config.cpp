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
#include "../Config/Config.h"
#include <filesystem>
#include "include/json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Version.h"
#include <cstring>

#include "io_helpers.h"

namespace pew::eos::config
{
    using namespace std::filesystem;

    /**
     * \brief This represents what the most current version of the schema is.
     */
    //static const Version CURRENT_SCHEMA_VERSION = Version{ 1, 0, 0 };

    Config::Config(const char* file_name)
    {
        const auto config_directory = absolute(io_helpers::get_path_relative_to_current_module(path("../../../../../../Assets/StreamingAssets/EOS/") / file_name));
        const auto file_path = absolute(io_helpers::get_path_relative_to_current_module(path(
#ifdef _DEBUG
            "../../../../../../Assets/StreamingAssets/EOS/"
#endif
#ifdef NDEBUG
            "../../StreamingAssets/EOS/"
#endif
        ) / file_name));
        const auto file_path_string = file_path.string();
        _file_path = file_path_string;
    }

    void Config::read() 
    {
        if(!exists(_file_path))
        {
            std::cerr << "Config file \"" << _file_path << "\" does not exist." << std::endl;
            return;
        }

        std::ifstream file(_file_path);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: \"" << _file_path << "\"" << std::endl;
            return;
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string json_content = buffer.str();

        nlohmann::json json = nlohmann::json::parse(json_content);

        from_json_internal(json);
    }

    void Config::write() 
    {

    }

    bool Config::needs_migration()
    {
        // The config needs migration if the pImpl is either not set or is not current.
        //return (CURRENT_SCHEMA_VERSION > _schema_version);
        return true;
    }

    void Config::from_json_internal(const nlohmann::json& json)
    {
        Version::try_parse(json["schemaVersion"], _schema_version);
        from_json(json);
    }

    Config::~Config() = default;
}
