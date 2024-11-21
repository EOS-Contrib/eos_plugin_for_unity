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

#include "pch.h"
#include <string>

namespace pew::eos::config
{
    struct CONFIG_API Version
    {
    public:
        int major = 0;
        int minor = 0;
        int part = 0;

        bool operator<(const Version& other) const;
        bool operator>(const Version& other) const;
        bool operator==(const Version& other) const;
        bool operator<=(const Version& other) const;
        bool operator>=(const Version& other) const;

        static bool try_parse(const std::string& str, Version& version);

    private:
        friend class ConfigBase;
        static bool try_parse_int(const std::string& str, int& value);
    };
}

#endif
