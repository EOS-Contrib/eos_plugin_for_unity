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

// Uncomment the following line to see all platforms, even ones that are not
// available
//#define DEBUG_SHOW_UNAVAILABLE_PLATFORMS

#if !EOS_DISABLE

namespace PlayEveryWare.EpicOnlineServices.Editor.Windows
{
#if !EOS_DISABLE
    using Epic.OnlineServices.UI;
#endif
    using PlayEveryWare.EpicOnlineServices.Utility;
    using System;
	using System.Collections.Generic;
    using System.Linq;
    using System.Threading.Tasks;
	using UnityEditor;
    using UnityEngine;

    /// <summary>
    /// Creates the view for showing the eos plugin editor config values.
    /// </summary>
    [Serializable]
    public class EOSSettingsWindow : EOSEditorWindow
    {
        /// <summary>
        /// The editor for the product information that is shared across all
        /// platforms (represents information that is common to all
        /// circumstances).
        /// </summary>
        private ConfigEditor<ProductConfig> _productConfigEditor = new();

        /// <summary>
        /// Stores the config editors for each of the platforms.
        /// </summary>
        private readonly IList<IPlatformConfigEditor> _platformConfigEditors = new List<IPlatformConfigEditor>();

        /// <summary>
        /// Contains the GUIContent that represents the set of tabs that contain
        /// platform icons and platform text (this is not the tab _content_).
        /// </summary>
        private GUIContent[] _platformTabs;

        /// <summary>
        /// The tab that is currently selected.
        /// </summary>
        private int _selectedTab = -1;

        /// <summary>
        /// The style to apply to the platform tabs.
        /// </summary>
        private static GUIStyle _platformTabsStyle;

        /// <summary>
        /// The style to apply to the platform tabs, uses lazy initialization.
        /// </summary>
        private static GUIStyle TAB_STYLE => _platformTabsStyle ??= new(GUI.skin.button)
        {
            fontSize = 14,
            padding = new RectOffset(10, 10, 10, 10),
            alignment = TextAnchor.MiddleCenter,
            fixedHeight = 60
        };

        public EOSSettingsWindow() : base("EOS Configuration") { }

        [MenuItem("EOS Plugin/EOS Configuration", priority = 1)]
        public static void ShowWindow()
        {
            var window = GetWindow<EOSSettingsWindow>();
            window.SetIsEmbedded(false);
        }

        protected override void OnEnable()
        {
            base.OnEnable();
            ProductConfig.DeploymentsUpdatedEvent += ReloadDeploymentSettingsForPlatformConfigEditors;
            ProductConfig.ClientCredentialsUpdatedEvent += ReloadClientCredentialsForPlatformConfigEditors;
        }

        protected override void OnDestroy()
        {
            ProductConfig.DeploymentsUpdatedEvent -= ReloadDeploymentSettingsForPlatformConfigEditors;
            ProductConfig.ClientCredentialsUpdatedEvent -= ReloadClientCredentialsForPlatformConfigEditors;
            base.OnDestroy();
        }

        // TODO: Refactor to reduce massive overlap between this function and 
        //       the older ReloadDeploymentSettingsForPlatformConfigEditors
        //       function.
        //       The Observable pattern would be appropriate - but 
        //       such a change would constitute a not insignificant change, and
        //       should be avoided until there is a time to properly review it.
        private void ReloadClientCredentialsForPlatformConfigEditors(object sender,
            ProductConfig.PlatformConfigsUpdatedEventArgs e)
        {
            // For each of the platform config editors
            foreach (IPlatformConfigEditor platformConfigEditor in _platformConfigEditors)
            {
                // If the platform config was not one of the ones updated, then skip it.
                if (!e.PlatformConfigsUpdated.Contains(platformConfigEditor.GetPlatform()))
                {
                    continue;
                }

                // If the platform config could not be read from disk
                if (!PlatformManager.TryGetConfig(platformConfigEditor.GetPlatform(),
                        out PlatformConfig platformConfigFromDisk))
                {
                    // TODO: Log warning?
                    continue;
                }

                // Update the client credentials for the cached instance of the
                // config within the PlatformConfigEditor.
                platformConfigEditor.SetClientCredentials(platformConfigFromDisk.clientCredentials);
            }
            Repaint();
        }

        // TODO: Refactor to reduce massive overlap between this function and 
        //       the more recently introduced
        //       ReloadClientCredentialsForPlatformConfigEditors function.
        //       The Observable pattern would be appropriate - but 
        //       such a change would constitute a not insignificant change, and
        //       should be avoided until there is a time to properly review it.
        private void ReloadDeploymentSettingsForPlatformConfigEditors(object sender, ProductConfig.PlatformConfigsUpdatedEventArgs e)
        {
            // For each of the platform config editors
            foreach (IPlatformConfigEditor platformConfigEditor in _platformConfigEditors)
            {
                // If the platform config was not one of the ones updated, then skip it.
                if (!e.PlatformConfigsUpdated.Contains(platformConfigEditor.GetPlatform()))
                {
                    continue;
                }

                // If the platform config could not be read from disk
                if (!PlatformManager.TryGetConfig(platformConfigEditor.GetPlatform(),
                        out PlatformConfig platformConfigFromDisk))
                {
                    // TODO: Log warning?
                    continue;
                }

                // Update the deployment for the cached instance of the config
                // within the PlatformConfigEditor.
                platformConfigEditor.SetDeployment(platformConfigFromDisk.deployment);
            }
            Repaint();
        }

