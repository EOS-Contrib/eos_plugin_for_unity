// Copyright Epic Games, Inc. All Rights Reserved.

namespace Epic.OnlineServices
{
	internal interface IGettable<T>
		where T : struct
	{
		void Get(out T other);
	}

	internal interface IGettable<T, TEnum>
		where T : struct
	{
		void Get(out T other, TEnum enumValue, int? arrayLength);
	}
}