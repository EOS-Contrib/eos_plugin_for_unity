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
	using System.Threading.Tasks;
	using UnityEditor;
    using UnityEngine;

    /// <summary>
    /// Creates the view for showing the eos plugin editor config values.
    /// </summary>
    [Serializable]
    public class NEW_EOSSettingsWindow : EOSEditorWindow
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
        private readonly IList<IConfigEditor> _platformConfigEditors = new List<IConfigEditor>();

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
            fixedHeight = 40
        };

        public NEW_EOSSettingsWindow() : base("EOS Configuration") { }

        [MenuItem("EOS Plugin/EOS Configuration", priority = 1)]
        public static void ShowWindow()
        {
            var window = GetWindow<NEW_EOSSettingsWindow>();
            window.SetIsEmbedded(false);
        }

        protected override async Task AsyncSetup()
        {
            await _productConfigEditor.LoadAsync();

            List<GUIContent> tabContents = new();
            int tabIndex = 0;
            foreach (PlatformManager.Platform platform in Enum.GetValues(typeof(PlatformManager.Platform)))
            {
                // This makes sure that the currently selected tab (upon first loading the window) is always the current platform.
                if (_selectedTab != -1 || platform == PlatformManager.CurrentPlatform)
                {
                    _selectedTab = tabIndex;
                }

                if (!PlatformManager.TryGetConfigType(platform, out Type configType) || null == configType)
                {
                    continue;
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
                    // We only increment the tab index if the editor has been
                    // added to the tabs.
                    tabIndex++;
                    continue;
                }
#endif

                _platformConfigEditors.Add(editor);

                tabContents.Add(new GUIContent($" {editor.GetLabelText()}", editor.GetPlatformIconTexture()));

            }

            // If (for some reason) a default platform was not selected, then
            // default to the first tab being selected
            if (_selectedTab == -1)
            {
                _selectedTab = 0;
            }

            _platformTabs = tabContents.ToArray();
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

        private async void Save()
        {
            // Save the product config editor
            await _productConfigEditor.Save();

            // Save each of the platform config editors.
            foreach (IConfigEditor editor in _platformConfigEditors)
            {
                await editor.Save();
            }
        }
    }
}

#endif