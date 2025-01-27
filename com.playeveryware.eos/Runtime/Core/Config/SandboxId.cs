/*
 * Copyright (c) 2021 PlayEveryWare
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

namespace PlayEveryWare.EpicOnlineServices
{
    using Newtonsoft.Json;
    using System.Text.RegularExpressions;
    using System;

    public struct SandboxId : IEquatable<SandboxId>
    {
        private string _value;
        private const string PreProductionEnvironmentRegex = @"^p\-[a-zA-Z\d]{30}$";

        public string Value
        {
            readonly get
            {
                return _value;
            }
            set
            {
                // Skip logic if value has not changed
                if (value == _value || (string.IsNullOrEmpty(_value) && string.IsNullOrEmpty(value)))
                {
                    return;
                }

                // allow null value
                if (value == null)
                {
                    _value = null;
                    return;
                }

                // Store previous value
                string previousValue = _value;

                // Set the value, and test for validity
                _value = value.ToLower();

                // If the newly set value is valid, stop here.
                if (IsValid())
                {
                    return;
                }

                // Otherwise, log a warning and return the value to what it was
                // before an attempt was made to change it.
                // TODO: Figure out how to have this manifest in the editor 
                //       window instead of just the log.
                string logMessage = $"Invalid SandboxId: \"{_value}\".";

                // If the previous value wasn't null, then inform the user that
                // the value is being restored to the previous.
                if (previousValue != null)
                {
                    logMessage += $"Restoring to previous value of \"{previousValue}\".";
                }

                // Actually log the composed message.
                UnityEngine.Debug.LogWarning(logMessage);

                _value = previousValue;
            }
        }

        public bool IsValid()
        {
            return Guid.TryParse(_value, out _) || 
                   Regex.IsMatch(_value, PreProductionEnvironmentRegex);
        }

        public static bool IsNullOrEmpty(string sandboxString)
        {
            return String.IsNullOrEmpty(sandboxString) || 
                   Guid.Empty.ToString("N").Equals(sandboxString);
        }

        public static bool IsNullOrEmpty(SandboxId sandboxId)
        {
            return IsNullOrEmpty(sandboxId._value);
        }

        public static SandboxId FromString(string sandboxString)
        {
            SandboxId retVal = default;
            retVal.Value = sandboxString;
            return retVal;
        }

        /// <summary>
        /// Indicates whether the sandbox id is empty. A sandbox id is empty if
        /// either the underlying value is null or empty, or if the underlying
        /// value is equal to the string value for an empty Guid with the dashes
        /// removed.
        /// TODO: Utilize this property when applying the SandboxId to the EOS
        ///       SDK during initialization.
        /// </summary>
        [JsonIgnore]
        public readonly bool IsEmpty
        {
            get
            {
                return IsNullOrEmpty(this);
            }
        }

        public readonly bool Equals(SandboxId other)
        {
            return _value == other._value;
        }

        public override readonly bool Equals(object obj)
        {
            return obj is SandboxId sandboxId && Equals(sandboxId);
        }

        public override readonly int GetHashCode()
        {
            return (_value?.GetHashCode()) ?? 0;
        }

        public override readonly string ToString()
        {
            return _value;
        }
    }
}