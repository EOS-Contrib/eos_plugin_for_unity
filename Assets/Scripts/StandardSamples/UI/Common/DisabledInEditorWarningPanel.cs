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

namespace PlayEveryWare.EpicOnlineServices.Samples
{
    using UnityEngine;

    /// <summary>
    /// Provides a simple utility, where this panel will be displayed
    /// only if inside a UNITY_EDITOR context. Otherwise, disables
    /// the <see cref="VisualComponent"/>, and the visual elements underneath.
    /// </summary>
    public class DisabledInEditorWarningPanel : MonoBehaviour
    {
        /// <summary>
        /// This object is set to be hidden if not in UNITY_EDITOR,
        /// and will only be visible inside the editor context.
        /// This can be an inner game object, or applied to this
        /// object.
        /// </summary>
        public GameObject VisualComponent;

        public void OnEnable()
        {
#if UNITY_EDITOR
            VisualComponent.SetActive(true);
#else
            VisualComponent.SetActive(false);
#endif
        }
    }
}