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
    using Common;
    using System;
    using System.Collections.Generic;
    using Newtonsoft.Json;

#if !EXTERNAL_TO_UNITY
    using UnityEngine;
#endif

    /// <summary>
    /// Contains information about the product entered by the user from the Epic
    /// Developer Portal.
    /// </summary>
    [ConfigGroup("Product Configuration", new[] { "", "Deployment Configuration" }, false)]
    public class ProductConfig : Config
    {
        /// <summary>
        /// The product ID is a unique GUID labeled "Product ID" in the Epic
        /// Developer Portal. The name for this value can be set to anything -
        /// it is used as a label for user interface purposes - and is allowed
        /// to differ from the label given to it on the Developer Portal.
        /// </summary>
        [ConfigField("Product Name",
            ConfigFieldType.Text,
            "Enter your product name as it appears in the EOS Dev " +
            "Portal here.",
            0)]
        public string ProductName;

        [ConfigField("Product Id",
            ConfigFieldType.Guid,
            "Enter your Product Id as it appears in the EOS Dev " +
            "Portal here.",
            0)]
        public Guid ProductId;

        [ConfigField("Version",
            ConfigFieldType.Text,
            "Use this to indicate to the EOS SDK your game version.",
            0)]
        public string ProductVersion;

        /// <summary>
        /// This additional flag determines whether or not the productconfig has
        /// been imported. The reason that schema alone is not sufficient for
        /// this is because product_config is a new file altogether, so when it
        /// is first created, it will have the newest schema version, and
        /// migration will need to take place.
        /// </summary>
        [JsonProperty("imported")]
        private bool _configImported = false;

#if !EOS_DISABLE
        /// <summary>
        /// The set of Clients as defined within the Epic Developer Portal. For
        /// EOS to function, at least one of these must be set, and the
        /// platform config needs to indicate which one to use. (If none is
        /// explicitly indicated, and only one is defined, that one will be
        /// used).
        /// </summary>
        [ConfigField("Client Credentials",
            ConfigFieldType.SetOfClientCredentials,
            "Enter the client credentials you have defined in the " +
            "Epic Dev Portal.", 1)]
        public SetOfNamed<EOSClientCredentials> Clients = new("Client");
#endif

        /// <summary>
        /// The set of Sandboxes as defined within the Epic Developer Portal.
        /// For EOS to function, at least one of these must be set, and it must
        /// match the deployment indicated by the platform config.
        /// </summary>
        [ConfigField("Production Environments",
            ConfigFieldType.ProductionEnvironments,
            "Enter the details of your deployment and sandboxes as they " +
            "exist within the Epic Dev Portal.", 1)]
        public ProductionEnvironments Environments = new();

        /// <summary>
        /// This field member is used to determine when deployments are first
        /// defined by the user. This determination is used to trigger the
        /// process of setting the default deployment for the platforms.
        /// </summary>
        [JsonIgnore]
        private bool _deploymentDefinedWhenLoaded;

        /// <summary>
        /// This field member is used to determine when client credentials are
        /// first defined by the user. This determination is used to trigger the
        /// process of setting the default deployment for the platforms.
        /// </summary>
        [JsonIgnore]
        private bool _clientCredentialsDefinedWhenLoaded;

        /// <summary>
        /// Used to store information about what platform configs have been
        /// updated.
        /// TODO: Implement via the Observable pattern instead.
        /// </summary>
        public sealed class PlatformConfigsUpdatedEventArgs : EventArgs
        {
            /// <summary>
            /// A list of all the platform configs that have been updated.
            /// </summary>
            public readonly IEnumerable<PlatformManager.Platform> PlatformConfigsUpdated;

            public PlatformConfigsUpdatedEventArgs(
                IEnumerable<PlatformManager.Platform> platformConfigsUpdated)
            {
                PlatformConfigsUpdated = platformConfigsUpdated;
            }
        }

        public static event EventHandler<PlatformConfigsUpdatedEventArgs> DeploymentsUpdatedEvent;

        public static event EventHandler<PlatformConfigsUpdatedEventArgs>
            ClientCredentialsUpdatedEvent;

        static ProductConfig()
        {
            RegisterFactory(() => new ProductConfig());
        }

        protected override bool NeedsMigration()
        {
            return base.NeedsMigration() || !_configImported;
        }
        
        protected ProductConfig() : base("eos_product_config.json") { }

        protected override void OnReadCompleted()
        {
            // This tracks whether there is a single deployment defined. The
            // out parameter is discarded because it is not needed at this 
            // juncture.
            _deploymentDefinedWhenLoaded = Environments.TryGetFirstDefinedNamedDeployment(out _);

            // This tracks whether there is a single client credential
            // completely defined when the product config is loaded. The out
            // parameter is discarded because it is not needed at this juncture.
            _clientCredentialsDefinedWhenLoaded = TryGetFirstCompleteNamedClientCredentials(out _);
        }

        public bool TryGetFirstCompleteNamedClientCredentials(out Named<EOSClientCredentials> credentials)
        {
            credentials = null;

            foreach (var clientCredentials in Clients)
            {
                if (clientCredentials.Value.IsComplete)
                {
                    credentials = clientCredentials;
                    break;
                }
            }

            return (credentials != null);
        }

        // This compile conditional is here because the OnWriteCompleted method
        // is only defined in the editor - so it can only be overriden if in the
        // editor.
#if UNITY_EDITOR

        protected override void BeforeWrite()
        {
            // If there is one deployment, and one sandbox, then make sure they
            // are linked to each other if the sandbox is not empty.
            // But only do this if they have been newly added
            if (!_deploymentDefinedWhenLoaded && Environments.Deployments.Count ==1 && Environments.Sandboxes.Count == 1 && !Environments.Sandboxes[0].Value.IsEmpty)
            {
                Environments.Deployments[0].Value.SandboxId = Environments.Sandboxes[0].Value;
            }
        }

        // TODO: Refactor to reduce massive overlap between this function and
        //       the more recently introduced
        //       UpdatePlatformConfigClientCredentials function.
        //       The Observable pattern would be appropriate - but such a change
        //       would constitute a not insignificant refactor and should be
        //       avoided until there is a time to properly review it.
        private void UpdatePlatformConfigDeployments()
        {
            bool definedDeploymentExists =
                Environments.TryGetFirstDefinedNamedDeployment(out Named<Deployment> deploymentToSetPlatformsTo);

            // If when the config was last read there was a deployment defined,
            // or there is not now one defined - then there is no need to try
            // and set the deployment values for each platform config.
            if (_deploymentDefinedWhenLoaded ||
                !definedDeploymentExists)
            {
                return;
            }

            List<PlatformManager.Platform> platformConfigsUpdated = new();

            // For each platform for which configuration can be done
            foreach (var platform in PlatformManager.ConfigurablePlatforms)
            {
                // If the PlatformConfig could not be retrieved, continue to the
                // next.
                if (!PlatformManager.TryGetConfig(platform, out PlatformConfig config))
                {
                    continue;
                }

                // If the config already has a completely defined deployment,
                // then do not override, and move to the next platform config
                if (config.deployment.IsComplete)
                {
                    continue;
                }

                // Add to the list of platform configs that have been updated
                platformConfigsUpdated.Add(platform);

                // Set the deployment.
                config.deployment = deploymentToSetPlatformsTo.Value;

                // Tell the user
                Debug.Log($"Deployment for platform " +
                          $"\"{config.Platform}\" has been defaulted to " +
                          $"{deploymentToSetPlatformsTo}.");

                // Save the config
                config.Write();
            }

            // If at least one platform config was updated as a result, trigger
            // the event that indicates as much
            if (platformConfigsUpdated.Count > 0)
            {
                DeploymentsUpdatedEvent?.Invoke(this, new PlatformConfigsUpdatedEventArgs(platformConfigsUpdated));
            }
        }

        // TODO: Refactor to reduce massive overlap between this function and
        //       the older UpdatePlatformConfigDeployments function.
        //       The Observable pattern would be appropriate - but such a change
        //       would constitute a not insignificant refactor and should be
        //       avoided until there is a time to properly review it.
        private void UpdatePlatformConfigClientCredentials()
        {
            bool completeClientCredentialsExist =
                TryGetFirstCompleteNamedClientCredentials(out Named<EOSClientCredentials> credentialsToSetPlatformsTo);

            // If when the config was last read there was a set of client
            // credentials already defined, or there is not now one defined -
            // then there is no need to try and set the client credential values
            // for each platform config.
            if (_deploymentDefinedWhenLoaded ||
                !completeClientCredentialsExist)
            {
                return;
            }

            List<PlatformManager.Platform> platformConfigsUpdated = new();

            // For each platform for which configuration can be done
            foreach (var platform in PlatformManager.ConfigurablePlatforms)
            {
                // If the PlatformConfig could not be retrieved, continue to the
                // next.
                if (!PlatformManager.TryGetConfig(platform, out PlatformConfig config))
                {
                    continue;
                }

                // If the config already has a completely defined set of client
                // credentials, then do not override, and move to the next
                // platform config.
                if (config.clientCredentials != null && config.clientCredentials.IsComplete)
                {
                    continue;
                }

                // Add to the list of platform configs that have been updated
                platformConfigsUpdated.Add(platform);

                // Set the client credentials.
                config.clientCredentials = credentialsToSetPlatformsTo.Value;

                // Tell the user
                Debug.Log($"Client credentials for platform " +
                          $"\"{config.Platform}\" has been defaulted to " +
                          $"{credentialsToSetPlatformsTo}.");

                // Save the config
                config.Write();
            }

            // If at least one platform config was updated as a result, trigger
            // the event that indicates as much
            if (platformConfigsUpdated.Count > 0)
            {
                ClientCredentialsUpdatedEvent?.Invoke(this, new PlatformConfigsUpdatedEventArgs(platformConfigsUpdated));
            }
        }

        protected override void OnWriteCompleted()
        {
            // Update the platform config deployments if needed.
            UpdatePlatformConfigDeployments();

            // Update the platform config client credentials if needed.
            UpdatePlatformConfigClientCredentials();
        }

#endif

        #region Functionality to migrate from old configuration to new

        internal class PreviousEOSConfig : Config
        {
            public string productName;
            public string productVersion;
            public string productID;
            public List<SandboxDeploymentOverride> sandboxDeploymentOverrides;
            public string sandboxID;
            public string deploymentID;
            public string clientSecret;
            public string clientID;
            public string encryptionKey;

            static PreviousEOSConfig()
            {
                RegisterFactory(() => new PreviousEOSConfig());
            }

            protected PreviousEOSConfig() : base("EpicOnlineServicesConfig.json") { }
        }

        private void MigrateProductNameVersionAndId(PreviousEOSConfig config)
        {
            ProductName = config.productName;
            ProductVersion = config.productVersion;

            // Attempt to parse the productID, and log a message if it cannot be parsed
            // Do not log a message if the productID is empty, and could not possibly migrate
            if (!string.IsNullOrWhiteSpace(config.productID) && !Guid.TryParse(config.productID, out ProductId))
            {
                Debug.LogWarning("Could not parse product ID.");
            }
        }

        private void MigrateClientCredentials(PreviousEOSConfig config)
        {
#if !EOS_DISABLE
            // Import the old config client stuff
            // Some amount of these values should be provided, though encryptionKey is optional
            if (!string.IsNullOrWhiteSpace(config.clientID) && !string.IsNullOrWhiteSpace(config.clientSecret))
            {
                Clients.Add(new EOSClientCredentials(config.clientID, config.clientSecret,
                    config.encryptionKey));
            }
#endif
        }

        private void MigrateSandboxAndDeployment(PreviousEOSConfig config)
        {
            // Check to see if the sandbox and deployment id were configured in the previous config
            if (string.IsNullOrWhiteSpace(config.sandboxID) || string.IsNullOrEmpty(config.deploymentID))
            {
                // One of them is empty, we can't possibly add this deployment
                return;
            }

            // Import explicitly set sandbox and deployment
            SandboxId sandboxId = new()
            {
                Value = config.sandboxID
            };

            Deployment oldDeployment = new()
            {
                DeploymentId = Guid.Parse(config.deploymentID),
                SandboxId = sandboxId
            };

            if (!Environments.AddDeployment(oldDeployment))
            {
                Debug.LogWarning("Could not import deployment " +
                                 "details from old config file. Please " +
                                 "reach out for support if you need " +
                                 "assistance.");
            }
        }

        private void MigrateSandboxAndDeploymentOverrides(PreviousEOSConfig config)
        {
            // If the sandboxDeploymentOverrides didn't parse, they were likely missing
            // Only migrate values if they are present
            if (config.sandboxDeploymentOverrides == null)
            {
                return;
            }

            // Import each of the overrides
            foreach (var overrideValues in config.sandboxDeploymentOverrides)
            {
                SandboxId overrideSandbox = new() { Value = overrideValues.sandboxID };
                Deployment overrideDeployment = new()
                {
                    DeploymentId = Guid.Parse(overrideValues.deploymentID),
                    SandboxId = overrideSandbox
                };

                Environments.Deployments.Add(overrideDeployment);
            }
        }

        protected override void MigrateConfig()
        {
            Environments ??= new();

            PreviousEOSConfig oldConfig = Get<PreviousEOSConfig>();

            MigrateProductNameVersionAndId(oldConfig);
            MigrateClientCredentials(oldConfig);
            MigrateSandboxAndDeployment(oldConfig);
            MigrateSandboxAndDeploymentOverrides(oldConfig);

            _configImported = true;
        }
        #endregion
    }
}

#endif