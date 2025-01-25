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
    using PlayEveryWare.EpicOnlineServices;
    using System;
    using UnityEditor;
    using UnityEngine;

    /// <summary>
    /// Represents a modal window used to display and edit the encryption key field of a set of client credentials.
    /// </summary>
    public sealed class EncryptionKeyWindow : ModalEOSEditorWindow<string>
    {
        public EncryptionKeyWindow() : base("Client Credential Encryption Key", 600f, 50f)
        {
        }

        /// <summary>
        /// Helper function to show the modal window.
        /// </summary>
        /// <param name="input">
        /// The encryption key string.
        /// </param>
        /// <param name="onSubmitCallback">
        /// The action to take when the modal window is submitted.
        /// </param>
        public static void Show(string input, Action<string> onSubmitCallback)
        {
            ScheduleShow<EncryptionKeyWindow>(
                input,
                onSubmitCallback,
                EOSClientCredentials.IsEncryptionKeyValid,
                "Enter the encryption key for these client credentials here:",
                "Invalid encryption key. Encryption key must be 64 characters long and contain only alphanumeric characters.");
        }

        /// <summary>
        /// Renders the fields necessary to edit the encryption key.
        /// </summary>
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

}