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

#if !EOS_DISABLE

namespace PlayEveryWare.EpicOnlineServices.Utility
{
    using System;

    public static class LoggingUtils
    {
        public static bool ShouldRedactValues
        {
            get
            {
#if DEBUG
                return false;
#else
#if !EXTERNAL_TO_UNITY
                return !UnityEngine.Debug.isDebugBuild;
#else
                return true;
#endif
#endif
            }
        }

        public static string Redact<T>(T value, int preserveChars = 3)
            where T : class
        {
            return Redact(value?.ToString(), preserveChars);
        }

        public static string Redact(string value, int preserveChars = 3)
        {
            if (!ShouldRedactValues || string.IsNullOrEmpty(value))
            {
                return value;
            }

            int charsToKeep = value.Length >= preserveChars * 3
                ? preserveChars
                : 1;

            if (value.Length <= charsToKeep * 2)
            {
                return value;
            }

            string start = value.Substring(0, charsToKeep);
            string end = value.Substring(value.Length - charsToKeep, charsToKeep);
            return $"{start}...{end}";
        }
    }
}

#endif
