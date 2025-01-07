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
    using System.Linq;

    public abstract class AuthenticationEventObserver : Disposable
    {
        protected bool NeedsAuthentication { get; private set; }

        protected LoginInterfaces[] ObservedLoginInterfaces { get; }

        protected AuthenticationEventObserver(params LoginInterfaces[] observedLoginInterfaces)
        {
            ObservedLoginInterfaces = observedLoginInterfaces;

            // Subscribe to the authentication change event
            AuthenticationEventDispatcher.Instance.AuthenticationChanged += ObserveAuthenticationChange;
        }

        private void ObserveAuthenticationChange(object sender, AuthenticationChangedEventArgs args)
        {
            // If the the interface for the authentication event does not 
            // match the login interfaces that should be observed, then stop
            // processing the event.
            if (ObservedLoginInterfaces.Contains(args.Interface))
            {
                return;
            }

            if (args.Authenticated)
            {
                NeedsAuthentication = false;
                OnLoggedIn();
            }
            else
            {
                NeedsAuthentication = true;
                OnLoggedOut();
            }
        }

        protected abstract void OnLoggedIn();

        protected abstract void OnLoggedOut();

        protected override void DisposeUnmanagedResources()
        {
            // Unsubscribe from the authentication changed event
            AuthenticationEventDispatcher.Instance.AuthenticationChanged -= ObserveAuthenticationChange;
        }
    }
}

#endif