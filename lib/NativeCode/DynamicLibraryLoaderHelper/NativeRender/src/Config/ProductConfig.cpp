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
#include "include/Config/ProductConfig.h"
#include "io_helpers.h"

namespace pew::eos::config
{
    void ProductConfig::from_json(const json_value_s& json)
    {
        // TODO: Implement
    }

    // TODO: The implementation between this and what is in PlatformConfig.h is
    //       identical. Figure out a way to reduce the duplication.
    std::filesystem::path ProductConfig::get_config_path(const char* file_name)
    {
        return std::filesystem::absolute(io_helpers::get_path_relative_to_current_module(std::filesystem::path(
#ifdef _DEBUG
            "../../../../../../"
#endif
#ifdef NDEBUG
            "../../../"
#endif
        ) / "Assets/StreamingAssets/EOS/" / file_name));
    }
}
