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

namespace PlayEveryWare.EpicOnlineServices.Tests.Native
{
    using NUnit.Framework;
    using System;
    using System.IO;
    using System.Runtime.InteropServices;
    using UnityEngine;

    public class NativeComponentTests
    {
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        internal struct EOS_Platform_ClientCredentials
        {
            [MarshalAs(UnmanagedType.LPStr)]
            public string ClientId;

            [MarshalAs(UnmanagedType.LPStr)]
            public string ClientSecret;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        internal struct EOS_Platform_Options
        {
            public int ApiVersion;

            // in C++ the point is void*
            public IntPtr Reserved;

            [MarshalAs(UnmanagedType.LPStr)]
            public string ProductId;

            [MarshalAs(UnmanagedType.LPStr)]
            public string SandboxId;

            // Points to EOS_Platform_ClientCredentials struct
            [MarshalAs(UnmanagedType.LPStr)] public IntPtr ClientCredentials;

            // Stored as int32_t in C++ 1 == true, 0 == false
            public int bIsServer;

            [MarshalAs(UnmanagedType.LPStr)]
            public string EncryptionKey;
            [MarshalAs(UnmanagedType.LPStr)]
            public string OverrideCountryCode;
            [MarshalAs(UnmanagedType.LPStr)]
            public string OverrideLocaleCode;
            [MarshalAs(UnmanagedType.LPStr)]
            public string DeploymentId;

            public UInt64 Flags;

            [MarshalAs(UnmanagedType.LPStr)]
            public string CacheDirectory;

            // Pointer to EOS_Platform_RTCOptions object in C++
            public IntPtr RTCOptions;

            // Points to EOS_HIntegratedPlatformOptionsContainer
            public IntPtr IntegratedPlatformOptionsContainerHandle;

            // Points to void*
            public IntPtr SystemSpecificOptions;

            [MarshalAs(UnmanagedType.LPStr)]
            public IntPtr TaskNetworkTimeoutSeconds;
        }

        private const string DllName = "GfxPluginNativeRender-x64.dll";
        private static readonly string DllPath = Path.Combine("Assets", "Plugins", "Windows", "x64", DllName);

        [DllImport("kernel32", SetLastError = true, CharSet = CharSet.Auto)]
        private static extern IntPtr LoadLibrary(string lpFileName);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr EOS_PEW_GetPlatformOptions();

        [SetUp]
        public static void LoadDLLManually()
        {
            if (!File.Exists(DllPath))
            {
                throw new FileNotFoundException("DLL NOT FOUND");
            }

            IntPtr handle = LoadLibrary(DllPath);
            if (handle == IntPtr.Zero)
            {
                throw new Exception("Failed to load DLL");
            }
            else
            {
                Debug.Log("Successfully Loaded DLL");
            }
        }

        [Test]
        public static void LoadPlatformOptionsAsParsedByNative()
        {
            IntPtr natively_parsed_options_ptr = EOS_PEW_GetPlatformOptions();

            EOS_Platform_Options natively_parsed_options =
                Marshal.PtrToStructure<EOS_Platform_Options>(natively_parsed_options_ptr);

            Debug.Log("testing");

        }
    }
}