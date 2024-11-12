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

namespace PlayEveryWare.EpicOnlineServices
{
    using Epic.OnlineServices.Auth;
    using Epic.OnlineServices.IntegratedPlatform;
    using Epic.OnlineServices.UI;
    using System;
    using System.Diagnostics.CodeAnalysis;
    using System.Runtime.InteropServices;

    // Flags specifically for Windows
    [Serializable]
    [ConfigGroup("Windows Config", new[]
    {
        "Windows-Specific Options",
        "Deployment",
        "Flags",
        "Tick Budgets",
        "Overlay Options"
    }, false)]
    [DynamicallyAccessedMembers(DynamicallyAccessedMemberTypes.All)]
    public class WindowsConfig : PlatformConfig
    {
        static WindowsConfig()
        {
            RegisterFactory(() => new WindowsConfig());
        }

        protected WindowsConfig() : base(PlatformManager.Platform.Windows) { }

#if EXTERNAL_TO_UNITY

        [UnmanagedCallersOnly(EntryPoint = "GetWindowsConfig")]
        public static Guid GetDeploymentId()
        {
            return Get<WindowsConfig>().deployment.DeploymentId;
        }

        [UnmanagedCallersOnly(EntryPoint = "GetSandboxId")]
        public static IntPtr GetSandboxId()
        {
            string sandboxIdString = Get<WindowsConfig>().deployment.SandboxId.Value;

            return Marshal.StringToHGlobalAnsi(sandboxIdString);
        }

        [UnmanagedCallersOnly(EntryPoint = "GetIsServer")]
        public static bool GetIsServer()
        {
            return Get<WindowsConfig>().isServer;
        }

        [UnmanagedCallersOnly(EntryPoint = "GetAuthScopeOptionsFlags")]
        public static AuthScopeFlags GetAuthScopeOptionsFlags()
        {
            return Get<WindowsConfig>().authScopeOptionsFlags;
        }

        [UnmanagedCallersOnly(EntryPoint = "GetIntegratedPlatformManagementFlags")]
        public static IntegratedPlatformManagementFlags GetIntegratedPlatformManagementFlags()
        {
            return Get<WindowsConfig>().integratedPlatformManagementFlags;
        }

        [UnmanagedCallersOnly(EntryPoint = "GetTickBudgetInMilliseconds")]
        public static uint GetTickBudgetInMilliseconds()
        {
            return Get<WindowsConfig>().tickBudgetInMilliseconds;
        }

        [UnmanagedCallersOnly(EntryPoint = "GetTaskNetworkTimeoutSeconds")]
        public static double GetTaskNetworkTimeoutSeconds()
        {
            return Get<WindowsConfig>().taskNetworkTimeoutSeconds;
        }

        [UnmanagedCallersOnly(EntryPoint = "GetThreadAffinity")]
        public static WrappedInitializeThreadAffinity GetThreadAffinity()
        {
            return Get<WindowsConfig>().threadAffinity;
        }

        [UnmanagedCallersOnly(EntryPoint = "GetAlwaysSendInputToOverlay")]
        public static bool GetAlwaysSendInputToOverlay()
        {
            return Get<WindowsConfig>().alwaysSendInputToOverlay;
        }

        [UnmanagedCallersOnly(EntryPoint = "GetInitialButtonDelayForOverlay")]
        public static float GetInitialButtonDelayForOverlay()
        {
            return Get<WindowsConfig>().initialButtonDelayForOverlay;
        }

        [UnmanagedCallersOnly(EntryPoint = "GetRepeatButtonDelayForOverlay")]
        public static float GetRepeatButtonDelayForOverlay()
        {
            return Get<WindowsConfig>().repeatButtonDelayForOverlay;
        }

        [UnmanagedCallersOnly(EntryPoint = "GetToggleFriendsButtonCombination")]
        public static InputStateButtonFlags GetToggleFriendsButtonCombination()
        {
            return Get<WindowsConfig>().toggleFriendsButtonCombination;
        }
#endif
    }
}