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

#include <pch.h>
#include "Version.h"
#include "include/json.hpp"
#include <string>
//
//namespace pew::eos::config
//{
//    bool Version::operator<(const Version& other) const
//    {
//        if (major != other.major) return major < other.major;
//        if (minor != other.minor) return minor < other.minor;
//        return part < other.part;
//    }
//
//    bool Version::operator>(const Version& other) const
//    {
//        return other < *this;
//    }
//
//    bool Version::operator==(const Version& other) const
//    {
//        return major == other.major && minor == other.minor && part == other.part;
//    }
//
//    bool Version::operator<=(const Version& other) const
//    {
//        return !(*this > other);
//    }
//
//    bool Version::operator>=(const Version& other) const
//    {
//        return !(*this < other);
//    }
//}
//
//namespace nlohmann
//{
//    void from_json(const nlohmann::json& j, pew::eos::config::Version& v) {
//        if (j.is_string()) {
//            std::string version_str = j.get<std::string>(); // Retrieve string from JSON
//            size_t pos = 0;
//
//            // Parse major version
//            pos = version_str.find('.');
//            v.major = static_cast<uint32_t>(std::stoul(version_str.substr(0, pos)));
//            if (pos == std::string::npos) {
//                v.minor = 0;
//                v.part = 0;
//                return;
//            }
//
//            // Parse minor version
//            size_t start = pos + 1;
//            pos = version_str.find('.', start);
//            v.minor = static_cast<uint32_t>(std::stoul(version_str.substr(start, pos - start)));
//            if (pos == std::string::npos) {
//                v.part = 0;
//                return;
//            }
//
//            // Parse patch version
//            start = pos + 1;
//            v.part = static_cast<uint32_t>(std::stoul(version_str.substr(start)));
//        }
//        else {
//            throw std::invalid_argument("Invalid JSON format for Version");
//        }
//    }
//
//
//    void to_json(nlohmann::json& j, const pew::eos::config::Version& v) {
//        if (v.minor == 0 && v.part == 0) {
//            j = std::to_string(v.major); // Assign string directly to JSON
//        }
//        else if (v.part == 0) {
//            j = std::to_string(v.major) + "." + std::to_string(v.minor); // Assign string directly
//        }
//        else {
//            j = std::to_string(v.major) + "." + std::to_string(v.minor) + "." + std::to_string(v.part);
//        }
//    }
//
//}
