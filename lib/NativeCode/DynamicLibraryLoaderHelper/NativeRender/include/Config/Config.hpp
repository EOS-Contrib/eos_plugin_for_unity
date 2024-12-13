#ifndef CONFIG_CLASS_HPP
#define CONFIG_CLASS_HPP

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

#include <filesystem>
#include <fstream>
#include <sstream>
#include <array>
#include <string>

#include "json.h"
#include "include/logging.h"
#include "include/io_helpers.h"
#include "include/Config/Serializable.hpp"
#include "include/Config/Version.hpp"

namespace pew::eos::config
{
    /**
     * \brief Used to describe information and functionality that is common to
     * all Config classes.
     */
    struct Config : public Serializable
    {
        // Delete the copy constructor and copy assignment operator
        Config(const Config&) = delete;
        Config& operator=(const Config&) = delete;

        /**
         * \brief Gets the config class and values indicated by the template
         * parameter given.
         * \tparam T The Config-type-derived class that is being retrieved.
         * \return An instance of a class derived from Config that contains all
         * values.
         */
        template <typename T>
        static std::enable_if_t<std::is_base_of_v<Config, T>, std::unique_ptr<T>> get()
        {
            // Create the config class. The reason that "new" is used inside
            // unique_ptr instead of using make_unique is because the
            // constructor for Config-derived classes is protected and/or
            // private - so it cannot be called by the internals of make_unique.
            // This solution maintains all of the benefits of unique_ptr save
            // one, which is that exceptions that might originate from within
            // the constructor will not be as elegantly caught as they would if
            // make_unique was utilized. For our purposes, this is acceptable.
            auto config = std::unique_ptr<T>(new T());

            // Read the values from the file
            config->read();

            return config;
        }

    private:

        // Depending on the configuration (debug or release) these are the possible relative paths to the config directory
        static constexpr std::array<std::string_view, 4> s_possible_config_directories = {
            // This is the relative path for the config files when in release
            // mode.
            "../../StreamingAssets/EOS/",
            // This is the relative path for the config files when in debug mode
            // (specifically when the library is loaded in the Build\Debug
            // directory).
            "../../../../../../Assets/StreamingAssets/EOS/",
            // These particular entries are used to maintain backwards 
            // compatibility with older versions of the plugin that supported a
            // different directory structure for the deployed config files.
            "../../",
            "./Data/",
        };

        static inline std::filesystem::path s_config_directory;

        static std::filesystem::path get_config_directory()
        {
            // If the config directory has not been determined, then determine
            // it.
            if (s_config_directory.empty())
            {
                for(const auto& directory : s_possible_config_directories)
                {
                    const auto absolute_dir_path = absolute(io_helpers::get_path_relative_to_current_module(directory));
                    if (!exists(absolute_dir_path))
                    {
                        continue;
                    }

                    s_config_directory = absolute_dir_path;
                    break;
                }
            }

            return s_config_directory;
        }

        /**
         * \brief Function internal to Config that is used to call the required
         * implementation from the more derived class.
         *
         * \param json The json value to use for parsing.
         */
        void from_json_internal(const json_object_s& json)
        {
            // Call the deriving class' from_json function.
            from_json(json);
        }

    protected:
        /**
         * \brief The fully qualified path to the file that backs the
         * configuration.
         */
        std::filesystem::path _file_path;
        
        /**
         * \brief The schema version for the file. 
         */
        Version _schema_version;

        /**
         * \brief Create a new Config class.
         * \param file_name The fully qualified path to the config file.
         */
        Config(const std::filesystem::path& file_name)
        {
            _file_path = get_config_directory() / file_name;
        }

        /**
         * \brief Default destructor
         */
        ~Config() = default;

        // Explicitly default move constructor and move assignment operator
        Config(Config&&) noexcept = default;
        Config& operator=(Config&&) noexcept = default;

        /**
         * \brief Reads the configuration values from the file.
         */
        void read()
        {
            if (!exists(_file_path))
            {
                logging::log_error("Config file \"" + _file_path.string() + "\" does not exist.");
                return;
            }

            const std::ifstream file(_file_path);
            if (!file.is_open())
            {
                logging::log_error("Failed to open existing file: \"" + _file_path.string() + "\"");
                return;
            }

            std::ostringstream buffer;
            buffer << file.rdbuf();
            const std::string json_content = buffer.str();

            json_value_s* json_value = json_parse(json_content.c_str(), json_content.length());
            const auto json_object = static_cast<json_object_s*>(json_value->payload);

            from_json(*json_object);

            free(json_value);
        }
    };
}

#endif