        protected override async Task AsyncSetup()
        {
            await _productConfigEditor.LoadAsync();
            int tabIndex = 0;
            foreach (PlatformManager.Platform platform in Enum.GetValues(typeof(PlatformManager.Platform)))
            {
                if (!PlatformManager.TryGetConfigType(platform, out Type configType) || null == configType)
                {
                    continue;
                }

                // This makes sure that the currently selected tab (upon first loading the window) is always the current platform.
                if (_selectedTab == -1 && platform == PlatformManager.CurrentTargetedPlatform)
                {
                    _selectedTab = tabIndex;
                }

                Type constructedType =
                    typeof(PlatformConfigEditor<>).MakeGenericType(configType);

                if (Activator.CreateInstance(constructedType) is not IPlatformConfigEditor editor)
                {
                    Debug.LogError($"Could not load config editor for platform \"{platform}\".");
                    continue;
                }

#if !DEBUG_SHOW_UNAVAILABLE_PLATFORMS
                // Do not add the platform if it is not currently available.
                if (!editor.IsPlatformAvailable())
                {
                    continue;
                }
#endif

                tabIndex++;

                _platformConfigEditors.Add(editor);
            }

            // If (for some reason) a default platform was not selected, then
            // default to the first tab being selected
            if (_selectedTab == -1)
            {
                _selectedTab = 0;
            }

            _platformTabs = BuildPlatformTabsDynamic();
        }

        protected override void RenderWindow()
        {
            if (_selectedTab < 0)
            {
                _selectedTab = 0;
            }

            // Render the generic product configuration stuff.
            _ = _productConfigEditor.RenderAsync();

            if (_platformTabs != null && _platformConfigEditors.Count != 0)
            {
                _selectedTab = GUILayout.Toolbar(_selectedTab, _platformTabs, TAB_STYLE);

                GUILayout.Space(30);

                _ = _platformConfigEditors[_selectedTab].RenderAsync();
            }

            GUI.SetNextControlName("Save");
            if (GUILayout.Button("Save All Changes"))
            {
                GUI.FocusControl("Save");
                Save();
            }
        }

        private void Save()
        {
            // Save the product config editor
            _productConfigEditor.Save();

            //Update deployment in current platform
            if(!ProductConfig.Get<ProductConfig>().Environments.TryGetFirstDefinedNamedDeployment(out var namedDep))
            {
                Debug.LogError($"{nameof(EOSSettingsWindow)} {nameof(Save)}: No named deployment found for current platform tab: {_platformConfigEditors[_selectedTab].GetPlatform()}");
                return;
            }
            // Save each of the platform config editors.
            foreach (IConfigEditor editor in _platformConfigEditors)
            {
                editor.Save();
            }
            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();
            _platformTabs = BuildPlatformTabsDynamic();
            Repaint();
        }

        /// <summary>
        /// Determines whether a Deployment object represents a valid and fully defined deployment.
        /// </summary>
        private static bool IsDeploymentSet(Deployment dep)
        {
            return dep.IsComplete && dep.DeploymentId != Guid.Empty;
        }

        /// <summary>
        /// Produces a simple text label describing the deployment type based on its name.
        /// </summary>
        private static bool TryGetDeploymentDisplayName(ProductionEnvironments envs, Deployment target, out string displayName)
        {
            displayName = null;
            if (envs == null || target.DeploymentId == Guid.Empty)
            {
                return false;
            }

            var named = envs.Deployments.FirstOrDefault(d => d != null && d.Value.DeploymentId == target.DeploymentId);

            if (named != null)
            {
                displayName = named.Name;
                return true;
            }

            return false;
        }

        /// <summary>
        /// Converts a GUID into a shorter, UI-friendly representation.
        /// Used only as a fallback when a deployment does not have a valid name.
        /// </summary>
        private static string ShortenGuid(Guid id)
        {
            var s = id.ToString("N");
            return s.Length > 8 ? s.Substring(0, 8) : s;
        }

        /// <summary>
        /// Builds the GUIContent array used for drawing the platform selection toolbar.
        /// Unlike the static version initialized at window setup, this version refreshes
        /// dynamically so that each platform tab displays the current deployment name
        /// </summary>
        private GUIContent[] BuildPlatformTabsDynamic()
        {
            var product = ProductConfig.Get<ProductConfig>();
            var envs = product?.Environments;
            var contents = new List<GUIContent>(_platformConfigEditors.Count);

            foreach (var editor in _platformConfigEditors)
            {
                string platformLabel = editor.GetLabelText();
                string deploymentName = "-";

                if (PlatformManager.TryGetConfig(editor.GetPlatform(), out PlatformConfig cfg) && cfg != null)
                {
                    if (IsDeploymentSet(cfg.deployment))
                    {
                        if (TryGetDeploymentDisplayName(envs, cfg.deployment, out var displayName))
                        {
                            deploymentName = displayName;
                        }
                        else
                        {
                            deploymentName = ShortenGuid(cfg.deployment.DeploymentId);
                        }
                    }
                    else
                    {
                        deploymentName = "nameless";
                    }

                }

                string text = $" {platformLabel} \n [{deploymentName}]";
                contents.Add(new GUIContent(text, editor.GetPlatformIconTexture()));
            }

            return contents.ToArray();
        }
    }
}

#endif