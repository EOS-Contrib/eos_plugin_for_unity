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
 *
 */

namespace PlayEveryWare.Common
{
    using System;

    /// <summary>
    /// Disposable provides a base implementation for the disposal pattern,
    /// requiring deriving classes to define behavior for disposal of both
    /// managed and unmanaged resources.
    /// </summary>
    public abstract class Disposable : IDisposable
    {
        /// <summary>
        /// Guards against circular disposal.
        /// </summary>
        private bool _disposed = false;

        /// <summary>
        /// Satisfies the IDisposable interface.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Accomplishes the disposal of both managed and unmanaged resources
        /// as defined in the deriving class' implementation of the
        /// DisposeManagedResources and DisposeUnmanagedResources functions.
        /// </summary>
        /// <param name="disposing">
        /// Indicates whether disposal is currently taking place. This guards
        /// against circular disposal.
        /// </param>
        protected virtual void Dispose(bool disposing)
        {
            if (_disposed)
            {
                return;
            }

            if (disposing)
            {
                // Call deriving class cleanup logic for managed resources.
                DisposeManagedResources();
            }

            // Call deriving class cleanup logic for unmanaged resources.
            DisposeUnmanagedResources();

            _disposed = true;
        }

        /// <summary>
        /// Deriving classes should implement this function to accomplish thread
        /// safe (managed) resource disposal.
        /// </summary>
        protected abstract void DisposeManagedResources();

        /// <summary>
        /// Deriving classes should implement this function to accomplish the
        /// disposal of unmanaged (unsafe) resources it may have control over
        /// the lifetime of.
        /// </summary>
        protected abstract void DisposeUnmanagedResources();

        /// <summary>
        /// Explicit definition of the destructor is here to ensure that the
        /// disposal pattern is followed properly.
        /// </summary>
        ~Disposable()
        {
            Dispose(false);
        }
    }
}