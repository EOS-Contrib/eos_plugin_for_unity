using Epic.OnlineServices;
using System;

namespace ApexSystems.Utility
{
    public class PluginVersionInterface
    {
        /// <summary>
        /// The company name
        /// </summary>
        public static readonly Utf8String COMPANY_NAME = "Apex Systems, Inc.";
        /// <summary>
        /// The copyright <see cref="Utf8String" />
        /// </summary>
        public static readonly Utf8String COPYRIGHT_STRING = "";
        /// <summary>
        /// The Hotfix version number
        /// </summary>
        public const int HOTFIX = 2;
        /// <summary>
        /// The Major version number
        /// </summary>
        public const int MAJOR = 1;
        /// <summary>
        /// The Minor version number
        /// </summary>
        public const int MINOR = 18;
        /// <summary>
        /// The Patch version number
        /// </summary>
        public const int PATCH = 1;
        /// <summary>
        /// The product identifier
        /// </summary>
        public static readonly Utf8String PRODUCT_IDENTIFIER = "Unity Plugin for Epic Online Services";
        /// <summary>
        /// The product name
        /// </summary>
        public static readonly Utf8String PRODUCT_NAME = "Unity Plugin for Epic Online Services";

        /// <summary>
        /// Get the version of the active Plugin version
        /// MAJOR.MINOR.PATCH.HOTFIX
        public static Utf8String GetVersion()
        {
            var callResult = MAJOR + "." + MINOR + "." + PATCH + "." + HOTFIX;
            return callResult;
        }
    }
}