<a href="/com.playeveryware.eos/README.md"><img src="/com.playeveryware.eos/Documentation~/images/PlayEveryWareLogo.gif" alt="README.md" width="5%"/></a>

# <div align="center">Connect Login with GoogleID using Credential Manager</div>
---

## Overview

This document describes how to enable Google ID authentication in conjunction with Epic Online Services Plugin for Unity.  
We followed [Authenticate users with Sign in with Google](https://developer.android.com/identity/sign-in/credential-manager-siwg) and implemented a plugin for the users to easily achieve the feature.

## Setup

### Plugins
The feature is included in our samples, so no external plugin is needed.

### Build Settings
To allow the feature, follow the build configuration step below. 
Add the following in the dependencies of the project's main gradle file. (The default is `mainTemplate.gradle`) 
```
        implementation 'androidx.credentials:credentials-play-services-auth:1.3.0'
        implementation 'androidx.credentials:credentials:1.3.0'
        implementation 'com.google.android.libraries.identity.googleid:googleid:1.1.1'
```

### Configurations
Configurate `GoogleLoginClientID` and optionally`GoogleLoginNonce` in `AndroidConfig`.   
The `GoogleLoginClientID` could be found from `https://console.cloud.google.com/apis/`.  
Follow [Set up your Google APIs console project](https://developer.android.com/identity/sign-in/credential-manager-siwg#set-google) to set up your project.  
> [!IMPORTANT]
> Make sure `GoogleLoginClientID` uses the ClientID of the Web Application type.

## Scripts

### Sample Sign In Scripts

Here is a sample script tested in EOS-Unity-Plugin

```cs
        private void ConnectGoogleId()
        {
            SignInWithGoogleManager signInWithGoogleManager = new();

            signInWithGoogleManager.GetGoogleIdToken((string token, string username) => 
            {
                if (string.IsNullOrEmpty(token))
                {
                    Debug.LogError("Failed to retrieve Google Id Token");
                    return;
                }
                StartConnectLoginWithToken(ExternalCredentialType.GoogleIdToken, token, username);
            });
        }
```

