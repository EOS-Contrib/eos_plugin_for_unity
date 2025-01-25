using PlayEveryWare.EpicOnlineServices.Editor.Utility;
using System;
using System.Threading.Tasks;
using UnityEditor;
using UnityEngine;

namespace PlayEveryWare.EpicOnlineServices.Editor.Windows
{
    public sealed class ClientCredentialsEncryptionKeyWindow : ModalInputWindow<string>
    {
        public ClientCredentialsEncryptionKeyWindow() : base(600f, 50f)
        {
        }

        public static void Show(
            string input,
            Action<string> onSubmitCallback,
            string windowTitle = "Client Credentials Encryption Key",
            string inputPrompt = "Enter the encryption key for these client credentials here:")
        {
            ScheduleShow<ClientCredentialsEncryptionKeyWindow>(
                input, 
                onSubmitCallback, 
                EOSClientCredentials.IsEncryptionKeyValid, 
                windowTitle, 
                inputPrompt,
                "Invalid encryption key. Encryption key must be 64 characters long and contain only alphanumeric characters.");
        }

        protected override void RenderModalContents()
        {
            GUILayout.BeginHorizontal();

            _input = GUILayout.TextField(_input, GUILayout.Width(GUIEditorUtility.MeasureLabelWidth(64)), GUILayout.Height(20));

            GUILayout.Space(5f);

            if (GUILayout.Button(
                new GUIContent(EditorGUIUtility.IconContent("Refresh").image, 
                "Click here to generate a new encryption key."), GUILayout.Height(20), GUILayout.Width(50)))
            {
                _input = EOSClientCredentials.GenerateEncryptionKey();
            }
            GUILayout.EndHorizontal();
        }
    }

    public abstract class ModalInputWindow<TInputType> : EOSEditorWindow
    {
        protected TInputType _input;
        private string _inputPrompt;
        private string _errorPrompt;
        private Action<TInputType> _onSubmit;
        private Func<TInputType, bool> _validateFunction;
        private bool _showError;

        // Keep a reference to prevent focus loss
        private static ModalInputWindow<TInputType> s_currentWindow;

        protected ModalInputWindow(float width, float height) : base("", height, width, false)
        {
        }

        /// <summary>
        /// Helper to schedule the showing of the modal window.
        /// </summary>
        protected static void ScheduleShow<TWindowType>(
            TInputType input,
            Action<TInputType> onSubmitCallback,
            Func<TInputType, bool> validateFunction,
            string windowTitle,
            string inputPrompt,
            string errorPrompt
        ) where TWindowType : ModalInputWindow<TInputType>
        {
            void ShowAndUnsubscribe()
            {
                // Unsubscribe first to ensure it runs only once
                EditorApplication.update -= ShowAndUnsubscribe;

                // Open the modal window
                ShowWindow<TWindowType>(input, onSubmitCallback, validateFunction, windowTitle, inputPrompt, errorPrompt);
            }

            // Subscribe the delegate to execute on the next frame
            EditorApplication.update += ShowAndUnsubscribe;
        }

        public static void ShowWindow<TWindowType>(
            TInputType input, 
            Action<TInputType> onSubmitCallback, 
            Func<TInputType, bool> validateFunction,
            string windowTitle,
            string inputPrompt,
            string errorPrompt) where TWindowType : ModalInputWindow<TInputType>
        {
            if (s_currentWindow != null)
            {
                s_currentWindow.Focus();
                return;
            }

            ModalInputWindow<TInputType> window = CreateInstance<TWindowType>();
            window._input = input;
            window.WindowTitle = windowTitle;
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

        protected abstract void RenderModalContents();

        protected override void RenderWindow()
        {
            bool shouldClose = false;

            EditorGUILayout.LabelField(_inputPrompt, GUILayout.Width(
                GUIEditorUtility.MeasureLabelWidth(_inputPrompt))
            );

            if (_showError)
            {
                EditorGUILayout.HelpBox(_errorPrompt, MessageType.Warning);
            }

            RenderModalContents();

            EditorGUILayout.Space();

            GUILayout.BeginHorizontal();
            if (GUILayout.Button("Save"))
            {
                if (_validateFunction(_input))
                {
                    _onSubmit?.Invoke(_input);
                    shouldClose = true;
                }
                else
                {
                    _showError = true;
                }
            }

            GUI.SetNextControlName("CancelButton");
            if (GUILayout.Button("Cancel"))
            {
                shouldClose = true;
            }
            GUILayout.EndHorizontal();

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