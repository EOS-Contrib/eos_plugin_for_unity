<a href="/com.playeveryware.eos/README.md"><img src="/com.playeveryware.eos/Documentation~/images/PlayEveryWareLogo.gif" alt="Readme" width="5%"/></a>

# Getting Started - Configuring the Plugin

To function, the plugin needs some information from your EOS project. Be sure to read the Epic Documentation on [getting started](https://dev.epicgames.com/docs/epic-account-services/getting-started?sessionInvalidated=true) with Epic Online Services.

## In the Unity editor, Open ```EOS Plugin -> EOS Configuration```.

![EOS Config Menu](/com.playeveryware.eos/Documentation~/images/eos-configuration-menu.png)

![EOS Configuration](/com.playeveryware.eos/Documentation~/images/eos_configuration.png)

## Set product settings from Epic Developer Portal

From the [Developer Portal](https://dev.epicgames.com/portal/), inside your game's `Product Settings` page, copy the configuration values listed below, and paste them into the similarly named fields in the editor tool window pictured above:

> [!NOTE]
> For more detailed information, check out Epic's Documentation on [Creating the Platform Interface](https://dev.epicgames.com/docs/game-services/eos-platform-interface#creating-the-platform-interface).

* ProductName
* [ProductID](https://dev.epicgames.com/docs/services/en-US/Glossary/index.html#D?term=ProductId)
* [SandboxID](https://dev.epicgames.com/docs/services/en-US/Glossary/index.html#D?term=SandboxId)
* [DeploymentID](https://dev.epicgames.com/docs/services/en-US/Glossary/index.html#D?term=DeploymentId)
* [ClientSecret](https://dev.epicgames.com/docs/services/en-US/Glossary/index.html#D?term=ClientSecret)
* [ClientID](https://dev.epicgames.com/docs/services/en-US/Glossary/index.html#D?term=ClientId)

There are two other fields in the configuration editor.

`ProductVersion` is a free-form numeric field, and can use any number.

> [!WARNING]
> The `ProductVersion` field needs to have some value inside it; it cannot be left empty.
	
`EncryptionKey` is used to encrypt uploads to the `Player Data Storage` and `Title Data Storage` EOS features. This value should be consistently used whenever uploading or downloading files from these Data Storages.

> [!NOTE]
> The Encryption Key is generated automatically for you when you create a set of Client Credentials. To view the key, generate a new one - or set it to a specific value, click on the "Key" button next to the client credentials.
> The Encryption Key is used for Player Data Storage and Title Storage.
**Click `Save All Changes`.**

## Add EOS functionality to your game scene

Navigate to `Packages/Epic Online Services for Unity/Runtime` via the `Project` window.

Either:
- Add the `Singletons.prefab`, to each of your game's scenes.
- Attach `EOSManager.cs (Script)` to a Unity object, and it will initialize the plugin with the specified configuration in `OnAwake()` (this is what `Singletons.prefab` does).

> [!NOTE]
> The included [samples](http://github.com/PlayEveryWare/eos_plugin_for_unity/blob/development/com.playeveryware.eos/README.md#samples) already have configuration values set for you to experiment with!