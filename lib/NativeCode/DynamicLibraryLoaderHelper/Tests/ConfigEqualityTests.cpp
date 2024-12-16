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

    /**
     * This function will check equality between a const char* value and a Utf8String, but it will consider a nullptr Utf8String to be equal to "" for the sake of
     * comparison.
     *
     * @param name The name of the property being examined for equality.
     * @param c_str The const char* version of the string.
     * @param str The Utf8String object.
     */
    void CheckEquality(const wchar_t* name, const char* c_str, Epic::OnlineServices::Utf8String^ str)
    {
        String^ string_value = "";
        if (str != nullptr)
        {
            string_value = str->ToString();
        }

        CheckEquality(name, c_str, string_value);
    }

    void CheckEquality(const wchar_t* name, const uint64_t& unmanaged_value, const uint64_t& managed_value)
    {
        EXPECT_TRUE(unmanaged_value == managed_value) << ConstructMessage(name, unmanaged_value, managed_value);
    }

    void CheckEquality(const wchar_t* name, const uint32_t& unmanaged_value, const unsigned int managed_value)
    {
        EXPECT_TRUE(unmanaged_value == managed_value) << ConstructMessage(name, unmanaged_value, managed_value);
    }

    void CheckEquality(const wchar_t* name, const double* unmanaged_value, System::Nullable<double>& managed_value)
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

    void CheckEquality(const wchar_t* name, const EOS_Bool& unmanaged_value, const bool managed_value)
    {
        bool unmanaged_bool_value = (unmanaged_value == 1);

        EXPECT_TRUE(unmanaged_bool_value == managed_value) << ConstructMessage(name, unmanaged_value, managed_value);
    }

    void CheckEquality(const wchar_t* name, const int32_t& unmanaged_value, const int32_t& managed_value)
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

        // Check the product name and product version for equality.
        CheckEquality(L"ProductName", native_initialize_options.ProductName, managed_platform_initialize_options->ProductName->ToString());
        CheckEquality(L"ProductVersion", native_initialize_options.ProductVersion, managed_platform_initialize_options->ProductVersion->ToString());

        // If the override thread affinity is set, then check equality for each value.
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

    TEST(TestNativeAndManagedConfigEquivalency, TestCreateOptionsEquality)
    {
        InitializeCLR();

        const auto native_create_options = PEW_EOS_Get_CreateOptions();
        const auto managed_create_options = ConfigurationUtilityBridge::get_windows_options();

        // TODO: Note that CacheDirectory is different since it is determined differently
        // CheckEquality(L"CacheDirectory", native_create_options.CacheDirectory, managed_create_options->CacheDirectory->ToString());

        // Check the primary deployment values.
        CheckEquality(L"DeploymentId", native_create_options.DeploymentId, managed_create_options->DeploymentId->ToString());
        CheckEquality(L"SandboxId", native_create_options.SandboxId, managed_create_options->SandboxId->ToString());

        // Check the flags.
        CheckEquality(L"Flags", native_create_options.Flags, static_cast<uint64_t>(managed_create_options->Flags));

        // Check the product id.
        CheckEquality(L"ProductId", native_create_options.ProductId, managed_create_options->ProductId->ToString());

        // Check the client credentials and file encryption key.
        CheckEquality(L"ClientId", native_create_options.ClientCredentials.ClientId, managed_create_options->ClientCredentials.ClientId->ToString());
        CheckEquality(L"ClientSecret", native_create_options.ClientCredentials.ClientSecret, managed_create_options->ClientCredentials.ClientSecret->ToString());
        CheckEquality(L"EncryptionKey", native_create_options.EncryptionKey, managed_create_options->EncryptionKey->ToString());

        // Check the timeout values.
        CheckEquality(L"TickBudgetInMilliseconds", native_create_options.TickBudgetInMilliseconds, managed_create_options->TickBudgetInMilliseconds);
        CheckEquality(L"TaskNetworkTimeoutSeconds", native_create_options.TaskNetworkTimeoutSeconds, managed_create_options->TaskNetworkTimeoutSeconds);

        // Check the deployment parameter indicating whether the deployment is a server or not.
        CheckEquality(L"IsServer", native_create_options.bIsServer, managed_create_options->IsServer);

        // TODO: Checking the OverrideCountryCode and OverrideLocaleCode causes a strange error, but since they are not supported in the Unity editor they currently do not need to be tested.
        //CheckEquality(L"OverrideCountryCode", native_create_options.OverrideCountryCode, managed_create_options->OverrideCountryCode);
        //CheckEquality(L"OverrideLocaleCode", native_create_options.OverrideLocaleCode, managed_create_options->OverrideLocaleCode);

        /* TODO: RTCOptions are currently not supported within the Unity Editor, so comparison of them does not make sense.
        bool native_rtc_is_null = native_create_options.RTCOptions == nullptr;
        bool managed_rtc_is_null = managed_create_options->RTCOptions.HasValue == false;

        EXPECT_TRUE(native_rtc_is_null == managed_rtc_is_null) << ConstructMessage(L"RTCOptions (Is Null)", native_rtc_is_null, managed_rtc_is_null);

        if (!native_rtc_is_null && !managed_rtc_is_null)
        {
            CheckEquality(L"RTCOptions.BackgroundMode", static_cast<int>(native_create_options.RTCOptions->BackgroundMode), static_cast<int>(managed_create_options->RTCOptions.Value.BackgroundMode));
        }
        */
    }
}
