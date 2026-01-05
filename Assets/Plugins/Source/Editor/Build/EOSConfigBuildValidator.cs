using PlayEveryWare.EpicOnlineServices;
using System;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;

/// <summary>
/// Validates platform-specific identifier and EOS-related configuration prior to starting a build.
/// Implements <see cref="UnityEditor.Build.IPreprocessBuildWithReport"/> to run checks early in the
/// build pipeline, preventing invalid builds and surfacing actionable diagnostics.
/// </summary>
/// <remarks>
/// <para>
/// Responsibilities:<br/>
/// - Verify that target platform has a valid deploy and sandbox ID.<br/>
/// - Accumulate detected issues in <see cref="EOSConfigBuildValidator.ErrorCode"/> (via <c>errorCode</c>) and construct a
///   human-readable summary using <c>BuildErrorMessage</c>.
/// </para>
/// <para>
/// Behavior:<br/>
/// - Runs during <see cref="OnPreprocessBuild(UnityEditor.Build.Reporting.BuildReport)"/>.<br/>
/// - If any errors are detected, throws <see cref="UnityEditor.Build.BuildFailedException"/> with a
///   detailed message to halt the build.<br/>
/// - If no errors are present, the build continues normally.
/// </para>
/// <para>
/// Notes:<br/>
/// - Extend platform checks as needed (Android/iOS/Windows/macOS/PS4/PS5/XBox One/XBox Series)
/// and align validation rules with your EOS Config.<br/>
/// - Keep validations deterministic and side-effect free; this class should only read settings
///   and report issues.
/// </para>
/// </remarks>
public class EOSConfigBuildValidator : IPreprocessBuildWithReport
{
    /// <summary>
    /// Error flags for the pre-build validation system.
    /// Flags can be combined (bitwise) to represent multiple failures.
    /// </summary>
    [Flags]
    private enum ErrorCode : uint
    {
        None = 0x0,
        InvalidSandboxId = 0x1,
        InvalidDeploymentId = 0x2,
    }

    /// <summary>
    /// Maps Unity BuildTarget to PlatformManager.Platform
    /// </summary>
    private static readonly IDictionary<BuildTarget, PlatformManager.Platform> TargetToPlatformsMap =
        new Dictionary<BuildTarget, PlatformManager.Platform>()
        {
                { BuildTarget.Android,             PlatformManager.Platform.Android     },
                { BuildTarget.GameCoreXboxOne,     PlatformManager.Platform.XboxOne     },
                { BuildTarget.GameCoreXboxSeries,  PlatformManager.Platform.XboxSeriesX },
                { BuildTarget.iOS,                 PlatformManager.Platform.iOS         },
                { BuildTarget.StandaloneLinux64,   PlatformManager.Platform.Linux       },
                { BuildTarget.PS4,                 PlatformManager.Platform.PS4         },
                { BuildTarget.PS5,                 PlatformManager.Platform.PS5         },
                { BuildTarget.Switch,              PlatformManager.Platform.Switch      },
                { BuildTarget.StandaloneOSX,       PlatformManager.Platform.macOS       },
                { BuildTarget.StandaloneWindows,   PlatformManager.Platform.Windows     },
                { BuildTarget.StandaloneWindows64, PlatformManager.Platform.Windows     },
        };

    /// <summary>
    /// Returns the relative callback order for callbacks. Callbacks with lower values
    /// are called before ones with higher values.
    /// </summary>
    public int callbackOrder => 0;

    /// <summary>
    /// Accumulates error flags detected during pre-build validation.
    /// Uses bitwise combinations of <see cref="ErrorCode"/> to represent multiple issues in a single value.
    /// </summary>
    private ErrorCode errorCode = ErrorCode.None;

    /// <summary>
    /// Produces a human-readable message based on active error flags in <see cref = "errorCode" />.
    /// Aggregates explanations and corrective actions for each flagged issue.
    /// </summary>
    /// <returns></returns>
    private string BuildErrorMessage()
    {
        bool isInvalidSandbox = errorCode.HasFlag(ErrorCode.InvalidSandboxId);
        bool isInvalidDeployment = errorCode.HasFlag(ErrorCode.InvalidDeploymentId);
        if (isInvalidSandbox && isInvalidDeployment)
        {
            return "Both sandbox and deployment ID missing. Please configure a valid Deployment and Sandbox IDs in EOS Plugin->EOS Configuration.";
        }

        if (isInvalidDeployment)
        {
            return "Deployment ID is missing. Please configure a valid Deployment ID in EOS Plugin->EOS Configuration.";
        }

        return "Sandbox ID missing. Please configure a valid Sandbox ID in EOS Plugin->EOS Configuration.";
    }

    /// <summary>
    /// Executes pre-build validations for platform-specific identifiers.
    /// Verifies that target platform IDs are present and correctly formatted.
    /// If inconsistencies or missing values are detected, the method
    /// sets the corresponding flags in <see cref="errorCode"/> to block the build and notify the user.
    /// </summary>
    /// <param name="report">The BuildReport API gives you information about the Unity build process.</param>
    /// <exception cref="BuildFailedException">An exception class that represents a failed build.</exception>
    public void OnPreprocessBuild(BuildReport report)
    {
        errorCode = ErrorCode.None;
        BuildTarget target = report.summary.platform;
        if (!PlatformManager.TryGetConfigFilePath(target, out string configFilePath))
        {
            throw new BuildFailedException($"{target} not supported.");
        }

        if (!File.Exists(configFilePath))
        {
            throw new BuildFailedException($"{target} config file not found, Set EOS configuration.");
        }

        if (!TargetToPlatformsMap.TryGetValue(target, out PlatformManager.Platform supportedPlatform))
        {
            throw new BuildFailedException($"{target} not found on PlatformManager platforms.");
        }

        if (!PlatformManager.TryGetConfig(supportedPlatform, out PlatformConfig platformConfig))
        {
            throw new BuildFailedException("Platform manager was unable to obtain the platform configuration.");
        }

        if (String.IsNullOrEmpty(platformConfig.deployment.SandboxId.Value))
        {
            errorCode = errorCode | ErrorCode.InvalidSandboxId;
        }
        if (platformConfig.deployment.DeploymentId.Equals(Guid.Empty))
        {
            errorCode = errorCode | ErrorCode.InvalidDeploymentId;
        }

        if (errorCode != ErrorCode.None)
        {
            throw new BuildFailedException(BuildErrorMessage());
        }
    }
}