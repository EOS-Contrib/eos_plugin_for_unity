# <div align="center">Android</div>
---

## Prerequisites

* The standard [Prerequisites](../../README.md#prerequisites) for all platforms.
*  Check out this [Environment Setup](environment_setup_android.md#environment-setup-for-android) doc for a quick setup guide.
*  More detailed instructions can be found in the links below.
    * <a href="https://docs.unity3d.com/Manual/android-sdksetup.html">Android environment setup</a> for Unity.
    * The Android Build Support <a href="https://docs.unity3d.com/hub/manual/AddModules.html">module</a>.

## Importing the Plugin


You can follow the standard [Importing the Plugin](../../README.md#importing-the-plugin-package) process. With a few changes here when [running the samples](#running-the-samples) and [configuring the plugin](#configuring-the-plugin).
> [!WARNING]
> If you choose the tarball method, when downloading the release on mac it may convert the `.tgz` into a `.tar` which is not compatible with unity. Changing the file extension back to a `.tgz` should fix this.

## Samples

You can follow the standard [Samples](../samples.md) process.   
Please note the details in the [Running the samples](#running-the-samples) section when running the samples from a build for Android.  

> [!WARNING] 
> The Social Overlay is not implemented yet. However, the Ecom Overlay is supported on mobile as of EOS Unity Plugin 4.1.0.

## Running the samples

When following the steps to [run a sample](../samples.md#running-the-samples) from a build for Android, follow the Unity doc for <a href="https://docs.unity3d.com/Manual/android-sdksetup.html">Debugging on an Android device</a>, to connect your device to the engine.  
This will allow the smoother ```Build And Run``` option to work instead of just using the ```Build``` button.  

When running on a device you may need to <a href="https://developer.android.com/studio/debug/dev-options#enable">enable developer mode</a> on the device, then <a href="https://developer.android.com/studio/debug/dev-options#Enable-debugging">Enable USB debugging on your device</a>, as well as accepting any popups that appear on the phone during the process.

## Configuring the Plugin

You can follow the standard [Configuring the Plugin](../configure_plugin.md) process.  With the additional steps after saving the Main EOS Config.

## Additional Configuration Steps <a name="configuration-steps" />

1. Select the ```Android``` button.

    ![EOS Config UI](../images/eosconfig_ui_android.gif)

2. Press ```Save All Changes```.

>[!WARNING] 
>This is required, even if you leave every field blank.

3. Update the <a href="https://docs.unity3d.com/Manual/class-PlayerSettingsAndroid.html">Minimum API Level</a> to be at least ```Android 7.0 'Nougat' (API Level 24)```.


# FAQ

See [frequently_asked_questions.md](../frequently_asked_questions.md).
