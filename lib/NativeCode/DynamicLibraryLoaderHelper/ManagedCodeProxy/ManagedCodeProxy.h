#pragma once

#using <mscorlib.dll>
#using <System.dll>

#using <EOSPluginConfig-x64.dll>

using namespace System;
using namespace PlayEveryWare::EpicOnlineServices;

namespace ManagedCodeProxy {
    class test
    {
        
    };
    public ref class ConfigWrapper
    {
    public:
        static WindowsConfig^ GetWindowsConfig()
        {
            return Config::GetWindowsConfig();
        }
	};
}
