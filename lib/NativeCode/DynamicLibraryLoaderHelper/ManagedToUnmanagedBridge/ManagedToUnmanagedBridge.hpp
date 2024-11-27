#pragma once

using namespace System;

#using <ManagedPluginCode.dll>

namespace pew::eos::tests {
    using namespace Epic::OnlineServices::Platform;
    using namespace PlayEveryWare::EpicOnlineServices::Utility;

    public ref class NativeTestUtilityBridge
    {
    public:
        static WindowsOptions^ get_windows_options()
        {
            return ConfigurationUtility::GetEOSCreateOptions()->options;
        }
        static InitializeOptions^ get_initialize_options()
        {
            return ConfigurationUtility::GetEOSInitializeOptions()->options;
        }
	};
}
