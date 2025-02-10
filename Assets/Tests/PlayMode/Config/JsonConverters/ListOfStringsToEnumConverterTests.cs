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

namespace PlayEveryWare.EpicOnlineServices.Tests.Config
{
    using Epic.OnlineServices.UI;
    using NUnit.Framework;
    using Newtonsoft.Json;
    using System;
    using System.Collections.Generic;
    using Newtonsoft.Json.Linq;
    public static partial class ListOfStringsToEnumConverterTests
    {
        private static readonly Dictionary<string, TestOrderEnum> TESTORDERENUM_CUSTOM_MAPPING = new()
        {
            { "FirstLetter", TestOrderEnum.A },
            { "SecondLetter", TestOrderEnum.B },
            { "ThirdLetter", TestOrderEnum.C },
            { "AppleStartsWith", TestOrderEnum.A }
        };

        [Flags]
        private enum TestOrderEnum : int
        {
            None = 0x00000,
            A = 0x00001,
            B = 0x00004,
            C = 0x00008
        }

        /// <summary>
        /// This Enum is explicitly marked as a flag, and lacks a "0" value.
        /// There are Flags inside the EOS SDK C# that do not set a "0".
        /// <seealso cref="Epic.OnlineServices.IntegratedPlatform.IntegratedPlatformManagementFlags"/>
        /// </summary>
        [Flags]
        private enum TestFlagWithoutZeroEnum : int
        {
            A = 0x00001,
            B = 0x00002,
            C = 0x00004
        }

        private class ListOfStringsToEnumConverterTestClass<TEnum> : ListOfStringsToEnumConverter<TEnum> where TEnum : struct, Enum
        {
            private readonly Dictionary<string, TEnum> mappings;

            public ListOfStringsToEnumConverterTestClass(Dictionary<string, TEnum> inMappings)
            {
                mappings = inMappings;
            }

            protected override TEnum FromStringArray(JArray array)
            {
                return FromStringArrayWithCustomMapping(array, mappings);
            }
        }

        [Test]
        public static void ListOfStringsToEnumConverterTestClass_ReadJson_ArrayOfStrings_ReturnsCorrectEnum()
        {
            var converter = new ListOfStringsToEnumConverterTestClass<TestOrderEnum>(TESTORDERENUM_CUSTOM_MAPPING);
            var json = JArray.FromObject(new[] { "FirstLetter", "ThirdLetter", "B" });
            var result = (TestOrderEnum)converter.ReadJson(json.CreateReader(), typeof(InputStateButtonFlags), null, null);

            Assert.AreEqual(TestOrderEnum.A | TestOrderEnum.C | TestOrderEnum.B, result);
        }

        [Test]
        public static void ListOfStringsToEnumConverterTestClass_ReadJson_SingleString_ReturnsCorrectEnum()
        {
            var converter = new ListOfStringsToEnumConverterTestClass<TestOrderEnum>(TESTORDERENUM_CUSTOM_MAPPING);
            var json = JToken.FromObject("A");
            var result = (TestOrderEnum)converter.ReadJson(json.CreateReader(), typeof(InputStateButtonFlags), null, null);

            Assert.AreEqual(TestOrderEnum.A, result);
        }

        [Test]
        public static void ListOfStringsToEnumConverterTestClass_ReadJson_Null_ReturnsDefaultEnum()
        {
            var converter = new ListOfStringsToEnumConverterTestClass<TestOrderEnum>(TESTORDERENUM_CUSTOM_MAPPING);
            var result = (TestOrderEnum)converter.ReadJson(new JValue((string)null).CreateReader(), typeof(InputStateButtonFlags), null, null);

            Assert.AreEqual(TestOrderEnum.None, result);
        }

        [Test]
        public static void ListOfStringsToEnumConverterTestClass_ReadJson_InvalidTokenType_ThrowsException()
        {
            var converter = new ListOfStringsToEnumConverterTestClass<TestOrderEnum>(TESTORDERENUM_CUSTOM_MAPPING);
            var invalidJson = JToken.FromObject(123); // Invalid type for enum parsing

            Assert.Throws<JsonSerializationException>(() => converter.ReadJson(invalidJson.CreateReader(), typeof(TestOrderEnum), null, null));
        }

        /// <summary>
        /// Converts a '0' into <see cref="TestFlagWithoutZeroEnum"/> using the <see cref="ListOfStringsToEnumConverterTestClass"/>.
        /// Even though <see cref="TestFlagWithoutZeroEnum"/> lacks a 0, it is a Flag enum.
        /// The resulting value should be equivalent to a '0' integer, as opposed to using the lowest value.
        /// </summary>
        [Test]
        public static void ListOfStringsToEnumConverterTestClass_ReadJson_TestFlagWithoutZeroEnum_EvaluatesToZero()
        {
            var converter = new ListOfStringsToEnumConverterTestClass<TestFlagWithoutZeroEnum>(null);
            var json = JToken.FromObject(0);
            var result = (TestFlagWithoutZeroEnum)converter.ReadJson(json.CreateReader(), typeof(TestFlagWithoutZeroEnum), null, null);

            Assert.AreEqual((TestFlagWithoutZeroEnum)0, result);
        }
    }
}