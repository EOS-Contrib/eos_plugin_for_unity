# <div align="center">Installing New versions of EOS</div>
---

This document covers three different scenarios where one might need to upgrade the EOS SDK:
 * When the plugin is installed into a Unity Project.
 * When one has a clone of this repo.
 * When one is a maintainer.

Upgrading the SDK is generally an easy process, mostly involving copying around files.

Sometimes it may be preferable to update the SDK for bug fixes while preserving backwards compatibility. To do this, follow the steps in [Update EOS SDK Binary Files Only](#update-eos-sdk-binary-files-only).

## Upgrading the EOS SDK in an installed plugin

### Prerequisites
* Have access to the EOS Developer Portal.
* Downloads of the EOS SDK one wishes to upgrade to.

If one has the plugin installed via 'From disk', one may proceed to the general
instructions in [Upgrading the EOS SDK in a clone of this repository](#upgrading-the-eos-sdk-in-a-clone-of-this-repository).

If one has the plugin installed on disk as a UPM package: 
1. Uninstall the plugin.
    * Optionally, move the package to somewhere else on disk.
2. Decompress the package.
3. Add it back in from the package manager  using 'Add Package From disk...'.
4. Continue with the steps at [Upgrading the EOS SDK in a clone of this repository](#upgrading-the-eos-sdk-in-a-clone-of-this-repository).



## Upgrading the EOS SDK in a clone of this repository
These steps are for users that are planning on creating a new version of the plugin
from a clone of the repository.

Download EOS Dlls and install them in the proper location:

`${PROJECT_ROOT}/Assets/Plugins/${PLATFORM}/${ARCH}/`

Where:

`PROJECT_ROOT` is the location of the cloned project on Disk.

`PLATFORM` is the Unity Platform (Windows, Linux, macOS, Consoles).

`ARCH` is the architecture (x64, x86, ETC.).

Additionally, the C# will have to be changed. Currently they are modified
to support dynamic loading of the DLLs in the Editor to ensure seamless 
usage of the EOS SDK in the Unity editor. Sometimes (due to a change in how
the EOS SDK initializes) native code will need to be updated and recompiled before a
new plugin can be generated.


## Upgrading the EOS SDK as a maintainer of the repo
These steps are for upgrading the EOS SDK as a maintainer of the repo.
There is a tool that one can use install new versions of the SDK, located under

`EOS Plugin -> Install EOS Zip`

It requires a JSON description file to direct it where to put the files in the zip,
and a zip file that contains the SDK. The latest version of the SDK can be downloaded from
the EOS Developer Portal.

After being installed via the Tool, update the repo [readme](/com.playeveryware.eos/README.md) to ensure it lists the correct version
and that any links on the readme are up to date.

## Read the Release Notes While Upgrading
Epic publishes Release Notes with each new version of the EOS SDK.
The Release Notes can be read either on the Epic Games Developer Portal,
or in the [documentation's EOS SDK Release Notes page](https://dev.epicgames.com/docs/epic-online-services/release-notes).

Epic will call out *Breaking changes* in the Release Notes.
These may indicate actions that are required for your upgrade, such as new API requirements,
changed function names, or permission changes that require configuration in the portal.

Some changes only affect the `C` level, and are already handled in other released SDK folders.
For example, the `1.16.4` integration had a breaking change where `eos_[PLATFORM]_types.h` moved
definitions to `eos_common.h`. In the `C#` SDK for this release, Epic's generated C# files
already moved and handled the change.

## Update EOS SDK Package Information
The plugin's `package.json`, and a few other places, mention the version of the EOS SDK being utilized.
When upgrading, the implementor should update all relevant `package.json` files to mention the newer version of the SDK.
Inside the `CHANGELOG.md` file, it should be noted when new versions of the SDK are implemented.
Other documentation mentioning the version should also be updated. At time of writing, that is the [supported platforms documentation](supported_platforms.md).

## Update EOS SDK Binary Files Only
This method involves upgrading only the binary files, but not the header files. This allows taking advantage of bug fixes in more recent SDK releases, or to swap the SDK for a version built against a different platform SDK version. This method does not require a rebuild of the engine or plugins since the headers have not changed. The SDK should continue to operate in the same manner as it did in the SDK version it was written against (the SDK version of the headers).

Do the following depending on the platform you are updating:

* For Windows: replace the .dll files at `Assets\Plugins\Windows\x64` and `Assets\Plugins\Windows\x86`.
* For Mac: replace the .dylib files at `Assets\Plugins\macOS`.
* For Android: replace the eos-sdk.aar file at `etc\PlatformSpecificAssets\EOS\Android\static-stdc++\aar`.
* For IOS: ignore the `Assets\Plugins\iOS\EOSSDK.framework\Headers` folder and just update the other contents of the `Assets\Plugins\iOS\EOSSDK.framework` folder.
* For Linux: replace the .so files at `Assets\Plugins\Linux`.