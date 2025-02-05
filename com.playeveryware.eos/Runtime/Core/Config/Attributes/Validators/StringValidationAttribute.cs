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
    using UnityEngine;

    [AttributeUsage(AttributeTargets.Field)]

    public abstract class StringValidationAttribute : ValidationAttribute
    {

        public override bool Validate(object value, out string errorMessage)
        {
            // Check to make sure the value is of the correct type.
            // This should really only fail if the editor for the config was improperly implemented,
            // so it's appropriate to log an error here.
            if (value is not string strValue)
            {
                errorMessage = "Invalid value type provided for field.";
                Debug.LogError(
                    $"There was likely a problem with how the currently open configuration window was implemented. Please reach out to the support team for assistance.");
                return false;
            }

            return ValidateStringField(strValue, out errorMessage);
        }

        public abstract bool ValidateStringField(string toValidate, out string errorMessage);
    }
}