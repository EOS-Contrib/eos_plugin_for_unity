/*
 * Copyright (c) 2026 Epic Games Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

namespace Epic.OnlineServices
{
    public sealed partial class Helper
    {
        /// <summary>
        /// Clears all notification callbacks from the registry. Call this when the EOS Platform
        /// is released so that stale notification IDs don't collide with a re-created Platform.
        /// </summary>
        public static void ClearNotificationCallbacks()
        {
            lock (s_Callbacks)
            {
                var keys = new System.Collections.Generic.List<System.IntPtr>();
                foreach (var pair in s_Callbacks)
                {
                    if (pair.Value.NotificationId.HasValue)
                    {
                        keys.Add(pair.Key);
                    }
                }
                foreach (var key in keys)
                {
                    s_Callbacks.Remove(key);
                }
            }

            lock (s_ClientDatas)
            {
                // Remove client data entries whose callbacks were just cleared.
                // Rebuilding from remaining s_Callbacks keys is safest.
                var validPointers = new System.Collections.Generic.HashSet<System.IntPtr>();
                lock (s_Callbacks)
                {
                    foreach (var key in s_Callbacks.Keys)
                    {
                        validPointers.Add(key);
                    }
                }

                var staleKeys = new System.Collections.Generic.List<System.IntPtr>();
                foreach (var key in s_ClientDatas.Keys)
                {
                    if (!validPointers.Contains(key))
                    {
                        staleKeys.Add(key);
                    }
                }
                foreach (var key in staleKeys)
                {
                    s_ClientDatas.Remove(key);
                }
            }
        }
    }
}
