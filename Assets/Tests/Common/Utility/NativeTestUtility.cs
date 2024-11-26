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

#if EXTERNAL_TO_UNITY
namespace PlayEveryWare.EpicOnlineServices.Tests.Utility
{
    using Epic.OnlineServices.Platform;

    /// <summary>
    /// This class serves as an entry point for native code to test equality
    /// between the config values parsed by managed code and those parsed by
    /// unmanaged code.
    /// </summary>
    public static class NativeTestUtility
    {
        /// <summary>
        /// Returns the WindowsOptions used to create the EOS SDK platform when
        /// it is created from within managed code.
        /// </summary>
        /// <returns>
        /// The result of injesting the configuration values used for EOS
        /// platform creation..</returns>
        public static WindowsOptions GetWindowsOptions()
        {
            return EOSManager.EOSSingleton.GetEOSCreateOptions().options;
        }

        /// <summary>
        /// Returns the InitializeOptions used to initialize the EOS SDK
        /// platform when initialized from within managed code.
        /// </summary>
        /// <returns>
        /// The result of injesting the configuration values used for EOS
        /// platform initialization.</returns>
        public static InitializeOptions GetInitializeOptions()
        {
            return EOSManager.EOSSingleton.GetEOSInitializeOptions().options;
        }
    }
}
#endif