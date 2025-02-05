/*
 * Copyright (c) 2025 PlayEveryWare
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

#if !EOS_DISABLE

namespace PlayEveryWare.EpicOnlineServices
{
    using System;
    using System.Text.RegularExpressions;

    /// <summary>
    /// ValidationAttribute used to make sure a string field matches a given
    /// regex.
    /// </summary>
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property, AllowMultiple = true)]
    public class RegexValidationAttribute : StringValidationAttribute
    {
        /// <summary>
        /// The pattern that the value must match to pass.
        /// </summary>
        private readonly string _validRegexPattern;

        /// <summary>
        /// The error message to return if the regex pattern match fails.
        /// </summary>
        private readonly string _customErrorMessage;

        /// <summary>
        /// Default value of the error message is set to describe the regex.
        /// </summary>
        /// <param name="validRegexPattern">The pattern to match.</param>
        public RegexValidationAttribute(string validRegexPattern) : 
            this(validRegexPattern, $"Value must satisfy the following regex: {validRegexPattern}")
        { }

        /// <summary>
        /// Determine the parameters for validating a string value with a given
        /// regex pattern.
        /// </summary>
        /// <param name="validRegexPattern">
        /// The pattern to check the value against for validation.
        /// </param>
        /// <param name="customErrorMessage">
        /// The error message should explain in plain language the meaning of
        /// the regex pattern being utilized for validation.
        /// </param>
        public RegexValidationAttribute(string validRegexPattern, string customErrorMessage)
        {
            _validRegexPattern = validRegexPattern;
            _customErrorMessage = customErrorMessage;
        }

        public override bool ValidateStringField(string value, out string errorMessage)
        {
            // Determine whether the value matches the regex pattern.
            if (!Regex.IsMatch(value, _validRegexPattern))
            {
                errorMessage = _customErrorMessage;
                return false;
            }

            // There was no error - so no need for an error message.
            errorMessage = string.Empty;
            return true;
        }
    }
}

#endif