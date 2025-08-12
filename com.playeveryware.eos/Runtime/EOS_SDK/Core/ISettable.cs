// Copyright Epic Games, Inc. All Rights Reserved.

using System;

namespace Epic.OnlineServices
{
	internal interface ISettable<T> : IDisposable
		where T : struct
	{
		void Set(ref T other);
	}
}