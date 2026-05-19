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
    using Common;

    [AttributeUsage(AttributeTargets.Field)]
    public class ClientCredentialsFieldValidatorAttribute : FieldValidatorAttribute
    {
        public const string NoValidClientCredentialsMessage =
                "At least one complete Client Credential must be configured.";

        public override bool FieldValueIsValid(object toValidate, out string configurationProblemMessage)
        {
            if (toValidate is not SetOfNamed<EOSClientCredentials> clients)
            {
                configurationProblemMessage = $"Field value type is not {nameof(SetOfNamed<EOSClientCredentials>)}.";
                return false;
            }

            foreach (var namedClient in clients)
            {
                if (namedClient.Value != null && namedClient.Value.IsComplete)
                {
                    configurationProblemMessage = string.Empty;
                    return true;
                }
            }

            configurationProblemMessage = NoValidClientCredentialsMessage;
            return false;
        }
    }
}