#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H
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
#include <string>
#include <vector>
#include <filesystem>

namespace pew::eos::string_helpers
{
    /**
       * \brief Trims the whitespace from the beginning and end of a string.
       *
       * \param str The string to trim.
       *
       * \return A string with no whitespace at the beginning or end.
       */
    std::string trim(const std::string& str);

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
    std::vector<std::string> split_and_trim(const std::string& input, char delimiter = ',');

    /**
     * @brief Creates an ISO 8601 formatted timestamp string with millisecond precision.
     *
     * This function generates a timestamp in the format "YYYY-MM-DDTHH:MM:SS.sss" and stores it
     * in the provided buffer. If the buffer is not large enough to hold the timestamp,
     * the function returns `false`.
     *
     * @param[out] final_timestamp A buffer to store the resulting timestamp string.
     * @param[in] final_timestamp_len The length of the buffer provided.
     * @return `true` if the timestamp was successfully created, `false` if the buffer was too small.
     */
    bool create_timestamp_str(char* final_timestamp, size_t final_timestamp_len);

    /**
     * @brief Calculates the number of bytes required to store a UTF-8 encoded version of a wide string.
     *
     * This function uses `WideCharToMultiByte` to determine the number of bytes necessary to represent
     * a given wide string in UTF-8 encoding.
     *
     * @param[in] wide_str The wide string to evaluate.
     * @param[in] wide_str_len The length of the wide string.
     * @return The number of bytes required for UTF-8 encoding, or `0` if an error occurs.
     */
    size_t utf8_str_bytes_required_for_wide_str(const wchar_t* wide_str, int wide_str_len);

    /**
     * @brief Converts a wide string to a UTF-8 encoded string and copies it to a provided buffer.
     *
     * This function converts a wide character string to UTF-8 and stores the result in `utf8_str`.
     * The buffer size should not exceed `INT_MAX` to avoid overflow.
     *
     * @param[out] utf8_str A buffer to store the UTF-8 encoded string.
     * @param[in] utf8_str_len The length of the UTF-8 buffer.
     * @param[in] wide_str The wide character string to convert.
     * @param[in] wide_str_len The length of the wide string. If `wide_str` is null-terminated, set to -1.
     * @return `true` if the conversion was successful, `false` otherwise.
     *
     * @note `wide_str` must be null-terminated if `wide_str_len` is set to -1.
     */
    bool copy_to_utf8_str_from_wide_str(char* RESTRICT utf8_str, size_t utf8_str_len, const wchar_t* RESTRICT wide_str, int wide_str_len);

    /**
     * @brief Creates a UTF-8 encoded string from a wide character string.
     *
     * This function allocates a UTF-8 encoded version of the provided wide character string.
     * The caller is responsible for freeing the returned string.
     *
     * @param[in] wide_str The wide character string to convert.
     * @return A dynamically allocated UTF-8 encoded string, or `nullptr` if conversion fails.
     *
     * @note The caller must free the returned string to avoid memory leaks.
     */
    char* create_utf8_str_from_wide_str(const wchar_t* wide_str);

    /**
     * @brief Creates a wide character string from a UTF-8 encoded string.
     *
     * Converts a UTF-8 string to a dynamically allocated wide character string.
     * The caller is responsible for freeing the returned string.
     *
     * @param[in] utf8_str The UTF-8 encoded string to convert.
     * @return A dynamically allocated wide string, or `nullptr` if conversion fails.
     *
     * @note The caller must free the returned string to avoid memory leaks.
     */
    wchar_t* create_wide_str_from_utf8_str(const char* utf8_str);

    /**
     * @brief Converts a wide string to a UTF-8 encoded `std::string`.
     *
     * Uses `std::wstring_convert` for the conversion. If `std::wstring_convert`
     * becomes unsupported in the future, this function could be rewritten using
     * `create_utf8_str_from_wide_str`.
     *
     * Using the std::wstring_convert method for this currently. It might be the
     * case that in the future this method won't work. If that happens, one
     * could convert this function to use the create_utf8_str_from_wide_str
     * function to emulate it. Doing this might come with a cost, as data will
     * need to be copied multiple times.
     *
     * @param[in] wide_str The wide string to convert.
     * @return A UTF-8 encoded `std::string` representation of the wide string.
     */
    std::string to_utf8_str(const std::wstring& wide_str);

    /**
     * @brief Converts a filesystem path to a UTF-8 encoded `std::string`.
     *
     * This function provides a workaround for converting filesystem paths that contain kanji characters.
     * Using this method ensures compatibility across different character sets.
     *
     * @param[in] path The filesystem path to convert.
     * @return A UTF-8 encoded `std::string` representation of the path.
     */
    std::string to_utf8_str(const std::filesystem::path& path);
}
#endif
