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

    /// <summary>
    /// Applies CLI overrides for Epic Online Services before any scene loads.
    /// </summary>
    public static class EOSCommandLineOverrides
    {
        private const string DeploymentKey = "-epicdeploymentid=";
        private const string SandboxKey = "-epicsandboxid=";
    
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.BeforeSceneLoad)]
        private static void SaveCliOverrides()
        {
            try
            {
                Debug.Log($"[{nameof(EOSCommandLineOverrides)}::{nameof(SaveCliOverrides)}] Initializing CLI overrides");
    
                // Get command line arguments
                string[] args = Environment.GetCommandLineArgs();
                string deploymentArg = null;
                string sandboxArg = null;
    
                // Parse CLI arguments for deployment and sandbox IDs
                foreach (var arg in args)
                {
                    if (arg.StartsWith(DeploymentKey, StringComparison.OrdinalIgnoreCase))
                        deploymentArg = arg.Substring(DeploymentKey.Length);
                    else if (arg.StartsWith(SandboxKey, StringComparison.OrdinalIgnoreCase))
                        sandboxArg = arg.Substring(SandboxKey.Length);
                }
    
                // If no overrides found, exit early
                if (string.IsNullOrEmpty(deploymentArg) && string.IsNullOrEmpty(sandboxArg))
                {
                    Debug.Log($"[{nameof(EOSCommandLineOverrides)}] No CLI overrides found. argsCount={args.Length}");
                    return;
                }
    
                Debug.Log($"[EOSBuildBootstrapper] CLI Override detected: DEPLOYMENT={deploymentArg}, SANDBOX={sandboxArg}");
    
                // Save values globally for later access
                EOSOverrideState.DeploymentIdOverride = deploymentArg;
                EOSOverrideState.SandboxIdOverride = sandboxArg;

                // Apply values 
                PlatformManager.GetPlatformConfig();

            }
            catch (Exception ex)
            {
                Debug.LogError($"[{nameof(EOSCommandLineOverrides)}] ERROR applying overrides: {ex}");
            }
        }
        public static void ApplyOverrides(PlatformConfig config)
        {
            if (config == null)
                return;

            if (!string.IsNullOrEmpty(EOSOverrideState.DeploymentIdOverride))
            { 
                if(Guid.TryParse(EOSOverrideState.DeploymentIdOverride,out var dep))
                {
                    config.deployment.DeploymentId = dep;
                    Debug.Log($"[{nameof(EOSCommandLineOverrides)}] Deployment override applied: {config.deployment.DeploymentId}");
                }
            }

            if (!string.IsNullOrEmpty(EOSOverrideState.SandboxIdOverride))
            {
                var sbx = SandboxId.FromString(EOSOverrideState.SandboxIdOverride);
                config.deployment.SandboxId = sbx;
                Debug.Log($"[{nameof(EOSCommandLineOverrides)}] Sandbox override applied: {config.deployment.SandboxId}");
                
            }

            Debug.Log($"[{nameof(EOSCommandLineOverrides)}] Overrides applied successfully.");
        }
    }
}

