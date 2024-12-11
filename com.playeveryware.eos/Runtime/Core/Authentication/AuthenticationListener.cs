/*
 * Copyright (c) 2024 PlayEveryWare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if !EOS_DISABLE

namespace PlayEveryWare.EpicOnlineServices
{
    using Epic.OnlineServices;
    using Epic.OnlineServices.Auth;
    using System;
    using UnityEngine;

    /// <summary>
    /// Used to listen for authentication events from EOSManager.
    /// </summary>
    public class AuthenticationListener: IAuthInterfaceEventListener, IConnectInterfaceEventListener, IDisposable
    {
        

        /// <summary>
        /// Used to describe the event arguments for when authentication state
        /// changes.
        /// </summary>
        public class AuthenticationChangedEventArgs : EventArgs
        {
            /// <summary>
            /// Whether the action was to authenticate or to de-authenticate
            /// </summary>
            public readonly bool Authenticated;

            /// <summary>
            /// The interface with which the authentication operation is taking
            /// place.
            /// </summary>
            public readonly LoginInterface Interface;

            /// <summary>
            /// Create a new AuthenticationChangedEventArgs object with the
            /// given parameters.
            /// </summary>
            /// <param name="authenticated">
            /// Whether the authentication or de-authentication is taking place.
            /// </param>
            /// <param name="loginInterface">
            /// The interface with which the authentication operation is taking
            /// place.
            /// </param>
            public AuthenticationChangedEventArgs(bool authenticated, LoginInterface loginInterface)
            {
                Authenticated = authenticated;
                Interface = loginInterface;
            }
        }

        /// <summary>
        /// Event that triggers when the state of authentication has changed.
        /// </summary>
        public event EventHandler<AuthenticationChangedEventArgs> AuthenticationChanged;

        #region Singleton Pattern Implementation

        /// <summary>
        /// Lazy instance for singleton allows for thread-safe interactions with
        /// the AuthenticationListener
        /// </summary>
        private static readonly Lazy<AuthenticationListener> s_LazyInstance = new(() => new AuthenticationListener());

        /// <summary>
        /// Accessor for the instance.
        /// </summary>
        public static AuthenticationListener Instance
        {
            get
            {
                return s_LazyInstance.Value;
            }
        }

        /// <summary>
        /// Private constructor ensures enforcement of the singleton pattern.
        /// </summary>
        private AuthenticationListener() 
        {
            EOSManager.Instance.AddAuthLoginListener(this);
            EOSManager.Instance.AddAuthLogoutListener(this);
            EOSManager.Instance.AddConnectLoginListener(this);
        }
        
        #endregion

        /// <summary>
        /// Stores the current status of authentication - where flags for each
        /// type of login interface are set if user is authenticated with the
        /// indicated interface, and not set if the user is not authenticated
        /// with the indicated interface.
        /// </summary>
        private LoginInterface _authenticationStatus = LoginInterface.None;

        /// <summary>
        /// Indicates whether the authentication listener has received an
        /// AuthenticationChanged event that indicates a user has been logged
        /// in.
        /// </summary>
        public LoginInterface InterfacesAuthenticatedWith
        {
            get
            {
                return _authenticationStatus;
            }
        }

        /// <summary>
        /// Based on the given result code, determine if the authentication
        /// state change event needs to be triggered or not, and if so trigger
        /// it with the appropriate event args.
        /// </summary>
        /// <param name="attemptedState"></param>
        /// <param name="attemptResult"></param>
        /// <param name="changeType">The type of authentication change.</param>
        private void TriggerAuthenticationChangedEvent(bool attemptedState, Result attemptResult, LoginInterface changeType)
        {
            // If the attempt to change the state of authentication did not 
            // succeed, then log a warning and stop.
            if (attemptResult != Result.Success)
            {
                Debug.LogWarning($"Authentication change attempt failed with following result code: {attemptResult}");
                return;
            }

            // Modify the authentication status bit-flag
            if (attemptedState)
            {
                // Bitwise OR to add the type authenticated
                _authenticationStatus |= (LoginInterface)changeType;
            }
            else
            {
                // Bitwise AND to remove the type being de-authenticated
                _authenticationStatus &= ~(LoginInterface)changeType;
            }

            // Trigger the event indicating that the state of authentication for 
            // the user has changed.
            AuthenticationChanged?.Invoke(this, new AuthenticationChangedEventArgs(attemptedState, changeType));
        }

        /// <summary>
        /// Called by EOSManager when auth login has taken place.
        /// </summary>
        /// <param name="loginCallbackInfo">
        /// Callback parameters for the authentication operation.
        /// </param>
        public void OnAuthLogin(LoginCallbackInfo loginCallbackInfo)
        {
            TriggerAuthenticationChangedEvent(true, loginCallbackInfo.ResultCode, LoginInterface.Auth);
        }

        /// <summary>
        /// Called by EOSManager when auth logout has taken place.
        /// </summary>
        /// <param name="logoutCallbackInfo">
        /// Callback parameters for the logout operation.
        /// </param>
        public void OnAuthLogout(LogoutCallbackInfo logoutCallbackInfo)
        {
            TriggerAuthenticationChangedEvent(false, logoutCallbackInfo.ResultCode, LoginInterface.Auth);
        }

        /// <summary>
        /// Called by EOSManager when connect login has taken place.
        /// </summary>
        /// <param name="loginCallbackInfo">
        /// Callback parameters for the connect login operation.
        /// </param>
        public void OnConnectLogin(Epic.OnlineServices.Connect.LoginCallbackInfo loginCallbackInfo)
        {
            TriggerAuthenticationChangedEvent(true, loginCallbackInfo.ResultCode, LoginInterface.Connect);
        }

        /// <summary>
        /// Dispose of the AuthenticationListener, removing it as a listener
        /// from the various ways that EOSManager keeps track of it.
        /// </summary>
        public void Dispose()
        {
            EOSManager.Instance.RemoveAuthLoginListener(this);
            EOSManager.Instance.RemoveAuthLogoutListener(this);
            EOSManager.Instance.RemoveConnectLoginListener(this);
        }
    }
}

#endif