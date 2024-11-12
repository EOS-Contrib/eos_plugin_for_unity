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
        /// Compares two versions for equality, ensuring that if one version has
        /// a component defined and the other one does not, they are considered
        /// not equal.
        /// </summary>
        /// <param name="v1">The first version.</param>
        /// <param name="v2">The second version.</param>
        /// <returns>
        /// True if both versions are considered equal, considering only defined
        /// components. If one version has a component defined and the other
        /// does not, they are considered not equal.
        /// </returns>
        public static bool AreVersionsEqual(Version v1, Version v2)
        {
            // Compare major versions (must both be defined and equal)
            if (v1.Major != v2.Major) return false;

            // Compare minor versions (must both be defined and equal)
            if (v1.Minor != v2.Minor) return false;

            // Compare build versions
            if ((v1.Build != -1 && v2.Build == -1) || (v1.Build == -1 && v2.Build != -1))
            {
                return false; // One is defined, the other is not
            }
            if (v1.Build != -1 && v2.Build != -1 && v1.Build != v2.Build)
            {
                return false; // Both are defined, but not equal
            }

            // Compare revision versions
            if ((v1.Revision != -1 && v2.Revision == -1) || (v1.Revision == -1 && v2.Revision != -1))
            {
                return false; // One is defined, the other is not
            }
            if (v1.Revision != -1 && v2.Revision != -1 && v1.Revision != v2.Revision)
            {
                return false; // Both are defined, but not equal
            }

            // If all specified parts are equal, return true
            return true;
        }
    }
}