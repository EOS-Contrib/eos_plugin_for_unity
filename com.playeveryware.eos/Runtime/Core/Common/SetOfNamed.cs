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

namespace PlayEveryWare.Common
{
    using Newtonsoft.Json;
    using System.Collections.Generic;
    using System;
    using System.Collections;

    /// <summary>
    /// Class that stores a unique set of values, where each value has a name
    /// associated to it. Items are sorted by the name. No two items of the same
    /// value can be added (regardless of if they have different names). Also,
    /// no two items can have the same name (regardless of if they have
    /// different values).
    /// </summary>
    /// <typeparam name="T">The type being wrapped with a name.</typeparam>
    public class SetOfNamed<T> : List<Named<T>> where T : IEquatable<T>, new()
    {
        /// <summary>
        /// When items without a name are added to the SortedSet, this is the
        /// default name to give to the newly added item. When an item with the
        /// default name already exists in the set, then the default name is
        /// appended with an increasing number until the resulting name does
        /// not exist in the collection.
        /// </summary>
        private readonly string _defaultNamePattern;

        /// <summary>
        /// Used to store a function that can be used to determine if an item
        /// can be removed or not.
        /// </summary>
        private Func<T, bool> _removePredicate = null;

        /// <summary>
        /// Copy of the names of the items that exist within the collection, to
        /// test for uniqueness in a performant manner.
        /// </summary>
        private readonly HashSet<string> _existingNames = new();

        /// <summary>
        /// Creates a new SortedSetOfNamed.
        /// </summary>
        /// <param name="defaultNamePattern">
        /// The default name pattern to apply to items being added to the set.
        /// </param>
        public SetOfNamed(string defaultNamePattern = "NamedItem")
        {
            _defaultNamePattern = defaultNamePattern;
        }

        /// <summary>
        /// Set the predicate used to determine if an item can be removed from
        /// the collection.
        /// </summary>
        /// <param name="removePredicate">
        /// A function that can determine whether or not an item can be removed.
        /// </param>
        public void SetRemovePredicate(Func<T, bool> removePredicate)
        {
            _removePredicate = removePredicate;
        }

        /// <summary>
        /// Returns the name of a new item to be added to the collection.
        /// </summary>
        /// <returns>The name to give to the added set.</returns>
        private string GetNewItemName()
        {
            int left = 1;
            int right = Count;
            string newItemName = _defaultNamePattern;

            while (left <= right)
            {
                int mid = left + (right - left) / 2;
                string testName = $"{_defaultNamePattern} ({mid})";

                if (ContainsName(testName))
                {
                    // If name exists, the smallest valid increment must be greater than mid
                    left = mid + 1;
                }
                else
                {
                    // If name does not exist, it might be the smallest increment
                    newItemName = testName;
                    right = mid - 1;
                }
            }

            return newItemName;
        }

        /// <summary>
        /// Adds an item to the sorted set with a default name.
        /// </summary>
        /// <param name="value">
        /// The value to add to the SortedSet.
        /// </param>
        /// <returns>
        /// True if the item was able to be added, false otherwise.
        /// </returns>
        public bool Add(T value = default)
        {
            // If value is null, then set it to either be default or a new 
            // instance of the value type. This is useful because in the case
            // where T is a ReferenceType, it's "default" is null, and we want
            // to make sure that it is instead a new (empty) instance of the
            // reference type.
            value ??= typeof(T).IsValueType ? default : new T();

            // Determines the name of the new item.
            string newItemName = GetNewItemName();

            // Add to the collection using the base implementation, so long as
            // the name for the new item is not used for another named item 
            // already in the collection, and so long as the value is not the
            // same as the value of another item in the collection.
            if (ContainsName(newItemName) || ContainsValue(value))
            {
                return false;
            }

            Named<T> newItem = new(newItemName, value);
            newItem.NameChanged += OnItemNameChanged;

            // Add name to internal hash set used to ensure uniqueness.
            _existingNames.Add(newItem.Name);

            Add(newItem);
            return true;
        }

        private void OnItemNameChanged(object sender, ValueChangedEventArgs<string> e)
        {
            // If the sender is not of the correct type, or if the value hasn't changed, then take no action.
            if (sender is not Named<T> item || string.Equals(e.OldValue, e.NewValue))
            {
                return;
            }

            // If the collection already contains an item with the name
            if (ContainsName(e.NewValue))
            {
                // Set the name back to the old value. The return value can be 
                // discarded, because it is always true if notify is set to
                // false.
                _ = item.TrySetName(e.OldValue, false);
            }
            else
            {
                // Remove old name from set used to ensure name uniqueness.
                _existingNames.Remove(e.OldValue);
                // Add the new name to the set used to ensure name uniqueness.
                _existingNames.Add(e.NewValue);
            }
        }

        /// <summary>
        /// Determines whether there is an item in the collection with the
        /// indicated name.
        /// </summary>
        /// <param name="name">
        /// The name to check.
        /// </param>
        /// <returns>
        /// True if there is an item in the collection with the given name,
        /// false otherwise.
        /// </returns>
        private bool ContainsName(string name)
        {
            return _existingNames.Contains(name);
        }

        /// <summary>
        /// Determines whether there is an item in the collection with the
        /// indicated value.
        /// </summary>
        /// <param name="item">
        /// The value to check.
        /// </param>
        /// <returns>
        /// True if there is an item in the collection with the indicated value,
        /// false otherwise.
        /// </returns>
        private bool ContainsValue(T item)
        {
            foreach (Named<T> namedItem in this)
            {
                // Keep going if the value is not the same
                if (namedItem.Value.Equals(item))
                {
                    return true;
                }
            }

            return false;
        }

        public new bool Remove(Named<T> item)
        {
            // If a predicate is not defined, or the predicate returns false,
            // then return false and stop.
            if (_removePredicate != null && !_removePredicate(item.Value))
            {
                return false;
            }
            
            // Remove the event subscription
            item.NameChanged -= OnItemNameChanged;

            // Otherwise attempt to remove the item
            base.Remove(item);

            return true;
        }
    }

}