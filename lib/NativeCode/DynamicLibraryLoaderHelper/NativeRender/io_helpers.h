#pragma once

namespace playeveryware::eos::io_helpers
{
    //-------------------------------------------------------------------------
    inline static TCHAR* get_path_to_module(HMODULE module)
    {
        DWORD module_path_length = 128;
        TCHAR* module_path = (TCHAR*)malloc(module_path_length * sizeof(TCHAR));

        DWORD buffer_length = 0;
        DWORD GetModuleFileName_last_error = 0;

        do {
            buffer_length = GetModuleFileName(module, module_path, module_path_length);
            GetModuleFileName_last_error = GetLastError();
            SetLastError(NOERROR);

            if (GetModuleFileName_last_error == ERROR_INSUFFICIENT_BUFFER)
            {
                buffer_length = 0;
                module_path_length += 20;
                module_path = (TCHAR*)realloc(module_path, module_path_length * sizeof(TCHAR));
            }
        } while (buffer_length == 0);

        return module_path;
    }

    //-------------------------------------------------------------------------
    inline static std::wstring get_path_to_module_as_string(HMODULE module)
    {
        wchar_t* module_path = get_path_to_module(module);

        std::wstring module_file_path_string(module_path);
        free(module_path);
        return module_file_path_string;
    }

    //-------------------------------------------------------------------------
    inline static std::filesystem::path get_path_relative_to_current_module(const std::filesystem::path& relative_path)
    {
        HMODULE this_module = nullptr;
        if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&get_path_relative_to_current_module, &this_module) || !this_module)
        {
            return {};
        }

        std::wstring module_file_path_string = get_path_to_module_as_string(this_module);

        return std::filesystem::path(module_file_path_string).remove_filename() / relative_path;
    }

}
