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

namespace PlayEveryWare.Common
{
    using System;

    public static class VersionUtility
    {
        /// <summary>
        /// Compares two versions for equality, ignoring any undefined
        /// components of the version.
        /// </summary>
        /// <param name="v1">The first version.</param>
        /// <param name="v2">The second version.</param>
        /// <returns>
        /// True if, when ignoring any undefined components of both versions,
        /// the versions are considered to be equal. The default equality
        /// comparator for Version will return false if either version has a
        /// component that is undefined. This utility function circumvents that
        /// limitation.
        /// </returns>
        public static bool AreVersionsEqual(Version v1, Version v2)
        {
            // Compare major versions
            if (v1.Major != v2.Major) return false;

            // Compare minor versions
            if (v1.Minor != v2.Minor) return false;

            // Compare build versions if both are specified
            if (v1.Build != -1 && v2.Build != -1 && v1.Build != v2.Build) return false;

            // Compare revision versions if both are specified
            if (v1.Revision != -1 && v2.Revision != -1 && v1.Revision != v2.Revision) return false;

            // If all specified parts are equal, return true
            return true;
        }
    }
}