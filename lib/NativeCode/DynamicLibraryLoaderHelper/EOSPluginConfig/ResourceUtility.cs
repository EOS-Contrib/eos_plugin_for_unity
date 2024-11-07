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

    public static class ResourceUtility
    {
        private static readonly EmbeddedResourceValues s_values;

        internal class EmbeddedResourceValues
        {
            public string StreamingAssetsPath { get; set; }
        }

        static ResourceUtility()
        {
            s_values = GetEmbeddedResourceValues();
        }

        private static string ReadEmbeddedJsonFile(string resourceFileName)
        {
            var assembly = Assembly.GetExecutingAssembly();

            // Automatically prepend the assembly name to the resource path
            string fullResourceName = $"EOSPluginConfig.{resourceFileName}";

            using (Stream stream = assembly.GetManifestResourceStream(fullResourceName))
            {
                if (stream == null)
                    throw new FileNotFoundException($"Resource not found: {fullResourceName}");

                using (StreamReader reader = new StreamReader(stream))
                {
                    return reader.ReadToEnd();
                }
            }
        }

        private static EmbeddedResourceValues GetEmbeddedResourceValues()
        {
            string jsonContent = ReadEmbeddedJsonFile("config.json");
            return JsonConvert.DeserializeObject<EmbeddedResourceValues>(jsonContent);
        }

        public static string GetStreamingAssetsPath()
        {
            return s_values.StreamingAssetsPath;
        }
    }
}
