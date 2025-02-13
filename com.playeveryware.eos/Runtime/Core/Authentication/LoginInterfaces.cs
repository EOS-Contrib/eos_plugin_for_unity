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

#if !EOS_DISABLE

namespace PlayEveryWare.EpicOnlineServices
{
    using System;

    /// <summary>
    /// Indicates a login interface or set of interfaces
    /// </summary>
    [Flags]
    public enum LoginInterfaces : int
    {
        /// <summary>
        /// Indicates that no form of authentication has taken place.
        /// </summary>
        None = 0,

        /// <summary>
        /// Indicates that authentication has been accomplished with the
        /// Auth Interface.
        /// </summary>
        Auth = 1 << 0,

        /// <summary>
        /// Indicates that the authentication has been accomplished with the
        /// Connect Interface.
        /// </summary>
        Connect = 1 << 1,
    }

    public static class LoginInterfacesExtensions
    {
        /// <summary>
        /// Checks if the current instance contains the specified LoginInterfaces value.
        /// </summary>
        /// <param name="current">The current LoginInterfaces instance.</param>
        /// <param name="value">The LoginInterfaces value to check.</param>
        /// <returns>True if the current instance contains the specified value; otherwise, false.</returns>
        public static bool Contains(this LoginInterfaces current, LoginInterfaces value)
        {
            // Special handling for None
            if (value == LoginInterfaces.None)
            {
                return current == LoginInterfaces.None;
            }

            return (current & value) == value;
        }
    }

}

#endif