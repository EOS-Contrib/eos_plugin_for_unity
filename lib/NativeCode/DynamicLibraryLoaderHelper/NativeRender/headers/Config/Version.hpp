#ifndef VERSION_H
#define VERSION_H

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

#include <sstream>
#include <string>

namespace pew::eos::config
{
    struct Version
    {
        int major = 0;
        int minor = 0;
        int part = 0;

        bool operator<(const Version& other) const
        {
            if (major != other.major) return major < other.major;
            if (minor != other.minor) return minor < other.minor;
            return part < other.part;
        }

        bool operator>(const Version& other) const
        {
            return other < *this;
        }

        bool operator==(const Version& other) const
        {
            return major == other.major && minor == other.minor && part == other.part;
        }

        bool operator<=(const Version& other) const
        {
            return !(*this > other);
        }

        bool operator>=(const Version& other) const
        {
            return !(*this < other);
        }

        static bool try_parse(const std::string& str, Version& version)
        {
            // Temporary variables to hold parsed values
            int temp_major = 0;
            int temp_minor = 0;
            int temp_part = 0;

            // Stream to parse the input
            std::istringstream stream(str);
            std::string segment;

            // Read major version
            if (!std::getline(stream, segment, '.') || !try_parse_int(segment, temp_major))
                return false;

            // Check for minor version
            if (std::getline(stream, segment, '.'))
            {
                if (!try_parse_int(segment, temp_minor))
                    return false;

                // Check for part version
                if (std::getline(stream, segment, '.'))
                {
                    if (!try_parse_int(segment, temp_part))
                        return false;
                }
            }

            // Update the output if parsing was successful
            version.major = temp_major;
            version.minor = temp_minor;
            version.part = temp_part;
            return true;
        }
    private:
        friend struct ConfigBase;

        static bool try_parse_int(const std::string& str, int& value)
        {
            try
            {
                size_t pos = 0;
                value = std::stoi(str, &pos);
                return pos == str.size(); // Ensure the entire string was parsed
            }
            catch (...)
            {
                return false;
            }
        }
    };
}

#endif
