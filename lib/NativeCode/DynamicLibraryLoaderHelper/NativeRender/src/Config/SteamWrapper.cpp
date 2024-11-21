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
#include "SteamWrapper.h"

#include "SteamConfig.h"

namespace pew::eos
{
    SteamWrapper::SteamWrapper(): DLLWrapper(get_library_path())
    {
        library_handle = SteamWrapper::load_library_at_path(get_library_path());
    }

    void SteamWrapper::init()
    {
        if (library_handle == nullptr)
        {
            return;
        }

        const auto result_of_init_call = call_library_function<SteamAPI_Init_t>();
        std::cout << "Result of SteamAPI_Init call:";
        if (result_of_init_call)
        {
            std::cout << "SUCCESS";
        }
        else
        {
            std::cout << "FAILURE";
        }
        std::cout << std::endl;
    }

    std::string SteamWrapper::get_library_path()
    {
        const auto steam_config = config::Config::get<config::SteamConfig>();

        std::filesystem::path library_path;
        if(steam_config->try_get_library_path(library_path))
        {
            return library_path.string();
        }

        return "";
    }

    void* SteamWrapper::load_library_at_path(const std::filesystem::path& library_path)
    {
        // Note that for this to work it has to be in the same directory as the current module.
        HMODULE handle = LoadLibrary(library_path.c_str());
        EnumerateFunctions(handle);

        typedef bool(__cdecl* SteamAPI_Init_t)();
        const auto function_ptr = reinterpret_cast<SteamAPI_Init_t>((void*)GetProcAddress(handle, "SteamAPI_Init"));
        function_ptr();
        return static_cast<void*>(handle);
    }
}
