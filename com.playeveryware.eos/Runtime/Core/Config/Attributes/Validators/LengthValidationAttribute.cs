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
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property, AllowMultiple = true)]
    public class LengthValidationAttribute : StringValidationAttribute
    {
        private readonly int? _minLength;
        private readonly int? _maxLength;

        // Constructor for specifying both min and max length
        public LengthValidationAttribute(int minLength, int maxLength)
        {
            _minLength = minLength;
            _maxLength = maxLength;
        }

        // Constructor for specifying only min length
        public LengthValidationAttribute(int minLength)
        {
            _minLength = minLength;
            _maxLength = null;
        }

        // Constructor for specifying only max length
        public LengthValidationAttribute(int maxLength, bool isMaxOnly)
        {
            _minLength = null;
            _maxLength = maxLength;
        }

        public override bool ValidateStringField(string value, out string errorMessage)
        {
            if (value.Length < _minLength)
            {
                errorMessage = $"String must be at least {_minLength.Value} characters long.";
                return false;
            }

            if (value.Length > _maxLength)
            {
                errorMessage = $"String must be no more than {_maxLength.Value} characters long.";
                return false;
            }

            // There was no error - so no need for an error message.
            errorMessage = string.Empty;
            return true;
        }
    }
}

#endif