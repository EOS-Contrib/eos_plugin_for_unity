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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if !EOS_DISABLE

namespace PlayEveryWare.EpicOnlineServices
{
    using System;
    using System.Collections.Generic;
    using Editor.Config;
    using Editor.Utility;

#if !EOS_DISABLE
    using Epic.OnlineServices.IntegratedPlatform;
#endif

    using Newtonsoft.Json;
    using Utility;

    [ConfigGroup("Steam Configuration", new string[]
    {
        "Steamworks SDK",
    })]
    public class SteamConfig : Config
    {
        #region These fields are referenced by the native code 

        [DirectoryPathField("Override Library Path",
            "Path to where you have your Steamworks SDK installed.", 0)]
        public string overrideLibraryPath;

        [ConfigField("SDK Major Version",
            ConfigFieldType.Uint,
            "The major version for the Steamworks SDK you have installed.", 0)]
        public uint steamSDKMajorVersion;

        [ConfigField("SDK Minor Version",
            ConfigFieldType.Uint,
            "The minor version for the Steamworks SDK you have installed.", 0)]
        public uint steamSDKMinorVersion;

        [ConfigField("Steamworks Interface Versions", ConfigFieldType.TextList, null, 0)]
        public List<string> steamApiInterfaceVersionsArray;

#if !EOS_DISABLE
        /// <summary>
        /// Used to store integrated platform management flags.
        /// </summary>
        [ConfigField("Integrated Platform Management Flags",
            ConfigFieldType.Enum, "Integrated Platform Management " +
                                  "Flags for platform specific options.",
            1, "https://dev.epicgames.com/docs/api-ref/enums/eos-e-integrated-platform-management-flags")]
        [JsonConverter(typeof(ListOfStringsToIntegratedPlatformManagementFlags))]
        public IntegratedPlatformManagementFlags integratedPlatformManagementFlags;

        // This property exists to maintain backwards-compatibility with 
        // previous versions of the config json structures.
        [JsonProperty] // Mark it so that it gets read
        [JsonIgnore] // Ignore so that it does not get written
        [Obsolete("This property is deprecated. Use the property integratedPlatformManagementFlags instead.")]
        [JsonConverter(typeof(ListOfStringsToIntegratedPlatformManagementFlags))]
        public IntegratedPlatformManagementFlags flags
        {
            get
            {
                return integratedPlatformManagementFlags;
            }
            set
            {
                integratedPlatformManagementFlags = value;
            }
        }
#endif

        #endregion

        [ButtonField("Update from Steamworks.NET", "Click here to try and import the SDK versions from the indicated Steamworks SDK Library referenced above", 0)]
        [JsonIgnore]
        public Action UpdateFromSteamworksNET;

        static SteamConfig()
        {
            RegisterFactory(() => new SteamConfig());
        }

        protected SteamConfig() : base("eos_steam_config.json")
        {
            UpdateFromSteamworksNET = () =>
            {
                string steamworksVersion = SteamworksUtility.GetSteamworksVersion();

                if (Version.TryParse(steamworksVersion, out Version version))
                {
                    _ = SafeTranslatorUtility.TryConvert(version.Major, out steamSDKMajorVersion);
                    _ = SafeTranslatorUtility.TryConvert(version.Minor, out steamSDKMinorVersion);
                }

                steamApiInterfaceVersionsArray = SteamworksUtility.GetSteamInterfaceVersions();
            };
        }
    }
}

#endif