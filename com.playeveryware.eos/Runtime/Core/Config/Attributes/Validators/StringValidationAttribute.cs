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

namespace PlayEveryWare.EpicOnlineServices
{
    using System;

    /// <summary>
    /// Used to describe a validation attribute that operates on a string data
    /// type.
    /// </summary>
    [AttributeUsage(AttributeTargets.Field)]
    public abstract class StringValidationAttribute : FieldValidatorAttribute
    {
        public override bool FieldValueIsValid(
            object value, 
            out string errorMessage)
        {
            // Check to make sure the value is actually a string.
            if (value is string strValue)
            {
                return ValidateStringField(strValue, out errorMessage);
            }

            // If this point is reached, it is because the attribute was
            // improperly applied to a field member that is not of type string,
            // therefore an invalid argument exception should be thrown.
            throw new ArgumentException(
                $"{nameof(StringValidationAttribute)} cannot be " +
                $"applied to a field member whose type is not string.");
        }

        /// <summary>
        /// Implement this function in deriving attributes to accomplish the
        /// validation of the string value.
        /// </summary>
        /// <param name="toValidate">
        /// The string value to be validated.
        /// </param>
        /// <param name="errorMessage">
        /// The error message to use if validation fails.
        /// </param>
        /// <returns>
        /// True if the string value is valid, false otherwise.
        /// </returns>
        public abstract bool ValidateStringField(
            string toValidate, 
            out string errorMessage);
    }
}