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
#include "include/Config/DLLWrapper.h"
#include <filesystem>
#include "include/Config/common.hpp"

namespace pew::eos
{
    using namespace common;

    DLLWrapper::DLLWrapper(const std::string& library_name)
    {
        _library_name = library_name;

        // Get the path to the library relative to the current module
        const auto library_path = get_path_relative_to_current_module(library_name);

        // Load a handle to the library
        library_handle = DLLWrapper::load_library_at_path(library_path);
    }

    DLLWrapper::~DLLWrapper()
    {
        // Make sure to free the library handle
        logging::log_inform("Freeing \"" + _library_name + "\".");
        FreeLibrary(static_cast<HMODULE>(library_handle));
        library_handle = nullptr;
    }

    void* DLLWrapper::load_library_at_path(const std::filesystem::path& library_path)
    {
        void* to_return = nullptr;

#if PLATFORM_WINDOWS
        logging::log_inform(("Loading \"" + _library_name + ",\" at \"" + to_utf8_str(library_path)) + "\".");
        const HMODULE handle = LoadLibrary(library_path.c_str());

        // Uncomment the following line to list all the functions in the library once it is loaded.
        //EnumerateFunctions(handle);
        to_return = static_cast<void*>(handle);
#endif

        return to_return;
    }

}
