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


namespace PlayEveryWare.EpicOnlineServices.Editor.Windows
{
    using PlayEveryWare.EpicOnlineServices.Editor.Utility;
    using System;
    using UnityEditor;
    using UnityEngine;

    /// <summary>
    /// Represents an editor window that is modal in nature - it is designed to
    /// only provide input for a single value, and not be dismissed from focus
    /// unless canceled or closed.
    /// </summary>
    /// <typeparam name="TInputType">
    /// The type of value that the modal window is designed to edit.
    /// </typeparam>
    public abstract class ModalEOSEditorWindow<TInputType> : EOSEditorWindow
    {
        /// <summary>
        /// The value being edited.
        /// </summary>
        protected TInputType _input;

        /// <summary>
        /// The prompt to display indicating what the value means.
        /// </summary>
        private string _inputPrompt;

        /// <summary>
        /// The prompt to display if the input is not valid.
        /// </summary>
        private string _errorPrompt;

        /// <summary>
        /// The action to take when the modal window is submitted.
        /// </summary>
        private Action<TInputType> _onSubmit;

        /// <summary>
        /// The function used to validate the input.
        /// </summary>
        private Func<TInputType, bool> _validateFunction;

        /// <summary>
        /// Flag that indicates whether the error prompt should be displayed.
        /// </summary>
        private bool _showError;

        // Keep a reference to prevent focus loss
        private static ModalEOSEditorWindow<TInputType> s_currentWindow;

        /// <summary>
        /// Constructs a noew modal eos editor window.
        /// </summary>
        /// <param name="windowTitle">Window title.</param>
        /// <param name="width">The fixed width of the window.</param>
        /// <param name="height">The fixed height of the window.</param>
        protected ModalEOSEditorWindow(string windowTitle, float width, float height) : base(windowTitle, height, width, false)
        {
        }

        /// <summary>
        /// Helper to schedule the showing of the modal window.
        /// </summary>
        /// <typeparam name="TWindowType">Type of value modal window is designed to edit.</typeparam>
        /// <param name="input">The value that the modal window provides editing of.</param>
        /// <param name="onSubmitCallback">The action that takes place when the modal window is submitted with a valid value.</param>
        /// <param name="validateFunction">Function used to validate the input.</param>
        /// <param name="inputPrompt">The prompt to display in the modal window.</param>
        /// <param name="errorPrompt">The error to display if the input value is invalid.</param>
        protected static void ScheduleShow<TWindowType>(
            TInputType input,
            Action<TInputType> onSubmitCallback,
            Func<TInputType, bool> validateFunction,
            string inputPrompt,
            string errorPrompt
        ) where TWindowType : ModalEOSEditorWindow<TInputType>
        {
            void ShowAndUnsubscribe()
            {
                // Unsubscribe first to ensure it runs only once
                EditorApplication.update -= ShowAndUnsubscribe;

                // Open the modal window
                ShowWindow<TWindowType>(input, onSubmitCallback, validateFunction, inputPrompt, errorPrompt);
            }

            // Subscribe the delegate to execute on the next frame
            EditorApplication.update += ShowAndUnsubscribe;
        }

        public static void ShowWindow<TWindowType>(
            TInputType input, 
            Action<TInputType> onSubmitCallback, 
            Func<TInputType, bool> validateFunction,
            string inputPrompt,
            string errorPrompt) where TWindowType : ModalEOSEditorWindow<TInputType>
        {
            if (s_currentWindow != null)
            {
                s_currentWindow.Focus();
                return;
            }

            ModalEOSEditorWindow<TInputType> window = CreateInstance<TWindowType>();
            window._input = input;
            window._inputPrompt = inputPrompt;
            window._onSubmit = onSubmitCallback;
            window._validateFunction = validateFunction;
            window._errorPrompt = errorPrompt;

            // Center the modal window relative to the parent window
            if (focusedWindow != null)
            {
                Rect parentRect = focusedWindow.position;
                float width = 300f;  // Width of the modal window
                float height = 150f; // Height of the modal window
                window.position = new Rect(
                    parentRect.x + (parentRect.width - width) / 2,
                    parentRect.y + (parentRect.height - height) / 2,
                    width,
                    height
                );
            }
            else
            {
                // Default size and position if no parent is specified
                window.position = new Rect(Screen.width / 2 - 150, Screen.height / 2 - 75, 300, 150);
            }

            window.ShowModalUtility(); // Makes it modal-like
            window.SetIsEmbedded(false);

            s_currentWindow = window;
        }

        private void OnLostFocus()
        {
            // Refocus if the window loses focus
            Focus();
        }

        protected new void OnEnable()
        {
            // Overridden to remove base functionality.
        }

        protected new void OnDisable()
        {
            // Overridden to remove base functionality.
        }

        protected override void OnDestroy()
        {
            s_currentWindow = null;
            base.OnDestroy();
        }

        /// <summary>
        /// Deriving modal window implementations should implement this function
        /// to render the contents of the input.
        /// </summary>
        protected abstract void RenderModalContents();

        protected override void RenderWindow()
        {
            bool shouldClose = false;

            // Render the prompt text
            EditorGUILayout.LabelField(_inputPrompt, GUILayout.Width(
                GUIEditorUtility.MeasureLabelWidth(_inputPrompt))
            );

            // Display error if it needs to be displayed.
            if (_showError)
            {
                EditorGUILayout.HelpBox(_errorPrompt, MessageType.Warning);
            }

            // Render the contents that are unique to the modal window implementation.
            RenderModalContents();

            EditorGUILayout.Space();

            GUILayout.BeginHorizontal();
            if (GUILayout.Button("Save"))
            {
                // Try to validate the value being submitted
                if (_validateFunction(_input))
                {
                    // If successful, then call the submit action.
                    _onSubmit?.Invoke(_input);
                    shouldClose = true;
                }
                else
                {
                    // Otherwise, turn on the flag that indicates the error should be displayed.
                    _showError = true;
                }
            }

            GUI.SetNextControlName("CancelButton");
            if (GUILayout.Button("Cancel"))
            {
                shouldClose = true;
            }
            GUILayout.EndHorizontal();

            // Close if it should be.
            if (shouldClose)
            {
                Close();
            }

            // Force focus back to the window
            if (s_currentWindow != this)
            {
                Focus();
                s_currentWindow = this;
            }
        }
    }
}