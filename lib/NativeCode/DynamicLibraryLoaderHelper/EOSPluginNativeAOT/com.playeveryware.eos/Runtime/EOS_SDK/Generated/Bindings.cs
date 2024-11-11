namespace Epic.OnlineServices
{
    using System;
    using System.Runtime.InteropServices;
 
    public static partial class Bindings
    {
        [UnmanagedFunctionPointer(Config.LibraryCallingConvention)]
        internal delegate System.IntPtr EOS_EResult_ToStringDelegate(Result result);
        internal static EOS_EResult_ToStringDelegate EOS_EResult_ToString;

        [UnmanagedFunctionPointer(Config.LibraryCallingConvention)]
        internal delegate int EOS_EResult_IsOperationCompleteDelegate(Result result);
        internal static EOS_EResult_IsOperationCompleteDelegate EOS_EResult_IsOperationComplete;

        [UnmanagedFunctionPointer(Config.LibraryCallingConvention)]
        internal delegate Result EOS_ByteArray_ToStringDelegate(System.IntPtr byteArray, uint length, System.IntPtr outBuffer, ref uint inOutBufferLength);
        internal static EOS_ByteArray_ToStringDelegate EOS_ByteArray_ToString;

        private const string EOS_ByteArray_ToStringName = "EOS_ByteArray_ToString";
        private const string EOS_EResult_ToStringName = "EOS_EResult_ToString";
        private const string EOS_EResult_IsOperationCompleteName = "EOS_EResult_IsOperationComplete";
        
        public static void Hook<TLibraryHandle>(TLibraryHandle libraryHandle,
            Func<TLibraryHandle, string, IntPtr> getFunctionPointer)
        {
            System.IntPtr functionPointer;

            functionPointer = getFunctionPointer(libraryHandle, EOS_EResult_ToStringName);
            if (functionPointer == System.IntPtr.Zero) throw new DynamicBindingException(EOS_EResult_ToStringName);
            EOS_EResult_ToString = (EOS_EResult_ToStringDelegate)Marshal.GetDelegateForFunctionPointer(functionPointer, typeof(EOS_EResult_ToStringDelegate));

            functionPointer = getFunctionPointer(libraryHandle, EOS_EResult_IsOperationCompleteName);
            if (functionPointer == System.IntPtr.Zero) throw new DynamicBindingException(EOS_EResult_IsOperationCompleteName);
            EOS_EResult_IsOperationComplete = (EOS_EResult_IsOperationCompleteDelegate)Marshal.GetDelegateForFunctionPointer(functionPointer, typeof(EOS_EResult_IsOperationCompleteDelegate));

            functionPointer = getFunctionPointer(libraryHandle, EOS_ByteArray_ToStringName);
            if (functionPointer == System.IntPtr.Zero) throw new DynamicBindingException(EOS_ByteArray_ToStringName);
            EOS_ByteArray_ToString = (EOS_ByteArray_ToStringDelegate)Marshal.GetDelegateForFunctionPointer(functionPointer, typeof(EOS_ByteArray_ToStringDelegate));
        }
    }
}
