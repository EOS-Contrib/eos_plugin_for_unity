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

namespace PlayEveryWare.EpicOnlineServices
{
    /// <summary>
    /// This class is defined here as a stand-in for UnityEngine.Application,
    /// so that files compiled outside of the Unity Editor that reference that
    /// class can still be compiled properly.
    /// </summary>
    internal static class Application
    {
        /// <summary>
        /// This is the path of the streaming assets directory relative to the
        /// output directory of this class library.
        /// </summary>
        public static readonly string streamingAssetsPath = @"..\..\..\..\Assets\StreamingAssets\";

        /// <summary>
        /// For the purposes of this class library, this field member must be
        /// present - but it's value is not utilized, so it is being left empty.
        /// </summary>
        public static readonly string temporaryCachePath = string.Empty;
    }
}
