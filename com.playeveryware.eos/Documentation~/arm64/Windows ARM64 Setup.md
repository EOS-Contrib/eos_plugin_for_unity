# <div align="center">Windows ARM64 Setup</div>
---

## Overview

Windows ARM64 support for Standalone builds is available in **Unity 6.0 and later**. Unity does not provide a built-in scripting define to distinguish ARM64 from x64 on the Standalone Windows target, so the EOS Plugin manages this automatically through a build hook.

> [!IMPORTANT]
> This feature requires Unity 6.0.0 or newer. ARM64 builds are not supported on earlier Unity versions.

---

## Prerequisites

* Unity 6.0.0 or newer
* Windows ARM64 build support module installed
* The [EOS C# SDK](https://dev.epicgames.com/docs/game-services/eos-sdk/eos-sdk-csharp-overview) with the manual modifications described below

---

## How the Plugin Handles ARM64 Builds

The plugin registers a build handler via `BuildPlayerWindow.RegisterBuildPlayerHandler`. When you start a Windows Standalone build targeting ARM64 architecture, the handler automatically injects the `EOS_PLATFORM_WINDOWS_ARM64` scripting define into `BuildPlayerOptions.extraScriptingDefines` for that build, without persisting it to your project settings.

This means the first ARM64 build works without any manual steps — no need to run a build twice or set the define manually beforehand.

When switching **back from ARM64 to x64**, if the `EOS_PLATFORM_WINDOWS_ARM64` define is still present in your project settings from a prior session, the plugin removes it and stops the build with an error. A second build is required in this direction to recompile without the symbol. See [Known Limitations](#known-limitations).

### Manual Define Controls

For cases where you need to manage the define manually, two menu items are available under **EOS Plugin > Advanced > Windows ARM64**:

* **Enable scripting define**: Adds `EOS_PLATFORM_WINDOWS_ARM64` to your Standalone scripting defines.
* **Disable scripting define**: Removes `EOS_PLATFORM_WINDOWS_ARM64` from your Standalone scripting defines.

---

## Builds Using `BuildPipeline.BuildPlayer()` Directly

> [!IMPORTANT]
> The build hook that injects `EOS_PLATFORM_WINDOWS_ARM64` is only triggered when the build is started via `BuildPlayerWindow.BuildPlayer()` the Build button in the Editor or any script that calls it explicitly.
>
> It is **not** triggered when calling `BuildPipeline.BuildPlayer()` directly (for example, in custom CI scripts or menu-item build methods). Builds that use this path will require a second build unless the define is added manually.

If your CI or build scripts call `BuildPipeline.BuildPlayer()` directly, add the define to `BuildPlayerOptions.extraScriptingDefines` yourself:

```csharp
var options = new BuildPlayerOptions
{
    scenes = EditorBuildSettings.scenes.Select(s => s.path).ToArray(),
    locationPathName = outputPath,
    target = BuildTarget.StandaloneWindows64,
    options = BuildOptions.None,
    extraScriptingDefines = new[] { "EOS_PLATFORM_WINDOWS_ARM64" }
};
BuildPipeline.BuildPlayer(options);
```

---

## Configuring the EOS SDK Binaries in Unity

The ARM64 and x64 EOS SDK binaries must be configured so that each is only included in the correct build.

1. In the **Project** window, select `Assets/Plugins/Windows/ARM64/EOSSDK-Win64-Shippingarm64.dll`.
2. In the **Inspector**, under **Platform settings**, enable **Standalone** and restrict it to Windows ARM64 only uncheck the x86 and x64 entries.
3. Repeat for the x64 binary (`Assets/Plugins/Windows/x64/EOSSDK-Win64-Shipping.dll`), ensuring it is excluded from ARM64 builds.

---

## Required Modifications to the C# EOS SDK

The EOS C# SDK requires manual changes to support Windows ARM64 in Unity. Apply the following to the C# SDK source before building.

### `SDK/Source/Core/Common.cs`

Add a check for `EOS_PLATFORM_WINDOWS_ARM64` above the existing Windows platform defines, and add a corresponding entry for the ARM64 library name (as per step 3 of the [EOS C# SDK setup guide](https://dev.epicgames.com/docs/game-services/eos-sdk/eos-sdk-csharp-overview)). The ARM64 library name is `EOSSDK-Win64-Shippingarm64` (omit the `.dll` extension for Unity projects).

```csharp
#if EOS_PLATFORM_WINDOWS_ARM64
    // Set externally by the Windows ARM64 build pipeline.
    // Unity does not provide a built-in Windows ARM64 scripting symbol.
#elif UNITY_EDITOR_WIN
    #define EOS_PLATFORM_WINDOWS_64
#elif UNITY_STANDALONE_WIN
    #if UNITY_64
        #define EOS_PLATFORM_WINDOWS_64
    #else
        #define EOS_PLATFORM_WINDOWS_32
    #endif
#endif
```

### Generated Files in `SDK/Source/Generated/Windows`

Add `EOS_PLATFORM_WINDOWS_ARM64` to the `#if` condition that gates each file in `SDK/Source/Generated/Windows` and its subfolders. For example, `SDK/Source/Generated/Windows/Common.cs`:

```csharp
#if EOS_PLATFORM_WINDOWS_32 || EOS_PLATFORM_WINDOWS_64 || EOS_PLATFORM_WINDOWS_ARM64
using System;

namespace Epic.OnlineServices
{
    public sealed partial class Common
    {
        /// <summary>
        /// Steam online platform
        /// </summary>
        public const int WINDOWS_STEAM_OPT = 4000;
    }
}
#endif
```

> [!NOTE]
> These files are auto-generated. Re-applying this change may be necessary after updating the C# SDK.

---

## Known Limitations

* **Switching ARM64 > x64:** If `EOS_PLATFORM_WINDOWS_ARM64` is still set in your project, the plugin removes it and stops the build. A second build is required to recompile without the symbol.
* **`BuildPipeline.BuildPlayer()` users:** The automatic define injection does not apply. Add the define to `extraScriptingDefines` manually (see above).
* **Steam:** Steam features are unavailable on Windows ARM64. No Steam binaries ship for this architecture.
