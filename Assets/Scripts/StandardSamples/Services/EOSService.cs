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
 *
 */

namespace PlayEveryWare.EpicOnlineServices
{
    using Epic.OnlineServices;
    using System;
    using System.Threading.Tasks;

    /// <summary>
    /// Contains implementation of common functionality between different
    /// EOS Service managers (Currently AchievementsService and StatsService).
    /// </summary>
    public abstract class EOSService : AuthenticationEventObserver
    {
        /// <summary>
        /// Event that triggers when changes have been made that may require a
        /// user interface update.
        /// </summary>
        public event Action Updated;

        /// <summary>
        /// Base constructor for services.
        /// </summary>
        /// <param name="requiredLoginInterfaces">
        /// Indicates the login interface required for the service to operate.
        /// By default, implementing services require either auth or connect.
        /// </param>
        protected EOSService(LoginInterfaces acceptedLoginInterfaces = LoginInterfaces.Connect | LoginInterfaces.Auth) : base(acceptedLoginInterfaces)
        {
            _ = RefreshAsync();
        }

        protected override void DisposeUnmanagedResources()
        {
            Reset();

            // Call the base implementation.
            base.DisposeUnmanagedResources();
        }

        protected override void DisposeManagedResources()
        {
            // Default behavior is to take no action. Empty implementation is 
            // added here because currently there are no implementing classes
            // that need to define bespoke behavior.
        }

        /// <summary>
        /// This method clears all state for the service. If overridden in an
        /// implementing class, this base implementation should be called at the
        /// end of that implementation. Implementing classes probably _should_
        /// override this function.
        /// </summary>
        protected virtual void Reset()
        {
            // Set the updated event to null, which removes any subscribers.
            Updated = null;
        }

        /// <summary>
        /// Destructor is defined to catch cases where Dispose was not properly
        /// called before the service ceases to exist.
        /// </summary>
        ~EOSService()
        {
            Dispose();
        }

        /// <summary>
        /// Try to get the product user id from the connect interface.
        /// </summary>
        /// <param name="productUserId">
        /// ProductUserId for the currently logged in user.
        /// </param>
        /// <returns>
        /// True if the ProductUserId was successfully retrieved and is valid,
        /// false otherwise.
        /// </returns>
        protected bool TryGetProductUserId(out ProductUserId productUserId)
        {
            productUserId = EOSManager.Instance.GetProductUserId();
            if (null != productUserId && productUserId.IsValid())
            {
                return true;
            }

            UnityEngine.Debug.LogWarning($"There was a problem getting the product user id of the currently logged-in user.");
            return false;

        }

        /// <summary>
        /// Implement this method to perform tasks when a user authenticates.
        /// By default, there is no action taken.
        /// </summary>
        protected override void OnLoggedIn() { }

        /// <summary>
        /// If there are tasks that need to be done when logged out, consider
        /// overriding the Reset() function as that is where such things should
        /// be done.
        /// </summary>
        protected override void OnLoggedOut()
        {
            Reset();
        }

        /// <summary>
        /// Refreshes the service manager
        /// </summary>
        /// <returns></returns>
        public async Task RefreshAsync()
        {
            // If authentication is needed then do not refresh.
            if (NeedsAuthentication)
            {
                return;
            }

            await InternalRefreshAsync();
        }
        
        /// <summary>
        /// Implementation of this function should refresh the locally cached
        /// data managed by the service manager in question.
        /// </summary>
        protected abstract Task InternalRefreshAsync();

        /// <summary>
        /// Trigger the notification callbacks that have been registered,
        /// letting any consumers know that the data base been updated.
        /// </summary>
        protected void NotifyUpdated()
        {
            Updated?.Invoke();
        }
    }
}