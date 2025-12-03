using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using PlayEveryWare.EpicOnlineServices;

namespace PlayEveryWare.EpicOnlineServices
{
    /// <summary>
    /// Holds override values for Deployment and Sandbox IDs.
    /// </summary>
    public static class EOSOverrideState
    {
        public static string DeploymentIdOverride { get; set; }
        public static string SandboxIdOverride { get; set; }
    }
}
/// <summary>
/// Applies CLI overrides for Epic Online Services before any scene loads.
/// </summary>
public static class EOSCommandLineOverrides
{
    private const string DeploymentKey = "-epicdeploymentid";
    private const string SandboxKey = "-epicsandboxid";

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    private static void ApplyCliOverrides()
    {
        try
        {
            Debug.Log($"[{nameof(EOSCommandLineOverrides)}::{nameof(ApplyCliOverrides)}] Initializing CLI overrides…");

            // Get command line arguments
            string[] args = Environment.GetCommandLineArgs();
            string deploymentArg = null;
            string sandboxArg = null;

            // Parse CLI arguments for deployment and sandbox IDs
            foreach (var arg in args)
            {
                if (arg.StartsWith(DeploymentKey, StringComparison.OrdinalIgnoreCase))
                    deploymentArg = arg.Substring(DeploymentKey.Length);

                if (arg.StartsWith(SandboxKey, StringComparison.OrdinalIgnoreCase))
                    sandboxArg = arg.Substring(SandboxKey.Length);
            }

            // If no overrides found, exit early
            if (deploymentArg == null && sandboxArg == null)
            {
                Debug.Log($"[{nameof(EOSCommandLineOverrides)}] No CLI overrides found. argsCount={args.Length}");
                return;
            }

            Debug.Log($"[EOSBuildBootstrapper] CLI Override detected: DEPLOYMENT={deploymentArg}, SANDBOX={sandboxArg}");

            // --------------------------------------------------------------------
            // Save values globally for later access
            // --------------------------------------------------------------------
            EOSOverrideState.DeploymentIdOverride = deploymentArg;
            EOSOverrideState.SandboxIdOverride = sandboxArg;

            // --------------------------------------------------------------------
            // Regenerate a clean platform config
            // --------------------------------------------------------------------
            var cfg = PlatformManager.GetPlatformConfig();

            // --------------------------------------------------------------------
            // Apply overrides to PlatformConfig
            // --------------------------------------------------------------------
            if (!string.IsNullOrEmpty(sandboxArg))
            {
                cfg.deployment.SandboxId = SandboxId.FromString(sandboxArg);
                Debug.Log($"[{nameof(EOSCommandLineOverrides)}] Sandbox configured: {sandboxArg}");
            }

            if (!string.IsNullOrEmpty(deploymentArg) && Guid.TryParse(deploymentArg, out Guid dep))
            {
                cfg.deployment.DeploymentId = dep;
                Debug.Log($"[{nameof(EOSCommandLineOverrides)}] Deployment configured: {dep}");
            }

            Debug.Log($"[{nameof(EOSCommandLineOverrides)}] Overrides applied successfully.");
        }
        catch (Exception ex)
        {
            Debug.LogError($"[{nameof(EOSCommandLineOverrides)}] ERROR applying overrides: {ex}");
        }
    }
}

