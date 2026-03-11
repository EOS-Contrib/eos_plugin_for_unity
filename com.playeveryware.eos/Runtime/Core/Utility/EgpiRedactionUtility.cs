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
    using System.Text.RegularExpressions;

    public static class EgpiRedactionUtility
    {
        private static readonly Regex s_keyValueRegex = new Regex(
        @"(?<key>Puid|ProductUserId|EpicAccountId|AccountId|LocalUserId|RemoteUserId|PlatformId|PlatformName|DisplayName)\b\s*[:=]\s*(?<quote>[""']?)(?<value>[A-Za-z0-9_\-\.]+)\k<quote>",
        RegexOptions.Compiled);

        public static bool ShouldRedactIdentifiers
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

        public static string RedactIdentifier(string identifier, int preserveChars = 3)
        {
            if (!ShouldRedactIdentifiers || string.IsNullOrEmpty(identifier))
            {
                return identifier;
            }

            int charsToKeep = identifier.Length >= preserveChars * 3
                ? preserveChars
                : 1;

            if (identifier.Length <= charsToKeep * 2)
            {
                return identifier;
            }

            string start = identifier.Substring(0, charsToKeep);
            string end = identifier.Substring(identifier.Length - charsToKeep, charsToKeep);
            return $"{start}...{end}";
        }

        public static string RedactKnownKeyValueIdentifiers(string text, int preserveChars = 3)
        {
            if (!ShouldRedactIdentifiers || string.IsNullOrEmpty(text))
            {
                return text;
            }

            return s_keyValueRegex.Replace(text, match =>
            {
                string key = match.Groups["key"].Value;
                string quote = match.Groups["quote"].Value;
                string value = match.Groups["value"].Value;

                string endQuote = match.Groups["endQuote"].Value;
                if (string.IsNullOrEmpty(endQuote) && !string.IsNullOrEmpty(quote))
                {
                    endQuote = quote;
                }

                return $"{key}{quote}{RedactIdentifier(value, preserveChars)}{endQuote}";
            });
        }
    }
}

#endif
