#pragma once

namespace playeveryware::eos::string_helpers
{
    /**
       * \brief Trims the whitespace from the beginning and end of a string.
       *
       * \param str The string to trim.
       *
       * \return A string with no whitespace at the beginning or end.
       */
    inline static std::string trim(const std::string& str)
    {
        const auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
        const auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();

        if (start < end)
        {
            return std::basic_string<char>(start, end);
        }
        else
        {
            return "";
        }
    }

    /**
       * \brief
       * Takes a string, splits it by the indicated delimiter, trims the results of
       * the split, and returns a collection of any non-empty values.
       *
       * \param input The string to split and trim.
       *
       * \param delimiter The character at which to split the string.
       *
       * \return A list of string values.
       */
    inline static std::vector<std::string> split_and_trim(const std::string& input, char delimiter = ',')
    {
        std::vector<std::string> result;
        std::stringstream ss(input);
        std::string item;

        while (std::getline(ss, item, delimiter))
        {
            std::string trimmedItem = trim(item);
            if (!trimmedItem.empty())
            {
                result.push_back(trimmedItem);
            }
        }

        return result;
    }


    //-------------------------------------------------------------------------
    inline static size_t utf8_str_bytes_required_for_wide_str(const wchar_t* wide_str, int wide_str_len = -1)
    {
        int bytes_required = WideCharToMultiByte(CP_UTF8, 0, wide_str, wide_str_len, NULL, 0, NULL, NULL);

        if (bytes_required < 0)
        {
            return 0;
        }

        return bytes_required;
    }

    //-------------------------------------------------------------------------
    // wide_str must be null terminated if wide_str_len is passed
    inline static bool copy_to_utf8_str_from_wide_str(char* RESTRICT utf8_str, size_t utf8_str_len, const wchar_t* RESTRICT wide_str, int wide_str_len = -1)
    {
        if (utf8_str_len > INT_MAX)
        {
            return false;
        }

        WideCharToMultiByte(CP_UTF8, 0, wide_str, wide_str_len, utf8_str, (int)utf8_str_len, NULL, NULL);

        return true;
    }

    //-------------------------------------------------------------------------
    inline static char* create_utf8_str_from_wide_str(const wchar_t* wide_str)
    {
        const int wide_str_len = (int)wcslen(wide_str) + 1;
        int bytes_required = (int)utf8_str_bytes_required_for_wide_str(wide_str, wide_str_len);
        char* to_return = (char*)malloc(bytes_required);

        if (!copy_to_utf8_str_from_wide_str(to_return, bytes_required, wide_str, wide_str_len))
        {
            free(to_return);
            to_return = NULL;
        }

        return to_return;
    }

    //-------------------------------------------------------------------------
    inline static wchar_t* create_wide_str_from_utf8_str(const char* utf8_str)
    {
        int chars_required = MultiByteToWideChar(CP_UTF8, 0, utf8_str, -1, NULL, 0);
        wchar_t* to_return = (wchar_t*)malloc(chars_required * sizeof(wchar_t));
        int utf8_str_len = (int)strlen(utf8_str);

        MultiByteToWideChar(CP_UTF8, 0, utf8_str, utf8_str_len, to_return, chars_required);

        return to_return;
    }

    //-------------------------------------------------------------------------
    // Using the std::wstring_convert method for this currently. It might be the
    // case that in the future this method won't work. If that happens,
    // one could convert this function to use the create_utf8_str_from_wide_str
    // function to emulate it. Doing this might come with a cost, as data will
    // need to be copied multiple times.
    inline static std::string to_utf8_str(const std::wstring& wide_str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::string utf8_str = converter.to_bytes(wide_str);

        return utf8_str;

    }

    //-------------------------------------------------------------------------
    // Using fs::path:string().c_str() seems to cause an issue when paths have
    // kanji in them. Using this function and then std::string:c_str() works around that
    // issue
    inline static std::string to_utf8_str(const std::filesystem::path& path)
    {
        return to_utf8_str(path.native());
    }
}
