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
    using System;

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
        public readonly LoginInterfaces Interface;

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
        public AuthenticationChangedEventArgs(bool authenticated, LoginInterfaces loginInterface)
        {
            Authenticated = authenticated;
            Interface = loginInterface;
        }
    }
}

#endif