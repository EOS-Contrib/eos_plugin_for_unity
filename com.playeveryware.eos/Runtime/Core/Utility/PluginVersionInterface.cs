/*
 * Copyright (c) 2025 Apex Systems
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
using Epic.OnlineServices;
using System;

namespace ApexSystems.Utility
{
    public class PluginVersionInterface
    {
        /// <summary>
        /// The company name
        /// </summary>
        public static readonly Utf8String COMPANY_NAME = "Apex Systems, Inc.";
        /// <summary>
        /// The copyright <see cref="Utf8String" />
        /// </summary>
        public static readonly Utf8String COPYRIGHT_STRING = "";
        /// <summary>
        /// The Hotfix version number
        /// </summary>
        public const int HOTFIX = 2;
        /// <summary>
        /// The Major version number
        /// </summary>
        public const int MAJOR = 1;
        /// <summary>
        /// The Minor version number
        /// </summary>
        public const int MINOR = 18;
        /// <summary>
        /// The Patch version number
        /// </summary>
        public const int PATCH = 1;
        /// <summary>
        /// The product identifier
        /// </summary>
        public static readonly Utf8String PRODUCT_IDENTIFIER = "Unity Plugin for Epic Online Services";
        /// <summary>
        /// The product name
        /// </summary>
        public static readonly Utf8String PRODUCT_NAME = "Unity Plugin for Epic Online Services";

        /// <summary>
        /// Get the version of the active Plugin version
        /// MAJOR.MINOR.PATCH.HOTFIX
        public static Utf8String GetVersion()
        {
            var callResult = MAJOR + "." + MINOR + "." + PATCH + "." + HOTFIX;
            return callResult;
        }
    }
}