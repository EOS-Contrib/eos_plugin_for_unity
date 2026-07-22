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

namespace PlayEveryWare.EpicOnlineServices.Samples
{
    using System;
    using UnityEngine;

    public static class UIActions
    {
        public static Action OnCollapseFriendsTab;
        public static Action OnExpandFriendsTab;

        public static void InvokeOnCollapseFriendsTab()
        {
            SafeInvokeAction(ref OnCollapseFriendsTab);
        }

        public static void InvokeOnExpandFriendsTab()
        {
            SafeInvokeAction(ref OnExpandFriendsTab);
        }

        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        private static void Initialize()
        {
            ClearActions();
        }

        private static void ClearActions()
        {
            OnCollapseFriendsTab = null;
            OnExpandFriendsTab = null;
        }

        private static void SafeInvokeAction(ref Action action)
        {
            if (action == null)
            {
                return;
            }

            foreach (Action handler in action.GetInvocationList())
            {
                if (handler.Target is UnityEngine.Object target && target == null)
                {
                    action -= handler;
                    continue;
                }

                handler.Invoke();
            }
        }
    }
}
