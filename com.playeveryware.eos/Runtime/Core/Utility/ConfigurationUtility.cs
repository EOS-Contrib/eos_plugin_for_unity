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

namespace PlayEveryWare.EpicOnlineServices.Utility
{
    using Epic.OnlineServices;
    using Epic.OnlineServices.Platform;
    using Extensions;
    using System;
    using Config = EpicOnlineServices.Config;

#if !EXTERNAL_TO_UNITY
    using UnityEngine;
#endif

    /// <summary>
    /// This class is used to segregate the code responsible for taking
    /// configuration values (as stored in JSON and editable by the user from
    /// within the Unity editor) and converting those values into the data
    /// structures that the EOS SDK requires to be created and initialized.
    ///
    /// The primary purpose of doing this - aside from separating the concerns,
    /// is to allow efficient testing of the verisimilitude between the native
    /// and managed implementations of this process.
    /// </summary>
    public static class ConfigurationUtility
    {
        /// <summary>
        /// Get the create options used to make the EOS platform.
        /// </summary>
        /// <returns>
        /// An object that encapsulates the CreateOptions used for creating the
        /// EOS SDK platform.
        /// </returns>
        public static EOSCreateOptions GetEOSCreateOptions()
        {
            PlatformConfig platformConfig = PlatformManager.GetPlatformConfig();
            ProductConfig productConfig = Config.Get<ProductConfig>();

            IPlatformSpecifics platformSpecifics =
#if EXTERNAL_TO_UNITY
                new WindowsPlatformSpecifics()
#else
                EOSManagerPlatformSpecificsSingleton.Instance
#endif
            ;

            EOSCreateOptions platformOptions = new();

            platformOptions.options.CacheDirectory = platformSpecifics.GetTempDir();
            platformOptions.options.IsServer = platformConfig.isServer;
            platformOptions.options.Flags =
#if UNITY_EDITOR
                PlatformFlags.LoadingInEditor;
#else
            platformConfig.platformOptionsFlags.Unwrap();
#endif

            if (platformConfig.clientCredentials == null || platformConfig.clientCredentials.IsEncryptionKeyValid() == false)
            {
                Debug.LogError("The encryption key used for the selected client credentials is invalid. Please see your platform configuration.");
            }
            else
            {
                platformOptions.options.EncryptionKey = platformConfig.clientCredentials.EncryptionKey;
            }

            platformOptions.options.OverrideCountryCode = null;
            platformOptions.options.OverrideLocaleCode = null;
            platformOptions.options.ProductId = productConfig.ProductId.ToString("N").ToLowerInvariant();
            platformOptions.options.SandboxId = platformConfig.deployment.SandboxId.ToString();
            platformOptions.options.DeploymentId = platformConfig.deployment.DeploymentId.ToString("N").ToLowerInvariant();

            platformOptions.options.TickBudgetInMilliseconds = platformConfig.tickBudgetInMilliseconds;

            // configData has to serialize to JSON, so it doesn't represent null
            // If the value is <= 0, then set it to null, which the EOS SDK will handle by using default of 30 seconds.
            platformOptions.options.TaskNetworkTimeoutSeconds = platformConfig.taskNetworkTimeoutSeconds > 0 ? platformConfig.taskNetworkTimeoutSeconds : null;

            platformOptions.options.ClientCredentials = new ClientCredentials
            {
                ClientId = platformConfig.clientCredentials.ClientId,
                ClientSecret = platformConfig.clientCredentials.ClientSecret,
            };

#if !EXTERNAL_TO_UNITY

#if !(UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN || UNITY_STANDALONE_LINUX || UNITY_EDITOR_LINUX || UNITY_STANDALONE_OSX || UNITY_EDITOR_OSX)
            var createIntegratedPlatformOptionsContainerOptions = new Epic.OnlineServices.IntegratedPlatform.CreateIntegratedPlatformOptionsContainerOptions();
                var integratedPlatformOptionsContainer = new Epic.OnlineServices.IntegratedPlatform.IntegratedPlatformOptionsContainer();
                var integratedPlatformOptionsContainerResult = Epic.OnlineServices.IntegratedPlatform.IntegratedPlatformInterface.CreateIntegratedPlatformOptionsContainer(ref createIntegratedPlatformOptionsContainerOptions, out integratedPlatformOptionsContainer);
                
                if (integratedPlatformOptionsContainerResult != Result.Success)
                {
                    Debug.LogError($"Error creating integrated platform container: {integratedPlatformOptionsContainerResult}");
                }
                platformOptions.options.IntegratedPlatformOptionsContainerHandle = integratedPlatformOptionsContainer;
#endif
#endif

            // Note that this function only sets rtcoptions - which is not
            // exposed or affected by configuration, so it is not considered
            // when being called external to unity.
#if !EXTERNAL_TO_UNITY
            platformSpecifics.ConfigureSystemPlatformCreateOptions(ref platformOptions);
#endif
            return platformOptions;
        }

        /// <summary>
        /// Get the initialize options used to initialize the EOS SDK after it
        /// has been created.
        /// </summary>
        /// <returns>
        /// An object that encapsulates the initialization options utilized to
        /// initialize the EOS SDK after it has been created.
        /// </returns>
        public static EOSInitializeOptions GetEOSInitializeOptions()
        {
            EOSInitializeOptions initOptions = new() { options = new() };

            // Get the product config and the platform config
            ProductConfig productConfig = Config.Get<ProductConfig>();
            PlatformConfig platformConfig = PlatformManager.GetPlatformConfig();

            // Set the product name, version, and override thread affinity
            initOptions.options.ProductName = productConfig.ProductName;
            initOptions.options.ProductVersion = productConfig.ProductVersion;
            initOptions.options.OverrideThreadAffinity = platformConfig.threadAffinity?.Unwrap();

            initOptions.options.AllocateMemoryFunction = IntPtr.Zero;
            initOptions.options.ReallocateMemoryFunction = IntPtr.Zero;
            initOptions.options.ReleaseMemoryFunction = IntPtr.Zero;

#if !EXTERNAL_TO_UNITY
            IPlatformSpecifics platformSpecifics = EOSManagerPlatformSpecificsSingleton.Instance;
            platformSpecifics.ConfigureSystemInitOptions(ref initOptions);
#endif

            // Return;
            return initOptions;
        }
    }
}

#endif