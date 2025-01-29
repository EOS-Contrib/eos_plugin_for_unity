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

namespace PlayEveryWare.Common
{
    using EpicOnlineServices.Utility;
    using System;
    using System.Collections.Generic;
    
    /// <summary>
    /// Used to associate a name with a particular type of value.
    /// </summary>
    public class Named<T> : IEquatable<Named<T>>, IComparable<Named<T>>, IEquatable<T> where T : IEquatable<T>
    {
        public event EventHandler<ValueChangedEventArgs<string>> NameChanged;

        /// <summary>
        /// The name of the value (typically used for things like UI labels)
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// The value itself.
        /// </summary>
        public T Value;

        /// <summary>
        /// Create a new Named with the given name and value.
        /// </summary>
        /// <param name="name">The name for the value.</param>
        /// <param name="value">The value.</param>
        public Named(string name, T value)
        {
            Name = name;
            Value = value;
        }

        public bool TrySetName(string newName, bool notify = true)
        {
            // If the name hasn't changed, do nothing and return true.
            if (string.Equals(Name, newName))
            {
                return true;
            }

            string oldName = Name;
            Name = newName;
            if (notify)
            {
                NameChanged?.Invoke(this, new ValueChangedEventArgs<string>(oldName, newName));
            }

            // If the name has not been set to the new name then it was 
            // not able to be set.
            return string.Equals(Name, newName);
        }

        /// <summary>
        /// Compares one named value to another. This is purely for sorting
        /// purposes, so only the Name component of the value is compared.
        /// </summary>
        /// <param name="other">
        /// The other named object (must be of same type).
        /// </param>
        /// <returns>
        /// Result of a comparison between the names of each Named object.
        /// </returns>
        public int CompareTo(Named<T> other)
        {
            return string.CompareOrdinal(Name, other.Name);
        }

        /// <summary>
        /// Compare the equality of one Named instance a value of the same type.
        /// Only the value of the Named instance is considered when evaluating
        /// equivalency.
        /// </summary>
        /// <param name="other">
        /// The value to evaluate equality for.
        /// </param>
        /// <returns>
        /// True if the given value is equivalent to the named value instance.
        /// </returns>
        public bool Equals(T other)
        {
            if (Value == null && other == null)
                return true;

            return Value != null && Value.Equals(other);
        }

        public override bool Equals(object obj)
        {
            return obj is Named<T> other && Equals(other);
        }

        public bool Equals(Named<T> other)
        {
            if (other is null)
            {
                return false;
            }

            return ReferenceEquals(this, other) || EqualityComparer<T>.Default.Equals(Value, other.Value);
        }

        public override int GetHashCode()
        {
            return HashUtility.Combine(Value);
        }

        public override string ToString()
        {
            return $"\"{Name}\" : ({Value})";
        }
    }
}