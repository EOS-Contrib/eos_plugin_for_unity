#ifndef CONFIG_CLASS_H
#define CONFIG_CLASS_H

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

#include "headers/Config/Version.hpp"
#include "headers/Config/Serializable.hpp"

namespace pew::eos::config
{
    /**
     * \brief Used to describe information and functionality that is common to
     * all Config classes.
     */
    class ConfigBase : public Serializable
    {
    private:
        /**
         * \brief Function internal to Config that is used to call the required
         * implementation from the more derived class.
         *
         * \param json The json value to use for parsing.
         */
        void from_json_internal(const nlohmann::json& json);

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
         * \brief The current schema version that can be parsed.
         */
        const Version CURRENT_SCHEMA_VERSION = { 1, 0, 0 };

        /**
         * \brief Create a new Config class.
         * \param file_name The fully qualified path to the config file.
         */
        ConfigBase(const std::filesystem::path& file_name);

        /**
         * \brief Default destructor
         */
        ~ConfigBase();

        // Explicitly default move constructor and move assignment operator
        ConfigBase(ConfigBase&&) noexcept = default;
        ConfigBase& operator=(ConfigBase&&) noexcept = default;

        // Delete the copy constructor and copy assignment operator
        ConfigBase(const ConfigBase&) = delete;
        ConfigBase& operator=(const ConfigBase&) = delete;

        virtual std::filesystem::path get_config_path(const char* file_name) = 0;

        /**
         * \brief Reads the configuration values from the file.
         */
        void read();

    public:
        /**
         * \brief Gets the config class and values indicated by the template
         * parameter given.
         * \tparam T The Config-type-derived class that is being retrieved.
         * \return An instance of a class derived from Config that contains all
         * values.
         */
        template <typename T>
        static std::enable_if_t<std::is_base_of_v<ConfigBase, T>, T*> get()
        {
            // Create the config class
            T* config = new T();

            // Read the values from the file
            config->read();

            return config;
        }
    };
}

#endif
