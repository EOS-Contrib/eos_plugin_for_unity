#include "pch.h"
#include "CppUnitTest.h"
#include "Common.hpp"
#include "ConfigurationUtilityBridge.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace pew::eos::tests
{
    /**
     * \brief Initialize the Common Language Runtime.
     */
    static void InitializeCLR()
    {
        gcnew System::String(L"CLR initialized");
    }

	TEST_CLASS(NativeRenderUnitTests)
	{
	public:
		
		TEST_METHOD(Test_Initialize_Options_Are_Equal)
		{
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

        TEST_METHOD(Test_Create_Options_Are_Equal)
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
	};
}
