// Copyright Epic Games, Inc. All Rights Reserved.

#if UNITY_SWITCH
	#define EOS_PLATFORM_SWITCH
#endif

#if EOS_PLATFORM_SWITCH
using System;
using System.Runtime.InteropServices;

namespace Epic.OnlineServices
{
	[StructLayout(LayoutKind.Sequential)]
	public struct SwitchUserHandle
	{
		[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
		public long[] Data;
		public UIntPtr Context;
	}
}
#endif