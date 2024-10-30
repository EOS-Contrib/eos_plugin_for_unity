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

namespace PlayEveryWare.EpicOnlineServices.Editor
{
    using UnityEditor;
    using UnityEngine;
    using Utility;

    /// <summary>
    /// This is a special case config editor because the ability to display a
    /// set of override values for the sandbox deployment and clients in a
    /// collapsible and concise manner is a unique requirement.
    /// </summary>
    public class ProductConfigEditor : ConfigEditor<ProductConfig>
    {
        public override void RenderContents()
        {
            float labelWidth = GUIEditorUtility.MeasureLongestLabelWidth(new()
            {
                "Name",
                "Product Version",
                "Default Sandbox",
                "Default Deployment",
                "Default Sandbox",
                "Default Client",
            });

            GUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("Name", GUILayout.Width(labelWidth));
            //config.ProductId = GUIEditorUtility.RenderInput(config.ProductId, "Product Name", labelWidth);
            GUILayout.EndHorizontal();
            config.Version = GUIEditorUtility.RenderInput(config.Version, "Product Version", "", labelWidth);

            GUILayout.BeginHorizontal();
            EditorGUILayout.LabelField("Sandbox", GUILayout.Width(labelWidth));
            GUILayout.EndHorizontal();


        }
    }
}