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
    using Common;
    using Epic.OnlineServices;
    using Epic.OnlineServices.Auth;
    using System;
    using UnityEngine;

    [Obsolete("AuthenticationListener has been deprecated. Please use AuthenticationEventDispatcher instead.", true)]
    public class AuthenticationListener
    {

    }

    /// <summary>
    /// Used to listen for authentication events from EOSManager.
    /// </summary>
    public sealed class AuthenticationEventDispatcher: Disposable, IAuthInterfaceEventListener, IConnectInterfaceEventListener
    {
        /// <summary>
        /// Event that triggers when the state of authentication has changed.
        /// </summary>
        public event EventHandler<AuthenticationChangedEventArgs> AuthenticationChanged;

        #region Singleton Pattern Implementation

        /// <summary>
        /// Lazy instance for singleton allows for thread-safe interactions with
        /// the AuthenticationListener
        /// </summary>
        private static readonly Lazy<AuthenticationEventDispatcher> s_LazyInstance = new(() => new AuthenticationEventDispatcher());

        /// <summary>
        /// Accessor for the instance.
        /// </summary>
        public static AuthenticationEventDispatcher Instance
        {
            get
            {
                return s_LazyInstance.Value;
            }
        }

        /// <summary>
        /// Private constructor ensures enforcement of the singleton pattern.
        /// </summary>
        private AuthenticationEventDispatcher()
        {
            EOSManager.Instance.AddAuthLoginListener(this);
            EOSManager.Instance.AddAuthLogoutListener(this);

            // TODO: Investigate why there is no AddConnectLogoutListener.
            EOSManager.Instance.AddConnectLoginListener(this);
        }
        
        #endregion

        /// <summary>
        /// Indicates whether the authentication listener has received an
        /// AuthenticationChanged event that indicates a user has been logged
        /// in.
        /// </summary>
        public LoginInterfaces AuthenticatedInterfaces { get; private set; } = LoginInterfaces.None;

        /// <summary>
        /// Based on the given result code, determine if the authentication
        /// state change event needs to be triggered or not, and if so trigger
        /// it with the appropriate event args.
        /// </summary>
        /// <param name="attemptedState"></param>
        /// <param name="attemptResult"></param>
        /// <param name="changeType">The type of authentication change.</param>
        private void TriggerAuthenticationChangedEvent(bool attemptedState, Result attemptResult, LoginInterfaces changeType)
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
                AuthenticatedInterfaces |= changeType;
            }
            else
            {
                // Bitwise AND to remove the type being de-authenticated
                AuthenticatedInterfaces &= ~changeType;
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
            TriggerAuthenticationChangedEvent(true, loginCallbackInfo.ResultCode, LoginInterfaces.Auth);
        }

        /// <summary>
        /// Called by EOSManager when auth logout has taken place.
        /// </summary>
        /// <param name="logoutCallbackInfo">
        /// Callback parameters for the logout operation.
        /// </param>
        public void OnAuthLogout(LogoutCallbackInfo logoutCallbackInfo)
        {
            TriggerAuthenticationChangedEvent(false, logoutCallbackInfo.ResultCode, LoginInterfaces.Auth);
        }

        /// <summary>
        /// Called by EOSManager when connect login has taken place.
        /// </summary>
        /// <param name="loginCallbackInfo">
        /// Callback parameters for the connect login operation.
        /// </param>
        public void OnConnectLogin(Epic.OnlineServices.Connect.LoginCallbackInfo loginCallbackInfo)
        {
            TriggerAuthenticationChangedEvent(true, loginCallbackInfo.ResultCode, LoginInterfaces.Connect);
        }

        protected override void DisposeManagedResources()
        {
            // Remove from list of authentication listeners
            EOSManager.Instance.RemoveAuthLoginListener(this);
            EOSManager.Instance.RemoveAuthLogoutListener(this);

            // TODO: Investigate why there is no RemoveConnectLogoutListener.
            EOSManager.Instance.RemoveConnectLoginListener(this);
        }

        protected override void DisposeUnmanagedResources()
        {
            // Empty definition is here to satisfy requirements of Disposable
            // abstract class - this class does not need to dispose of any
            // unmanaged resources.
        }
    }
}

#endif