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
    using System.IO;
    using System.Reflection;
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

#if UNITY_6000_0_OR_NEWER
        /// <summary>
        /// True when the active Standalone build is configured to produce ARM64 binaries.
        /// </summary>
        /// <remarks>
        /// Unity does not expose a stable public API for the Windows architecture sub-option
        /// across all 6.x minor versions. We try (in order):
        ///   1. <see cref="PlayerSettings.GetArchitecture(NamedBuildTarget)"/> — Unity 6.x.
        ///   2. <see cref="PlayerSettings.GetArchitecture(BuildTargetGroup)"/> — fallback older overload.
        ///   3. Presence of <see cref="Symbol"/> in the active scripting defines — manual escape hatch.
        /// Any thrown exception is swallowed so an API change in a Unity update does not break builds.
        /// </remarks>
        public static bool IsArm64Active()
        {
            // Try newer NamedBuildTarget overload via reflection so that absence on a given
            // Unity build does not produce a hard compile failure.
            try
            {
                MethodInfo method = typeof(PlayerSettings).GetMethod(
                    "GetArchitecture",
                    new[] { typeof(NamedBuildTarget) });
                if (method != null)
                {
                    object value = method.Invoke(null, new object[] { NamedBuildTarget.Standalone });
                    if (value is int arch)
                    {
                        // Windows architecture sub-option: 0 = x86_64, 1 = ARM64 in Unity 6.
                        return arch == 1;
                    }
                }
            }
            catch { /* fall through */ }

            // Older overload accepting BuildTargetGroup is available across all Unity 6.x.
            try
            {
                int arch = PlayerSettings.GetArchitecture(BuildTargetGroup.Standalone);
                if (arch != 0)
                {
                    return true;
                }
            }
            catch { /* fall through */ }

            // Manual escape hatch: respect the scripting define if the user set it explicitly.
            try
            {
                NamedBuildTarget named = NamedBuildTarget.FromBuildTargetGroup(BuildTargetGroup.Standalone);
                string defines = PlayerSettings.GetScriptingDefineSymbols(named);
                return !string.IsNullOrEmpty(defines)
                    && System.Array.IndexOf(defines.Split(';'), Symbol) >= 0;
            }
            catch
            {
                return false;
            }
        }
#else
        public static bool IsArm64Active() => false;
#endif

#if UNITY_6000_0_OR_NEWER
        [MenuItem("EOS Plugin/Advanced/Windows ARM64/Enable scripting define")]
        private static void EnableArm64Define()
        {
            ScriptingDefineUtility.AddDefine(BuildTarget.StandaloneWindows64, Symbol);
            UnityEngine.Debug.Log($"Scripting define '{Symbol}' enabled for Standalone Windows. Build for ARM64 architecture to produce ARM64 binaries.");
        }

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
        public WindowsBuilder64() : base("Plugins/Windows/x64", BuildTarget.StandaloneWindows64)
        {
            AddProjectFileToBinaryMapping(
                "DynamicLibraryLoaderHelper/DynamicLibraryLoaderHelper.sln",
                "DynamicLibraryLoaderHelper-x64.dll",
                "GfxPluginNativeRender-x64.dll");
        }

        public override string GetPlatformString()
        {
            return "x64";
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
            // Ensure ARM64 define is cleared on x64 builds so Common.cs picks the x64 SDK name.
            ScriptingDefineUtility.RemoveDefine(BuildTarget.StandaloneWindows64, WindowsArm64Define.Symbol);
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
        public WindowsBuilderArm64() : base("Plugins/Windows/ARM64", BuildTarget.StandaloneWindows64)
        {
            AddProjectFileToBinaryMapping(
                "DynamicLibraryLoaderHelper/DynamicLibraryLoaderHelper.sln",
                // ARM64 binaries are copied into Plugins/Windows/ARM64, but keep
                // the existing logical DllImport filenames so Unity resolves them
                // by plugin importer CPU metadata.
                "DynamicLibraryLoaderHelper-x64.dll",
                "GfxPluginNativeRender-x64.dll");
        }

        public override string GetPlatformString()
        {
            return "ARM64";
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
            // Set ARM64 define so platform-specific code can gate unsupported dependencies such as Steam.
            ScriptingDefineUtility.AddDefine(BuildTarget.StandaloneWindows64, WindowsArm64Define.Symbol);
            base.PreBuild(report);
        }
    }
#endif

    /// <summary>
    /// WindowsBuilder for 32-bit deployment.
    /// </summary>
    public class WindowsBuilder32 : WindowsBuilder
    {
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
            return "Win32";
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