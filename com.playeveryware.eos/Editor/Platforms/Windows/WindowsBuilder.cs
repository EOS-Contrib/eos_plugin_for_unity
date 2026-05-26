/*
 * Copyright (c) 2026 Epic Games Inc
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

namespace PlayEveryWare.EpicOnlineServices.Editor.Build
{
#if !EOS_DISABLE
    using Epic.OnlineServices.Platform;
    using Extensions;
#endif
    using Config;
    using Config = EpicOnlineServices.Config;
    using System;
    using System.IO;
    using UnityEditor;
    using UnityEditor.Build;
    using UnityEditor.Build.Reporting;
    using UnityEngine;
    using Utility;

    /// <summary>
    /// Scripting define set when targeting the Windows ARM64 architecture sub-option
    /// of <see cref="BuildTarget.StandaloneWindows64"/>. Unity does not provide a built-in
    /// scripting define to distinguish ARM64 from x64 on Standalone Windows, so this
    /// project-local symbol is auto-managed by the Windows builders during preprocess.
    ///
    /// IMPORTANT: scripting defines set during build preprocess only affect the *next*
    /// build's script compilation, not the current one. Switching between x64 and ARM64
    /// for Windows builds requires running the menu item under "EOS Plugin/Advanced/Windows
    /// ARM64..." once before the first build, or running a build twice (the first to set
    /// the define, the second to compile against it).
    /// </summary>
    internal static class WindowsArm64Define
    {
        public const string Symbol = "EOS_PLATFORM_WINDOWS_ARM64";
        internal const string Architecture = "ARM64";

#if UNITY_6000_0_OR_NEWER
        /// <summary>
        /// True when the active Standalone build is configured to produce ARM64 binaries.
        /// Uses <see cref="PlayerSettings.GetArchitecture"/> which returns 1 for ARM64 on
        /// the Standalone Windows target in Unity 6. Falls back to the scripting define as
        /// a manual escape hatch if the API throws.
        /// </summary>
        public static bool IsArm64Active()
        {
            // Guard: only meaningful when actively building for Windows 64-bit.
            // Also protects against Apple Silicon Macs where BuildTargetGroup.Standalone
            // architecture would otherwise reflect the host Mac architecture.
            if (EditorUserBuildSettings.activeBuildTarget != BuildTarget.StandaloneWindows64)
            {
                return false;
            }

            try
            {
                // The "Architecture" dropdown in the Build Settings window is stored as a
                // platform setting, not in PlayerSettings. "ARM64" is set when the user
                // selects "ARM 64-bit"; the field is empty or "x64" for Intel 64-bit.
                string arch = EditorUserBuildSettings.GetPlatformSettings(
                    BuildPipeline.GetBuildTargetName(BuildTarget.StandaloneWindows64),
                    "Architecture");
                if (!string.IsNullOrEmpty(arch))
                {
                    return string.Equals(arch, Architecture, System.StringComparison.OrdinalIgnoreCase);
                }
            }
            catch (Exception ex)
            {
                Debug.LogWarning($"EOS Plugin: Could not read ARM64 architecture setting: {ex.Message}");
            }

            // Manual escape hatch: respect the scripting define if the user set it explicitly.
            try
            {
                NamedBuildTarget named = NamedBuildTarget.FromBuildTargetGroup(BuildTargetGroup.Standalone);
                string defines = PlayerSettings.GetScriptingDefineSymbols(named);
                return !string.IsNullOrEmpty(defines)
                    && Array.IndexOf(defines.Split(';'), Symbol) >= 0;
            }
            catch (Exception ex)
            {
                Debug.LogWarning($"EOS Plugin: Could not read scripting defines: {ex.Message}");
                return false;
            }
        }
#else
        public static bool IsArm64Active() => false;
#endif

        /// <summary>
        /// Returns true if <see cref="Symbol"/> is currently present in the Standalone
        /// scripting defines, regardless of the active build target or architecture setting.
        /// Used to detect define/architecture mismatches before a build compiles scripts.
        /// </summary>
        public static bool IsDefineSet()
        {
            try
            {
                NamedBuildTarget named = NamedBuildTarget.FromBuildTargetGroup(BuildTargetGroup.Standalone);
                string defines = PlayerSettings.GetScriptingDefineSymbols(named);
                return !string.IsNullOrEmpty(defines)
                    && Array.IndexOf(defines.Split(';'), Symbol) >= 0;
            }
            catch (Exception ex)
            {
                Debug.LogWarning($"EOS Plugin: Could not read scripting defines: {ex.Message}");
                return false;
            }
        }

#if UNITY_6000_0_OR_NEWER
        [InitializeOnLoad]
        private static class BuildGuard
        {
            static BuildGuard()
            {
                // Intercept the Build button BEFORE Unity compiles scripts, so any define
                // change takes effect in the same compilation rather than requiring a second build.
                BuildPlayerWindow.RegisterBuildPlayerHandler(OnBuildPlayer);
            }

            private static void OnBuildPlayer(BuildPlayerOptions options)
            {
                if (options.target == BuildTarget.StandaloneWindows64)
                {
                    bool wantsArm64  = IsArm64Active();
                    bool defineIsSet = IsDefineSet();

                    if (wantsArm64 && !defineIsSet)
                    {
                        ScriptingDefineUtility.AddDefine(BuildTarget.StandaloneWindows64, Symbol);
                        EditorUtility.DisplayDialog(
                            "EOS Plugin — Windows ARM64",
                            $"The scripting define '{Symbol}' was missing and has been added.\n\n" +
                            "Scripts are recompiling. Please click Build again once compilation finishes.",
                            "OK");
                        return;
                    }

                    if (!wantsArm64 && defineIsSet)
                    {
                        ScriptingDefineUtility.RemoveDefine(BuildTarget.StandaloneWindows64, Symbol);
                        EditorUtility.DisplayDialog(
                            "EOS Plugin — Windows x64",
                            $"The scripting define '{Symbol}' was left over from a previous ARM64 build and has been removed.\n\n" +
                            "Scripts are recompiling. Please click Build again once compilation finishes.",
                            "OK");
                        return;
                    }
                }

                BuildPlayerWindow.DefaultBuildMethods.BuildPlayer(options);
            }
        }
#endif

#if UNITY_6000_0_OR_NEWER
        /// <summary>
        /// Manually enables the ARM64 scripting define. Use this as a workaround before
        /// the first ARM64 build to ensure scripts compile with ARM64 support on the first
        /// attempt, without needing to build twice.
        /// </summary>
        [MenuItem("EOS Plugin/Advanced/Windows ARM64/Enable scripting define")]
        private static void EnableArm64Define()
        {
            ScriptingDefineUtility.AddDefine(BuildTarget.StandaloneWindows64, Symbol);
            UnityEngine.Debug.Log($"Scripting define '{Symbol}' enabled for Standalone Windows. Build for ARM64 architecture to produce ARM64 binaries.");
        }

        /// <summary>
        /// Manually disables the ARM64 scripting define. Use this as a workaround before
        /// switching back to an x64 build to ensure scripts compile without ARM64 support
        /// on the first attempt, without needing to build twice.
        /// </summary>
        [MenuItem("EOS Plugin/Advanced/Windows ARM64/Disable scripting define")]
        private static void DisableArm64Define()
        {
            ScriptingDefineUtility.RemoveDefine(BuildTarget.StandaloneWindows64, Symbol);
            UnityEngine.Debug.Log($"Scripting define '{Symbol}' disabled for Standalone Windows. Subsequent builds will produce x64 binaries.");
        }
#endif
    }

    /// <summary>
    /// WindowsBuilder for 64-bit (x86_64) deployment.
    /// </summary>
    public class WindowsBuilder64 : WindowsBuilder
    {
        private const string PlatformId = "x64";

        public WindowsBuilder64() : base($"Plugins/Windows/{PlatformId}", BuildTarget.StandaloneWindows64)
        {
            AddProjectFileToBinaryMapping(
                "DynamicLibraryLoaderHelper/DynamicLibraryLoaderHelper.sln",
                $"DynamicLibraryLoaderHelper-{PlatformId}.dll",
                $"GfxPluginNativeRender-{PlatformId}.dll");
        }

        public override string GetPlatformString()
        {
            return PlatformId;
        }

        protected override bool ShouldHandle(BuildReport report)
        {
            // StandaloneWindows64 covers both x64 and ARM64 in Unity 6; only handle x64 here.
            if (!base.ShouldHandle(report))
            {
                return false;
            }
            return !WindowsArm64Define.IsArm64Active();
        }

        public override void PreBuild(BuildReport report)
        {
#if UNITY_6000_0_OR_NEWER
            Debug.Log(WindowsArm64Define.IsArm64Active()
                ? "Targeting Windows ARM64"
                : "Targeting Windows x64 (Intel/AMD)");
#endif
            // Safety net for scripted/CI builds that bypass RegisterBuildPlayerHandler.
            if (WindowsArm64Define.IsDefineSet())
            {
                ScriptingDefineUtility.RemoveDefine(BuildTarget.StandaloneWindows64, WindowsArm64Define.Symbol);
                throw new BuildFailedException(
                    $"Scripting define '{WindowsArm64Define.Symbol}' was active from a previous ARM64 build. " +
                    "It has been removed. Please rebuild to compile with Windows x64 support.");
            }

            base.PreBuild(report);
        }
    }

#if UNITY_6000_0_OR_NEWER
    /// <summary>
    /// WindowsBuilder for ARM64 deployment. Available in Unity 6000.0+ where
    /// Standalone Windows ARM64 is supported as an architecture sub-option of
    /// <see cref="BuildTarget.StandaloneWindows64"/>. Steam features are unavailable
    /// on this architecture (no Steam binaries ship for Windows ARM64).
    /// </summary>
    public class WindowsBuilderArm64 : WindowsBuilder
    {
        private const string PlatformId = WindowsArm64Define.Architecture;

        public WindowsBuilderArm64() : base($"Plugins/Windows/{PlatformId}", BuildTarget.StandaloneWindows64)
        {
            AddProjectFileToBinaryMapping(
                "DynamicLibraryLoaderHelper/DynamicLibraryLoaderHelper.sln",
                $"DynamicLibraryLoaderHelper-{PlatformId}.dll",
                $"GfxPluginNativeRender-{PlatformId}.dll");
        }

        public override string GetPlatformString()
        {
            return PlatformId;
        }

        protected override bool ShouldHandle(BuildReport report)
        {
            if (!base.ShouldHandle(report))
            {
                return false;
            }
            return WindowsArm64Define.IsArm64Active();
        }

        public override void PreBuild(BuildReport report)
        {
            Debug.Log(WindowsArm64Define.IsArm64Active()
                ? "Targeting Windows ARM64"
                : "Targeting Windows x64 (Intel/AMD)");

            // Safety net for scripted/CI builds that bypass RegisterBuildPlayerHandler.
            if (!WindowsArm64Define.IsDefineSet())
            {
                ScriptingDefineUtility.AddDefine(BuildTarget.StandaloneWindows64, WindowsArm64Define.Symbol);
                throw new BuildFailedException(
                    $"Scripting define '{WindowsArm64Define.Symbol}' was not set before this build. " +
                    "It has been added. Please rebuild to compile with Windows ARM64 support.");
            }

            base.PreBuild(report);
        }
    }
#endif

    /// <summary>
    /// WindowsBuilder for 32-bit deployment.
    /// </summary>
    public class WindowsBuilder32 : WindowsBuilder
    {
        private const string PlatformId = "Win32";

        public WindowsBuilder32() : base("Plugins/Windows/x86", BuildTarget.StandaloneWindows)
        {
            // TODO: These libraries do not appear to be building properly - and the process
            //       also appears to delete the x64 libraries. It's possible that both things
            //       are caused by some other process.
            AddProjectFileToBinaryMapping(
                "DynamicLibraryLoaderHelper/DynamicLibraryLoaderHelper.sln",
                "DynamicLibraryLoaderHelper-x86.dll",
                "GfxPluginNativeRender-x86.dll");
        }

        public override string GetPlatformString()
        {
            return PlatformId;
        }
    }

    /// <summary>
    /// Base implementation for a WindowsBuilder. Cannot be instantiated, but is used
    /// as base implementation for both 64 and 32 bit flavors of Windows.
    /// </summary>
    public abstract class WindowsBuilder : PlatformSpecificBuilder
    {
        private const string ProjectPathToEOSBootstrapperTool = "tools/bin/EOSBootstrapperTool.exe";

        protected WindowsBuilder(string nativeBinaryDirectory, params BuildTarget[] buildTargets) : base(nativeBinaryDirectory, buildTargets) {   }

        public override void PostBuild(BuildReport report)
        {
            base.PostBuild(report);

            ConfigureAndInstallBootstrapper(report);
        }

        private static async void ConfigureAndInstallBootstrapper(BuildReport report)
        {
#if EOS_DISABLE
            // If EOS_DISABLE is defined, then the bootstrapper should never be included
            await System.Threading.Tasks.Task.CompletedTask;
            return;
#else
            // Determine if 'DisableOverlay' is set in Platform Flags. If it is, then the EOSBootstrapper.exe is not included in the build,
            // because without needing the overlay, the EOSBootstrapper.exe is not useful to users of the plugin
            PlatformConfig configuration = PlatformManager.GetPlatformConfig();
            PlatformFlags configuredFlags = configuration.platformOptionsFlags.Unwrap();
            if (configuredFlags.HasFlag(PlatformFlags.DisableOverlay))
            {
                Debug.Log($"The '{nameof(PlatformFlags.DisableOverlay)}' flag has been configured, EOSBootstrapper.exe will not be included in this build.");
                return;
            }

            /*
             * NOTE:
             *
             * The following code functions properly, but exposes some poor design with
             * respect to the build process. For starters, in order to determine whether
             * EAC is installed, this function must instantiate a config editor. It would
             * be nice if there was a way to query the config values via a static property
             * like this:
             *
             * if (ToolsConfig.UseEAC) { ... }
             *
             * However, that does not actually answer the question that needs answering
             * in the context of installing the bootstrapper. This answers the question
             * "Is EAC supposed to be configured?" Because if the answer is yes, then
             * the bootstrapper tool needs to use EACLauncher.exe as the target.
             *
             * The reason it is insufficient to answer the question "Is EAC supposed to be
             * configured?" for this purpose is that it doesn't determine if EAC *IS*
             * configured. This current solution relies on the fact that the steps happen
             * to be in-order.
             *
             * Rectifying these design flaws is beyond the scope of what needs to be done
             * right now, but this note remains for the sake of future Build engineers
             * wishing to improve the system, and future developers who may encounter
             * build issues surrounding the Bootstrapper and/or the Easy Anti-Cheat system
             * that are difficult to diagnose.
             */

            // Determine whether to install EAC

            ToolsConfig toolsConfig = await Config.GetAsync<ToolsConfig>();

            string bootstrapperName = null;
            if (toolsConfig != null)
            {
                bootstrapperName = toolsConfig.bootstrapperNameOverride;
            }

            if (string.IsNullOrWhiteSpace(bootstrapperName))
            {
                bootstrapperName = "EOSBootstrapper.exe";
            }

            if (!bootstrapperName.EndsWith(".exe"))
            {
                bootstrapperName += ".exe";
            }

            string pathToEOSBootStrapperTool = Path.Combine(EACUtility.GetPathToEOSBin(), "EOSBootstrapperTool.exe");

            string installDirectory = Path.GetDirectoryName(report.summary.outputPath);

            string bootstrapperTarget = toolsConfig.useEAC ? "EACLauncher.exe" : Path.GetFileName(report.summary.outputPath);

            InstallBootStrapper(bootstrapperTarget, installDirectory, pathToEOSBootStrapperTool,
                bootstrapperName);
