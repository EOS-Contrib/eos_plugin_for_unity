// Copyright Epic Games, Inc. All Rights Reserved.

using System;

namespace Epic.OnlineServices
{
	internal interface ICallbackInfo
	{
		object GetClientData();
		Result? GetResultCode();
	}

	internal interface ICallbackInfoInternal
	{
		IntPtr ClientDataPointer { get; }
	}
}
