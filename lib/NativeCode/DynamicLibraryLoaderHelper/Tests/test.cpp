#include "pch.h"

#define NOMINMAX
#ifdef IServiceProvider
#undef IServiceProvider
#endif
#include <msclr/marshal_cppstd.h>

#include "ConfigurationUtilityBridge.hpp"
#include "CppUnitTest.h"
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

    void CheckEquality(const wchar_t* name, const char* c_str, System::String^ str)
    {
        const auto managed = msclr::interop::marshal_as<std::string>(str);
        const std::string unmanaged = c_str;

        EXPECT_TRUE(managed == unmanaged) << ConstructMessage(name, unmanaged, managed);
    }

    void CheckEquality(const wchar_t* name, const uint64_t& unmanaged_value, const uint64_t& managed_value)
    {
        EXPECT_TRUE(unmanaged_value == managed_value) << ConstructMessage(name, unmanaged_value, managed_value);
    }

    /**
     * \brief Initialize the Common Language Runtime.
     */
    static void InitializeCLR()
    {
        gcnew String(L"CLR initialized");
    }

    TEST(TestNativeAndManagedConfigEquivalency, TestInitializeOptionsEquality) {
        InitializeCLR();
        const auto native_initialize_options = PEW_EOS_Get_InitializeOptions();
        const auto managed_platform_initialize_options = ConfigurationUtilityBridge::get_initialize_options();

        // TODO: Check that neither options objects are nullptr
        CheckEquality(L"ProductName", native_initialize_options.ProductName, managed_platform_initialize_options->ProductName->ToString());
        CheckEquality(L"ProductVersion", native_initialize_options.ProductVersion, managed_platform_initialize_options->ProductVersion->ToString());

        if (nullptr != native_initialize_options.OverrideThreadAffinity)
        {
            CheckEquality(L"OverrideThreadAffinity.NetworkWork", native_initialize_options.OverrideThreadAffinity->NetworkWork, managed_platform_initialize_options->OverrideThreadAffinity.Value.NetworkWork);
            CheckEquality(L"OverrideThreadAffinity.StorageIo", native_initialize_options.OverrideThreadAffinity->StorageIo, managed_platform_initialize_options->OverrideThreadAffinity.Value.StorageIo);
            CheckEquality(L"OverrideThreadAffinity.WebSocketIo", native_initialize_options.OverrideThreadAffinity->WebSocketIo, managed_platform_initialize_options->OverrideThreadAffinity.Value.WebSocketIo);
            CheckEquality(L"OverrideThreadAffinity.P2PIo", native_initialize_options.OverrideThreadAffinity->P2PIo, managed_platform_initialize_options->OverrideThreadAffinity.Value.P2PIo);
            CheckEquality(L"OverrideThreadAffinity.HttpRequestIo", native_initialize_options.OverrideThreadAffinity->HttpRequestIo, managed_platform_initialize_options->OverrideThreadAffinity.Value.HttpRequestIo);
            CheckEquality(L"OverrideThreadAffinity.RTCIo", native_initialize_options.OverrideThreadAffinity->RTCIo, managed_platform_initialize_options->OverrideThreadAffinity.Value.RTCIo);
            CheckEquality(L"OverrideThreadAffinity.EmbeddedOverlayMainThread", native_initialize_options.OverrideThreadAffinity->EmbeddedOverlayMainThread, managed_platform_initialize_options->OverrideThreadAffinity.Value.EmbeddedOverlayMainThread);
            CheckEquality(L"OverrideThreadAffinity.EmbeddedOverlayWorkerThreads", native_initialize_options.OverrideThreadAffinity->EmbeddedOverlayWorkerThreads, managed_platform_initialize_options->OverrideThreadAffinity.Value.EmbeddedOverlayWorkerThreads);
        }
    }
}