#endif
        }

        private static void InstallBootStrapper(string appFilenameExe, string installDirectory,
            string pathToEOSBootStrapperTool, string bootstrapperFileName)
        {
            string installPathForEOSBootStrapper = Path.Combine(installDirectory, bootstrapperFileName);
            string workingDirectory = EACUtility.GetPathToEOSBin();
            string bootStrapperArgs = ""
                                      + $" --output-path \"{installPathForEOSBootStrapper}\""
                                      + $" --app-path \"{appFilenameExe}\"";

            var procInfo = new System.Diagnostics.ProcessStartInfo
            {
                FileName = pathToEOSBootStrapperTool, Arguments = bootStrapperArgs,
                UseShellExecute = false,
                WorkingDirectory = workingDirectory,
                RedirectStandardOutput = true,
                RedirectStandardError = true
            };

            var process = new System.Diagnostics.Process { StartInfo = procInfo };
            process.OutputDataReceived += (sender, e) =>
            {
                if (!string.IsNullOrEmpty(e.Data))
                {
                    Debug.Log($"BootstrapperTool stdout: \"{e.Data}\"");
                }
            };

            process.ErrorDataReceived += (sender, e) =>
            {
                if (!string.IsNullOrEmpty(e.Data))
                {
                    Debug.LogError($"BootstrapperTool stderr: \"{e.Data}\"");
                }
            };

            if (false == process.Start())
            {
                throw new BuildFailedException(
                    $"Failed to run the BootstrapperTool \"{pathToEOSBootStrapperTool}\". Please see log for more details."
                    );
            }

            process.BeginOutputReadLine();
            process.BeginErrorReadLine();
            process.WaitForExit();
            process.Close();
        }
    }
}

#endif