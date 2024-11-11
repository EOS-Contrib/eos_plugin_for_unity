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

using System.IO;
using System.Reflection;

namespace PlayEveryWare.EpicOnlineServices
{
    using Newtonsoft.Json;

    /// <summary>
    /// This class is used to read values from an embedded "config.json" file.
    /// </summary>
    public static class ResourceUtility
    {
        /// <summary>
        /// Private readonly instance of the resource values.
        /// </summary>
        private static readonly EmbeddedResourceValues s_values;

        /// <summary>
        /// Kept internal because there are a very limited set of circumstances
        /// where these values need to be used.
        /// </summary>
        internal class EmbeddedResourceValues
        {
            public string StreamingAssetsPath { get; set; }
        }

        /// <summary>
        /// Static constructor so that the static values can be set properly.
        /// </summary>
        static ResourceUtility()
        {
            s_values = GetEmbeddedResourceValues();
        }

        /// <summary>
        /// Reads an embedded resource file of the given name.
        /// </summary>
        /// <param name="resourceFileName">
        /// The name of the embedded file to read from.
        /// </param>
        /// <returns>
        /// The string contents of the indicated embedded resource file.
        /// </returns>
        /// <exception cref="FileNotFoundException">
        /// Thrown if the embedded file is not found.
        /// </exception>
        private static string ReadEmbeddedJsonFile(string resourceFileName)
        {
            var assembly = Assembly.GetExecutingAssembly();

            // Automatically prepend the assembly name to the resource path
            string fullResourceName = $"EOSPluginConfig.{resourceFileName}";

            using Stream stream = assembly.GetManifestResourceStream(fullResourceName);

            // Deal with the circumstance where the file does not exist.
            if (stream == null)
                throw new FileNotFoundException($"Resource not found: {fullResourceName}");

            using StreamReader reader = new(stream);
            return reader.ReadToEnd();
        }

        /// <summary>
        /// Gets the embedded resource values.
        /// </summary>
        /// <returns>
        /// The embedded resources contained within config.json.
        /// </returns>
        private static EmbeddedResourceValues GetEmbeddedResourceValues()
        {
            string jsonContent = ReadEmbeddedJsonFile("config.json");
            return JsonConvert.DeserializeObject<EmbeddedResourceValues>(jsonContent);
        }

        /// <summary>
        /// Gets the streaming assets path.
        /// </summary>
        /// <returns>
        /// Fully-qualified path to the streaming assets path.
        /// </returns>
        public static string GetStreamingAssetsPath()
        {
            return s_values.StreamingAssetsPath;
        }
    }
}
