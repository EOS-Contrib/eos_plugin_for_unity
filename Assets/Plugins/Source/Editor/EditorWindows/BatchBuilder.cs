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

using UnityEngine;
using UnityEditor;

// make lines a little shorter
using UPMUtility = PlayEveryWare.EpicOnlineServices.Editor.Utility.UnityPackageCreationUtility;

using System;

namespace PlayEveryWare.EpicOnlineServices.Editor.Windows
{
    using Config;
    using EpicOnlineServices.Utility;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;
    using System.Threading;
    using System.Threading.Tasks;
    using UnityEditorInternal;
    using Utility;
    using Config = EpicOnlineServices.Config;

    [Serializable]
    public class BatchCreatePackageWindow : EOSEditorWindow
    {
        private const string DefaultPackageDescription = "etc/PackageConfigurations/eos_package_description.json";

        [RetainPreference("CleanBeforeCreate")]
        private bool _cleanBeforeCreate = true;

        [RetainPreference("IgnoreGitWhenCleaning")]
        private bool _ignoreGitWhenCleaning = true;

        private PackagingConfig _packagingConfig;

        private CancellationTokenSource _createPackageCancellationTokenSource;

        private bool _operationInProgress;

        public BatchCreatePackageWindow() : base("Create Multiple Packages") { }

        string DescriptionDirPath;
        List<string> files = new DirectoryInfo("etc/PackageConfigurations").GetFiles().Select(o => o.Name).ToList();
        List<Description> descriptions = new List<Description>();
        #region Progress Bar Stuff

        private float _actualProgress;
        private float _displayedProgress;
        private string _progressText;
        private object _progressLock = new object();
        private Thread _progressUpdateThread;

        #endregion

        [MenuItem("EOS Plugin/Advanced/Create Multiple Packages")]
        public static void ShowWindow()
        {
            GetWindow<BatchCreatePackageWindow>();
        }

        protected override async Task AsyncSetup()
        {
            _packagingConfig = await Config.GetAsync<PackagingConfig>();

            if (string.IsNullOrEmpty(_packagingConfig.pathToJSONPackageDescription))
            {
                _packagingConfig.pathToJSONPackageDescription =
                    Path.Combine(FileSystemUtility.GetProjectPath(), DefaultPackageDescription);
                await _packagingConfig.WriteAsync();
            }

            DescriptionDirPath = Path.Combine(FileSystemUtility.GetProjectPath(), "etc/PackageConfigurations/update_description/");
            foreach (string file in files)
            {
                Description description = new Description();
                description.file = file;
                description.target = file.Substring(32);
                description.platform = description.target.Split('_')[0];
                description.version = description.target.Split('_')[1].Replace(".json","");
                description.isExporting = true;
                descriptions.Add(description);
            }

            await base.AsyncSetup();
            SetAutoResize(false);
        }

        protected static bool SelectOutputDirectory(ref string path)
        {
            string selectedPath = EditorUtility.OpenFolderPanel(
                "Pick output directory",
                Path.GetDirectoryName(FileSystemUtility.GetProjectPath()),
                "");

            if (string.IsNullOrEmpty(selectedPath) || !Directory.Exists(selectedPath))
            {
                return false;
            }

            path = selectedPath;
            return true;
        }

        protected override void Teardown()
        {
            base.Teardown();

            if (_createPackageCancellationTokenSource != null)
            {
                _createPackageCancellationTokenSource.Cancel();
                _createPackageCancellationTokenSource.Dispose();
                _createPackageCancellationTokenSource = null;
            }
        }

        class Description
        {
            public string file;
            public string target;
            public string platform;
            public string version;
            public bool isExporting;
        }

        protected override void RenderWindow()
        {
            if (_operationInProgress)
            {
                GUI.enabled = false;
            }

            GUILayout.BeginHorizontal();
            GUILayout.Space(10f);
            var outputPath = _packagingConfig.pathToOutput;
            GUIEditorUtility.AssigningTextField("Output Path", ref outputPath, 100f);
            if (GUILayout.Button("Select", GUILayout.MaxWidth(100)))
            {
                if (SelectOutputDirectory(ref outputPath))
                {
                    _packagingConfig.pathToOutput = outputPath;
                    _packagingConfig.Write();
                }
            }

            GUILayout.EndHorizontal();

            foreach (Description description in descriptions)
            {
                GUIEditorUtility.AssigningBoolField(description.target, ref description.isExporting);
            }

            GUILayout.Space(10f);

            GUILayout.BeginHorizontal();
            GUILayout.FlexibleSpace();


            if (GUILayout.Button($"Export All Upms", GUILayout.MaxWidth(200)))
            {
                // if the output path is empty or doesn't exist, prompt for the user to select one
                if (string.IsNullOrEmpty(outputPath) || !Directory.Exists(outputPath))
                {
                    if (SelectOutputDirectory(ref outputPath))
                    {
                        _packagingConfig.pathToOutput = outputPath;
                        _packagingConfig.Write();
                    }
                    else
                    {
                        EditorUtility.DisplayDialog("Package Export Canceled",
                            "An output directory was not selected, so package export has been canceled.",
                            "ok");
                        return;
                    }
                }
                foreach (Description desc in descriptions)
                {
                    if (desc.isExporting)
                    {
                        string individualOutputPath = $"{outputPath}/{desc.platform}/{desc.version}";
                        if (!Directory.Exists(individualOutputPath))
                        {
                            Directory.CreateDirectory(individualOutputPath);
                        }
                        StartCreatePackageAsync(individualOutputPath, DescriptionDirPath + desc.file);
                    }
                }
            }

            GUILayout.FlexibleSpace();
            GUILayout.Space(20f);
            GUILayout.EndHorizontal();

        }

        protected void RenderAdvanced()
        {
            GUILayout.BeginVertical();
            GUILayout.Space(5f);
            var jsonPackageFile = _packagingConfig.pathToJSONPackageDescription;

            GUILayout.BeginHorizontal();
            GUIEditorUtility.AssigningTextField("JSON Description Path", ref jsonPackageFile, 150f);
            if (GUILayout.Button("Select", GUILayout.MaxWidth(100)))
            {
                var jsonFile = EditorUtility.OpenFilePanel(
                    "Pick JSON Package Description",
                    Path.Combine(FileSystemUtility.GetProjectPath(), Path.GetDirectoryName(DefaultPackageDescription)),
                    "json");

                if (!string.IsNullOrWhiteSpace(jsonFile))
                {
                    jsonPackageFile = jsonFile;
                }
            }
            GUILayout.EndHorizontal();

            if (jsonPackageFile != _packagingConfig.pathToJSONPackageDescription)
            {
                _packagingConfig.pathToJSONPackageDescription = jsonPackageFile;
                _packagingConfig.Write(true);
            }

            GUIEditorUtility.AssigningBoolField("Clean target directory", ref _cleanBeforeCreate, 150f,
                "Cleans the output target directory before creating the package.");

            GUIEditorUtility.AssigningBoolField("Don't clean .git directory", ref _ignoreGitWhenCleaning, 150f,
                "When cleaning the output target directory, don't delete any .git files.");

            GUILayout.EndVertical();
            GUILayout.Space(10f);
        }

        private async void StartCreatePackageAsync(string output, string description)
        {
            await UPMUtility.CreateUPMTarball(output, description, null, CancellationToken.None);
        }
    }
}
