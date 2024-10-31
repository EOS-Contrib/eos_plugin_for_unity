<a href="/com.playeveryware.eos/README.md"><img src="/com.playeveryware.eos/Documentation~/images/PlayEveryWareLogo.gif" alt="README.md" width="5%"/></a>

# <div align="center">Connect Login with GoogleID using Credential Manager</div>
---

## Overview

This document describes how to enable Connect Login with GoogleID in conjunction with Epic Online Services Plugin for Unity.  
We followed [Authenticate users with Sign in with Google](https://developer.android.com/identity/sign-in/credential-manager-siwg) and implemented some scripts for the users to easily achieve the feature.

## Setup

### Plugins

The feature is included in our samples, so no external plugin is needed.

### Build Settings

To enable the feature, follow the build configuration step below. 
Add the following in the dependencies of the project's main gradle file. (The default is `mainTemplate.gradle`) 
```
        implementation 'androidx.credentials:credentials-play-services-auth:1.3.0'
        implementation 'androidx.credentials:credentials:1.3.0'
        implementation 'com.google.android.libraries.identity.googleid:googleid:1.1.1'
```

### Configurations

Configure `GoogleLoginClientID` and optionally `GoogleLoginNonce` in `AndroidConfig`.   

The `GoogleLoginClientID` could be found from `https://console.cloud.google.com/apis/`.  
Follow [Set up your Google APIs console project](https://developer.android.com/identity/sign-in/credential-manager-siwg#set-google) to set up your project.  

> [!IMPORTANT]
> Make sure `GoogleLoginClientID` uses the ClientID of the Web Application type.

## Scripts

### Sample Sign In Scripts

Here are some sample scripts tested in PlayEveryWare EOS Unity Plugin

Starting with the java calls of google APIs

```java
package com.playeveryware.googlelogin;

import com.google.android.libraries.identity.googleid.GetSignInWithGoogleOption;
import com.google.android.libraries.identity.googleid.GoogleIdTokenCredential;
import com.google.android.libraries.identity.googleid.GoogleIdTokenParsingException;

import androidx.credentials.GetCredentialRequest;
import androidx.credentials.GetCredentialResponse;
import androidx.credentials.exceptions.GetCredentialException;
import androidx.credentials.Credential;
import androidx.credentials.CustomCredential;
import androidx.credentials.CredentialManager;
import androidx.credentials.CredentialManagerCallback;

import android.app.Activity;
import android.content.Context;
import android.os.CancellationSignal;
import android.util.Log;

import java.util.concurrent.Executors;


public class login extends Activity
{
    private String name;
    private String token;
    private static login instance;

    public login()
    {
        this.instance = this;
    }
    public static login instance()
    {
        if(instance == null)
        {
            instance = new login();
        }
        return instance;
    }

    public String getResultName()
    {
        return name;
    }
    public String getResultIdToken()
    {
        return token;
    }

    public void SignInWithGoogle(String clientID, String nonce, Context context, CredentialManagerCallback callback)
    {
        GetSignInWithGoogleOption signInWithGoogleOption = new GetSignInWithGoogleOption.Builder(clientID)
                .setNonce(nonce)
                .build();

        GetCredentialRequest request = new GetCredentialRequest.Builder()
                .addCredentialOption(signInWithGoogleOption)
                .build();

        CredentialManager credentialManager = CredentialManager.create(this);
        credentialManager.getCredentialAsync(context,request,new CancellationSignal(),Executors.newSingleThreadExecutor(),callback);
    }

    public void handleFailure(GetCredentialException e)
    {
        Log.e("Unity", "Received an invalid google id token response", e);
    }

    public void handleSignIn(GetCredentialResponse result)
    {
        Credential credential = result.getCredential();

        if (credential instanceof CustomCredential) 
        {
            if (GoogleIdTokenCredential.TYPE_GOOGLE_ID_TOKEN_CREDENTIAL.equals(credential.getType())) 
            {
                try
                {
                    GoogleIdTokenCredential googleIdTokenCredential = GoogleIdTokenCredential.createFrom(credential.getData());
                    name = googleIdTokenCredential.getDisplayName();
                    token = googleIdTokenCredential.getIdToken();
                }
                catch (Exception e)
                {
                    if (e instanceof GoogleIdTokenParsingException)
                    {
                        Log.e("Unity", "Received an invalid google id token response", e);
                    }
                    else
                    {
                        Log.e("Unity", "Some exception", e);
                    }
                }
            }
        }
    }
}
```

Next we create the C# wrappers that calls the java code, and use `AndroidJavaProxy` to implement the login callbacks in C# instead of java

```cs
using UnityEngine;

namespace PlayEveryWare.EpicOnlineServices.Samples
{
    public class SignInWithGoogleManager : MonoBehaviour
    {
        AndroidJavaObject loginObject;

        public void GetGoogleIdToken(System.Action<string, string> callback)
        {
            using AndroidJavaClass unityPlayer = new("com.unity3d.player.UnityPlayer");
            using AndroidJavaObject activity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity");
            if (activity == null)
            {
                Debug.LogError("EOSAndroid: activity context is null!");
                return;
            }

            using AndroidJavaClass loginClass = new AndroidJavaClass("com.playeveryware.googlelogin.login");
            if (loginClass == null)
            {
                Debug.LogError("Java Login Class is null!");
                return;
            }

            loginObject = loginClass.CallStatic<AndroidJavaObject>("instance");

            /// Create the proxy class and pass instances to be used by the callback
            EOSCredentialManagerCallback javaCallback = new EOSCredentialManagerCallback();
            javaCallback.loginObject = loginObject;
            javaCallback.callback = callback;

            AndroidConfig config = AndroidConfig.Get<AndroidConfig>();

            if (string.IsNullOrEmpty(config.GoogleLoginClientID))
            {
                Debug.LogError("Client ID is null, needs to be configured for Google ID connect login");
                return;
            }

            /// SignInWithGoogle(String clientID, String nonce, Context context, CredentialManagerCallback callback)
            loginObject.Call("SignInWithGoogle", config.GoogleLoginClientID, config.GoogleLoginNonce, activity, javaCallback);
        }

        class EOSCredentialManagerCallback : AndroidJavaProxy
        {
            public AndroidJavaObject loginObject;
            public System.Action<string, string> callback;

            /// <summary>
            /// Proxy class to receive Android callbacks in C#
            /// </summary>
            public EOSCredentialManagerCallback() : base("androidx.credentials.CredentialManagerCallback") { }

            /// <summary>
            /// Succeeding Callback of GetCredentialAsync  
            /// GetCredentialAsync is called in com.playeveryware.googlelogin.login.SignInWithGoogle)
            /// </summary>
            /// <param name="credentialResponseResult"></param>
            public void onResult(AndroidJavaObject credentialResponseResult)
            {
                /// Parses the response resilt into google credentials
                loginObject.Call("handleSignIn", credentialResponseResult);

                /// Invoke Connect Login with fetched Google ID
                callback.Invoke(loginObject.Call<string>("getResultIdToken"), loginObject.Call<string>("getResultName"));
            }

            /// <summary>
            /// Failing Callback of GetCredentialAsync  
            /// GetCredentialAsync is called in com.playeveryware.googlelogin.login.SignInWithGoogle)
            /// </summary>
            /// <param name="credentialException"></param>
            public void onError(AndroidJavaObject credentialException)
            {
                loginObject.Call("handleFailure", credentialException);
                callback.Invoke(null, null);
            }
        }
    }
}
```

Finally the method that triggers the login process.
Example from PlayEveryWare EOS Unity Plugin's `UILoginMenu.cs`

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

