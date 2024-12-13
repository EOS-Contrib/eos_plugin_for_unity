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

#define NOMINMAX
#ifdef IServiceProvider
#undef IServiceProvider
#endif
#include <msclr/marshal_cppstd.h>

#include "ConfigurationUtilityBridge.hpp"
#include "CppUnitTest.h"
#include "eos_helpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace pew::eos::tests
{
    using namespace Epic::OnlineServices;

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

    void CheckEquality(const wchar_t* name, const char* c_str, String^ str)
    {
        const auto managed = msclr::interop::marshal_as<std::string>(str);
        const std::string unmanaged = c_str;

        Assert::IsTrue(managed == unmanaged, ConstructMessage(name, unmanaged, managed).c_str());
    }


	TEST_CLASS(Tests)
	{
        /**
         * \brief Initialize the Common Language Runtime.
         */
        static void InitializeCLR()
        {
            gcnew String(L"CLR initialized");
        }

	public:
		// Call CLR initialization before the tests run
        Tests()
        {
            InitializeCLR();
        }

		TEST_METHOD(TestMethod1)
		{
            const auto native_initialize_options = PEW_EOS_Get_InitializeOptions();
            const auto managed_platform_initialize_options = ConfigurationUtilityBridge::get_initialize_options();

            // TODO: Check that neither options objects are nullptr

            CheckEquality(L"ProductName", native_initialize_options->ProductName, managed_platform_initialize_options->ProductName->ToString());
            //CheckEquality("ProductVersion", native_initialize_options->ProductVersion, managed_platform_initialize_options->ProductVersion->ToString());

            //if (nullptr != native_initialize_options->OverrideThreadAffinity)
            //{
                //CheckEquality("OverrideThreadAffinity.NetworkWork", native_initialize_options->OverrideThreadAffinity->NetworkWork, managed_platform_initialize_options->OverrideThreadAffinity.Value.NetworkWork);
                //CheckEquality("OverrideThreadAffinity.StorageIo", native_initialize_options->OverrideThreadAffinity->StorageIo, managed_platform_initialize_options->OverrideThreadAffinity.Value.StorageIo);
                //CheckEquality("OverrideThreadAffinity.WebSocketIo", native_initialize_options->OverrideThreadAffinity->WebSocketIo, managed_platform_initialize_options->OverrideThreadAffinity.Value.WebSocketIo);
                //CheckEquality("OverrideThreadAffinity.P2PIo", native_initialize_options->OverrideThreadAffinity->P2PIo, managed_platform_initialize_options->OverrideThreadAffinity.Value.P2PIo);
                //CheckEquality("OverrideThreadAffinity.HttpRequestIo", native_initialize_options->OverrideThreadAffinity->HttpRequestIo, managed_platform_initialize_options->OverrideThreadAffinity.Value.HttpRequestIo);
                //CheckEquality("OverrideThreadAffinity.RTCIo", native_initialize_options->OverrideThreadAffinity->RTCIo, managed_platform_initialize_options->OverrideThreadAffinity.Value.RTCIo);
                //CheckEquality("OverrideThreadAffinity.EmbeddedOverlayMainThread", native_initialize_options->OverrideThreadAffinity->EmbeddedOverlayMainThread, managed_platform_initialize_options->OverrideThreadAffinity.Value.EmbeddedOverlayMainThread);
                //CheckEquality("OverrideThreadAffinity.EmbeddedOverlayWorkerThreads", native_initialize_options->OverrideThreadAffinity->EmbeddedOverlayWorkerThreads, managed_platform_initialize_options->OverrideThreadAffinity.Value.EmbeddedOverlayWorkerThreads);
            //}
		}
	};



    //bool Equals(const int32_t& bool_value, const bool& managed_bool)
    //{
    //    bool translated = bool_value == 1 ? true : false;

    //    return (translated == managed_bool);
    //}

    //bool Equals(const uint64_t& long_value, const uint64_t& long_value_managed)
    //{
    //    return (long_value == long_value_managed);
    //}

    //void CheckEquality(const char* name, const int32_t& bool_value, const bool& managed_bool)
    //{
    //    if (!Equals(bool_value, managed_bool))
    //    {
    //        std::cerr << name << " is not equal (Native: " << bool_value << ", Managed: " << managed_bool << ")." << std::endl;
    //    }
    //}

    //void CheckEquality(const char* name, const uint64_t& long_value, Epic::OnlineServices::Platform::PlatformFlags^ flags)
    //{
    //    uint64_t managed_long_value = static_cast<uint64_t>(static_cast<Epic::OnlineServices::Platform::PlatformFlags>(*flags));

    //    if (!Equals(long_value, managed_long_value))
    //    {
    //        std::cerr << name << " is not equal (Native: " << long_value << ", Managed: " << managed_long_value << ")." << std::endl;
    //    }
    //}

    //void CheckEquality(const char* name, const double* double_value, System::Nullable<double>^ managed_double_value)
    //{
    //    double managed_dbl = 0.0;
    //    if (managed_double_value->HasValue)
    //    {
    //        managed_dbl = managed_double_value->Value;
    //    }
    //    if (*double_value != managed_dbl)
    //    {
    //        std::cerr << name << " is not equal (Native: " << *double_value << ", Managed: " << managed_dbl << ")." << std::endl;
    //    }
    //}

    //void CheckEquality(const char* name, const uint32_t& unmanaged_uint, const uint32_t& managed_uint)
    //{
    //    if (unmanaged_uint != managed_uint)
    //    {
    //        std::cerr << name << " is not equal (Native: " << unmanaged_uint << ", Managed: " << managed_uint << ")." << std::endl;
    //    }
    //}

    //void CheckEquality(const char* name, const uint64_t& long_value, uint64_t managed_long_value)
    //{
    //    const auto message = ConstructMessage(name, long_value, managed_long_value).c_str();
    //    Assert::IsTrue(long_value == managed_long_value, message);
    //}

}
