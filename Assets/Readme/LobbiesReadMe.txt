PlayEveryWare Lobbies Epic Online Services Demo

This demo is intended to show the Lobby function of the EOS Plugin for Unity through a faux game lobby application.
A sample of the Lobby interface can be found in the EOSLobbyManager script.

How to use:
Select a login type
    -Dev Auth uses the Dev Auth tool that comes with the EOS SDK
    -Account Portal uses the popup Epic window
    -Persistent uses the most recent logged in information

If you are getting a error logging in pertaining to the EOS Overlay, this is caused by the overlay not being installed yet. 
To install it create a build of the demo and run the bootstrapper included in the build folder, this will install the overlay.

Users can create a lobby by choosing a BucketID, and choosing any options they want and clicking "Create Lobby" Other users can then search of that lobby.
This is done using the search options on the right side, and any results will be listed below. 
Once in a lobby, if enabled, users will be able to talk over voice chat, individually mute others or themselves, and the lobby owner can promote or kick users.

Note on Anti-Cheat Toggle:
To enable the Anti-Cheat toggle when creating a lobby, the project must have Easy Anti-Cheat (EAC) properly configured according to the official configuration documentation.
If EAC is not correctly set up and initialized, the Anti-Cheat toggle will be disabled in the UI and cannot be selected by users.

Ensure that:
    -The necessary `.key` and `.cer` files are present.
    -The paths are correctly configured in the EOS settings panel.
    -The build is launched using the EAC bootstrapper executable.
