#ifndef TESTS_COMMON_HPP
#define TESTS_COMMON_HPP
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

#include "pch.h"

#include "eos_helpers.h"

namespace pew::eos::tests
{
  // Helper trait to detect if a type supports the << operator
  template<typename T, typename Stream, typename = void>
  struct supports_streaming : std::false_type {};

  // Specialization if the << operator is valid
  template<typename T, typename Stream>
  struct supports_streaming<T, Stream, std::void_t<decltype(std::declval<Stream&>() << std::declval<T>())>> : std::true_type {};

  template<typename T, typename Stream>
  inline constexpr bool supports_streaming_v = supports_streaming<T, Stream>::value;

  /**
   * \brief Utility to convert unsupported types for wide streams.
   */
  struct WideStreamHelper {
    static std::wstring Convert(const std::string& str) {
      return std::wstring(str.begin(), str.end()); // Naive conversion for ASCII strings
    }

    static const std::wstring& Convert(const std::wstring& str) {
      return str;
    }

    template <typename T>
    static const T& Convert(const T& value) {
      return value; // Pass through for types that are natively streamable
    }
  };

  /**
   * \brief Construct a message to display if an equality assert fails.
   * \tparam T1 The type of the unmanaged value.
   * \tparam T2 The type of the managed value.
   * \param name The name for the value.
   * \param unmanaged_value The unmanaged value.
   * \param managed_value The managed value.
   * \return A string message describing the inequality between two values.
   */
  template<typename T1, typename T2,
    std::enable_if_t<
    (supports_streaming_v<decltype(WideStreamHelper::Convert(std::declval<T1>())), std::wostream>&&
      supports_streaming_v<decltype(WideStreamHelper::Convert(std::declval<T2>())), std::wostream>),
    int> = 0>
  static std::wstring ConstructMessage(const wchar_t* name, const T1& unmanaged_value, const T2& managed_value)
  {
    std::wstringbuf buffer;
    std::wostream os(&buffer);

    os << name << L" is not equal (Native: "
      << WideStreamHelper::Convert(unmanaged_value)
      << L", Managed: "
      << WideStreamHelper::Convert(managed_value)
      << L").";

    return buffer.str();
  }

  /**
   * @brief Determine whether the value with the indicated name is equal from managed and unmanaged.
   * @param name Checks the equality of two values.
   * @param c_str The unmanaged value.
   * @param str The managed value.
   */
  inline void CheckEquality(const wchar_t* name, const char* c_str, System::String^ str)
  {
    const auto managed = msclr::interop::marshal_as<std::string>(str);
    const std::string unmanaged = c_str;

    EXPECT_TRUE(managed == unmanaged) << ConstructMessage(name, unmanaged, managed);
  }

  /**
   * This function will check equality between a const char* value and a Utf8String, but it will consider a nullptr Utf8String to be equal to "" for the sake of
   * comparison.
   *
   * @param name The name of the property being examined for equality.
   * @param c_str The const char* version of the string.
   * @param str The Utf8String object.
   */
  inline void CheckEquality(const wchar_t* name, const char* c_str, Epic::OnlineServices::Utf8String^ str)
  {
	  System::String^ string_value = "";
    if (str != nullptr)
    {
      string_value = str->ToString();
    }

    CheckEquality(name, c_str, string_value);
  }

  /**
   * @brief Determine whether the value with the indicated name is equal from managed and unmanaged.
   * @param name Checks the equality of two values.
   * @param unmanaged_value The unmanaged value.
   * @param managed_value The managed value.
   */
  inline void CheckEquality(const wchar_t* name, const uint64_t& unmanaged_value, const uint64_t& managed_value)
  {
    EXPECT_TRUE(unmanaged_value == managed_value) << ConstructMessage(name, unmanaged_value, managed_value);
  }

  /**
   * @brief Determine whether the value with the indicated name is equal from managed and unmanaged.
   * @param name Checks the equality of two values.
   * @param unmanaged_value The unmanaged value.
   * @param managed_value The managed value.
   */
  inline void CheckEquality(const wchar_t* name, const uint32_t& unmanaged_value, const unsigned int managed_value)
  {
    EXPECT_TRUE(unmanaged_value == managed_value) << ConstructMessage(name, unmanaged_value, managed_value);
  }

  /**
   * @brief Determine whether the value with the indicated name is equal from managed and unmanaged.
   * @param name Checks the equality of two values.
   * @param unmanaged_value The unmanaged value.
   * @param managed_value The managed value.
   */
  inline void CheckEquality(const wchar_t* name, const double* unmanaged_value, System::Nullable<double>& managed_value)
  {
    // Helper to extract value or default for System::Nullable<double>
    auto get_managed_value = [](System::Nullable<double>& value, double default_value = 0.0) -> double {
      return value.HasValue ? value.Value : default_value;
      };

    // Helper to extract value or default for unmanaged pointer
    auto get_unmanaged_value = [](const double* value, double default_value = 0.0) -> double {
      return value ? *value : default_value;
      };

    // If both are "null," they are considered equal
    if (!unmanaged_value && !managed_value.HasValue)
    {
      return; // Equal; no need to proceed
    }

    // If only one is "null," they cannot be equal
    if (!unmanaged_value || !managed_value.HasValue)
    {
      EXPECT_TRUE(false) << "Mismatch: "
        << (unmanaged_value ? *unmanaged_value : NAN)
        << " vs. "
        << (managed_value.HasValue ? managed_value.Value : NAN)
        << " (unmanaged vs. managed).";
      return;
    }

    // Extract the actual values
    double unmanaged_dbl_value = get_unmanaged_value(unmanaged_value);
    double managed_dbl_value = get_managed_value(managed_value);

    // Construct message for failure reporting
    const auto message = ConstructMessage(name, unmanaged_dbl_value, managed_dbl_value);

    // Assert equality
    EXPECT_TRUE(unmanaged_dbl_value == managed_dbl_value) << message;
  }

  /**
   * @brief Determine whether the value with the indicated name is equal from managed and unmanaged.
   * @param name Checks the equality of two values.
   * @param unmanaged_value The unmanaged value.
   * @param managed_value The managed value.
   */
  inline void CheckEquality(const wchar_t* name, const EOS_Bool& unmanaged_value, const bool managed_value)
  {
    bool unmanaged_bool_value = (unmanaged_value == 1);

    EXPECT_TRUE(unmanaged_bool_value == managed_value) << ConstructMessage(name, unmanaged_value, managed_value);
  }

  /**
   * @brief Determine whether the value with the indicated name is equal from managed and unmanaged.
   * @param name Checks the equality of two values.
   * @param unmanaged_value The unmanaged value.
   * @param managed_value The managed value.
   */
  inline void CheckEquality(const wchar_t* name, const int32_t& unmanaged_value, const int32_t& managed_value)
  {
    EXPECT_TRUE(unmanaged_value == managed_value) << ConstructMessage(name, unmanaged_value, managed_value);
  }
}

#endif