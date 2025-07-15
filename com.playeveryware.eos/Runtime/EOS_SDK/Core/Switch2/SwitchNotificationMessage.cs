// Copyright Epic Games, Inc. All Rights Reserved.

#if UNITY_SWITCH2
	#define EOS_PLATFORM_SWITCH2
#endif

#if EOS_PLATFORM_SWITCH2
using System;

namespace Epic.OnlineServices
{
	public enum SwitchNotificationMessage : int
	{
		ExitRequest = 4,
		FocusStateChanged = 15,
		Resume = 16,
		OperationModeChanged = 30,
		PerformanceModeChanged = 31
	}
}
#endif