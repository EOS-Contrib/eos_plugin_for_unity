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

namespace PlayEveryWare.EpicOnlineServices
{
    using System;
    using System.Collections.Generic;
    using Editor;
    using Editor.Config;
    using Editor.Utility;
    using Epic.OnlineServices.IntegratedPlatform;
    using Newtonsoft.Json;
    using Utility;

    [Serializable]
    [ConfigGroup("Steam Configuration")]
    public class SteamConfig : EditorConfig
    {
        /// <summary>
        /// Used to store integrated platform management flags.
        /// </summary>
        [ConfigField("Integrated Platform Management Flags",
            ConfigFieldType.Enum, "Integrated Platform Management " +
                                  "Flags for platform specific options.",
            2, "https://dev.epicgames.com/docs/api-ref/enums/eos-e-integrated-platform-management-flags")]
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

        #region These fields are referenced by the native code 

        [DirectoryPathField("Override Library Path")]
        public string overrideLibraryPath;

        [ConfigField("Steamworks SDK Major Version", ConfigFieldType.Uint)]
        public uint steamSDKMajorVersion;

        [ConfigField("Steamworks SDK Minor Version", ConfigFieldType.Uint)]
        public uint steamSDKMinorVersion;

        [ConfigField("Steamworks Interface Versions", ConfigFieldType.TextList)]
        public List<string> steamApiInterfaceVersionsArray;

        #endregion

        [ButtonField("Update from Steamworks.NET")]
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

