#include <pch.h>

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

#include "eos_library_helpers.h"

#include "logging.h"

namespace playeveryware::eos::eos_library_helpers
{
    
    typedef void (EOS_CALL* EOS_Platform_Release_t)(EOS_HPlatform Handle);
    
    

    
    
    EOS_Platform_Release_t EOS_Platform_Release_ptr;
    

    
    

    //-------------------------------------------------------------------------
     void* load_library_at_path(const std::filesystem::path& library_path)
    {
        void* to_return = nullptr;

#if PLATFORM_WINDOWS
        logging::log_inform(("Loading path at " + string_helpers::to_utf8_str(library_path)).c_str());
        HMODULE handle = LoadLibrary(library_path.c_str());
        to_return = (void*)handle;
#endif

        return to_return;
    }

    //-------------------------------------------------------------------------
     void* load_function_with_name(void* library_handle, const char* function)
    {
        void* to_return = nullptr;
#if PLATFORM_WINDOWS
        HMODULE handle = (HMODULE)library_handle;
        to_return = (void*)GetProcAddress(handle, function);
#endif
        return to_return;
    }

    const char* pick_if_32bit_else(const char* choice_if_32bit, const char* choice_if_else)
    {
#if PLATFORM_32BITS
        return choice_if_32bit;
#else
        return choice_if_else;
#endif
    }

    //-------------------------------------------------------------------------
     void FetchEOSFunctionPointers()
    {
        // The '@' in the function names is apart of how names are mangled on windows. The value after the '@' is the size of the params on the stack
        EOS_Initialize_ptr = load_function_with_name<EOS_Initialize_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_Initialize@4", "EOS_Initialize"));
        EOS_Shutdown_ptr = load_function_with_name<EOS_Shutdown_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_Shutdown@0", "EOS_Shutdown"));
        EOS_Platform_Create_ptr = load_function_with_name<EOS_Platform_Create_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_Platform_Create@4", "EOS_Platform_Create"));
        EOS_Platform_Release_ptr = load_function_with_name<EOS_Platform_Release_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_Platform_Release@4", "EOS_Platform_Release"));
        EOS_Logging_SetLogLevel_ptr = load_function_with_name<EOS_Logging_SetLogLevel_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_Logging_SetLogLevel@8", "EOS_Logging_SetLogLevel"));
        EOS_Logging_SetCallback_ptr = load_function_with_name<EOS_Logging_SetCallback_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("EOS_Logging_SetCallback@4", "EOS_Logging_SetCallback"));

        EOS_IntegratedPlatformOptionsContainer_Add_ptr = load_function_with_name<EOS_IntegratedPlatformOptionsContainer_Add_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_IntegratedPlatformOptionsContainer_Add@8", "EOS_IntegratedPlatformOptionsContainer_Add"));
        EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_ptr = load_function_with_name<EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer@8", "EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer"));
        EOS_IntegratedPlatformOptionsContainer_Release_ptr = load_function_with_name<EOS_IntegratedPlatformOptionsContainer_Release_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_IntegratedPlatformOptionsContainer_Release@4", "EOS_IntegratedPlatformOptionsContainer_Release"));
    }

    //-------------------------------------------------------------------------
     bool QueryRegKey(const HKEY InKey, const TCHAR* InSubKey, const TCHAR* InValueName, std::wstring& OutData)
    {
        bool bSuccess = false;
#if PLATFORM_WINDOWS
        // Redirect key depending on system
        for (uint32_t RegistryIndex = 0; RegistryIndex < 2 && !bSuccess; ++RegistryIndex)
        {
            HKEY Key = 0;
            const uint32_t RegFlags = (RegistryIndex == 0) ? KEY_WOW64_32KEY : KEY_WOW64_64KEY;
            if (RegOpenKeyEx(InKey, InSubKey, 0, KEY_READ | RegFlags, &Key) == ERROR_SUCCESS)
            {
                ::DWORD Size = 0;
                // First, we'll call RegQueryValueEx to find out how large of a buffer we need
                if ((RegQueryValueEx(Key, InValueName, NULL, NULL, NULL, &Size) == ERROR_SUCCESS) && Size)
                {
                    // Allocate a buffer to hold the value and call the function again to get the data
                    char* Buffer = new char[Size];
                    if (RegQueryValueEx(Key, InValueName, NULL, NULL, (LPBYTE)Buffer, &Size) == ERROR_SUCCESS)
                    {
                        const uint32_t Length = (Size / sizeof(TCHAR)) - 1;
                        OutData = (TCHAR*)Buffer;
                        bSuccess = true;
                    }
                    delete[] Buffer;
                }
                RegCloseKey(Key);
            }
        }
#endif
        return bSuccess;
    }

    //-------------------------------------------------------------------------
     void unload_library(void* library_handle)
    {
        FreeLibrary((HMODULE)library_handle);
    }

}
