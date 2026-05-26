// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.Runtime.InteropServices;

namespace Epic.OnlineServices
{
	public sealed partial class Helper
	{
		internal static void Get<TArray>(TArray[] from, out int to)
		{
			Convert(from, out to);
		}

		internal static void Get<TArray>(TArray[] from, out uint to)
		{
			Convert(from, out to);
		}

		internal static void Get(ArraySegment<byte> from, out uint to)
		{
			Convert(from, out to);
		}

		internal static void Get<TInternal, TPublic>(ref TInternal from, out TPublic to)
			where TInternal : struct, IGettable<TPublic>
			where TPublic : struct
		{
			from.Get(out to);
		}

		internal static void Get<TInternal, TPublic>(ref TInternal from, out TPublic? to)
			where TInternal : struct, IGettable<TPublic>
			where TPublic : struct
		{
			TPublic toPublic = default;
			from.Get(out toPublic);
			to = toPublic;
		}

		internal static void Get<T>(T from, out T? to)
			where T : struct
		{
			to = from;
		}

		internal static void Get(int from, out bool to)
		{
			Convert(from, out to);
		}

		internal static void Get(int from, out bool? to)
		{
			bool intermediate;
			Convert(from, out intermediate);
			to = intermediate;
		}

		internal static void Get(bool from, out int to)
		{
			Convert(from, out to);
		}

		internal static void Get(long from, out DateTimeOffset? to)
		{
			Convert(from, out to);
		}

		internal static void Get(IntPtr from, out ArraySegment<byte> to, uint arrayLength)
		{
			to = new ArraySegment<byte>();
			if (arrayLength != 0)
			{
				byte[] bytes = new byte[arrayLength];
				Marshal.Copy(from, bytes, 0, (int)arrayLength);
				to = new ArraySegment<byte>(bytes);
			}
		}

		internal static void Get(IntPtr from, out Utf8String[] to, int arrayLength, bool isArrayItemAllocated)
		{
			GetAllocation(from, out to, arrayLength, isArrayItemAllocated);
		}

		internal static void Get(IntPtr from, out Utf8String[] to, uint arrayLength, bool isArrayItemAllocated)
		{
			GetAllocation(from, out to, (int)arrayLength, isArrayItemAllocated);
		}

		internal static void Get<T>(IntPtr from, out T[] to, uint arrayLength, bool isArrayItemAllocated)
			where T : struct
		{
			GetAllocation(from, out to, (int)arrayLength, isArrayItemAllocated);
		}

		internal static void Get<T>(IntPtr from, out T[] to, int arrayLength, bool isArrayItemAllocated)
			where T : struct
		{
			GetAllocation(from, out to, arrayLength, isArrayItemAllocated);
		}

		internal static void Get<THandle>(IntPtr from, out THandle to)
			where THandle : Handle, new()
		{
			Convert(from, out to);
		}

		internal static void Get<THandle>(IntPtr from, out THandle[] to, uint arrayLength)
			where THandle : Handle, new()
		{
			GetAllocation(from, out to, (int)arrayLength);
		}

		internal static void Get(IntPtr from, out IntPtr[] to, uint arrayLength)
		{
			GetAllocation(from, out to, (int)arrayLength, false);
		}

		internal static void Get<TInternal, TPublic>(TInternal[] from, out TPublic[] to)
			where TInternal : struct, IGettable<TPublic>
			where TPublic : struct
		{
			to = default;

			if (from != null)
			{
				to = new TPublic[from.Length];

				for (int index = 0; index < from.Length; ++index)
				{
					from[index].Get(out to[index]);
				}
			}
		}

		internal static void Get<TInternal, TPublic>(IntPtr from, out TPublic[] to, int arrayLength, bool isArrayItemAllocated)
			where TInternal : struct, IGettable<TPublic>
			where TPublic : struct
		{
			TInternal[] fromInternal;
			Get(from, out fromInternal, arrayLength, isArrayItemAllocated);
			Get(fromInternal, out to);
		}

		internal static void Get<TInternal, TPublic>(IntPtr from, out TPublic[] to, uint arrayLength, bool isArrayItemAllocated)
			where TInternal : struct, IGettable<TPublic>
			where TPublic : struct
		{
			Get<TInternal, TPublic>(from, out to, (int)arrayLength, isArrayItemAllocated);
		}

		internal static void Get<T>(IntPtr from, out T? to)
			where T : struct
		{
			GetAllocation(from, out to);
		}

		internal static void Get(byte[] from, out Utf8String to)
		{
			Convert(from, out to);
		}

		internal static void Get(IntPtr from, out object to)
		{
			to = GetClientData(from);
		}

		internal static void Get(IntPtr from, out Utf8String to)
		{
			GetAllocation(from, out to);
		}

		internal static void Get<TInternal, TPublic>(IntPtr from, out TPublic to)
			where TInternal : struct, IGettable<TPublic>
			where TPublic : struct
		{
			to = default;

			TInternal? fromInternal;
			Get(from, out fromInternal);

			if (fromInternal.HasValue)
			{
				fromInternal.Value.Get(out to);
			}
		}

		internal static void Get<TInternal, TPublic>(IntPtr from, out TPublic? to)
			where TInternal : struct, IGettable<TPublic>
			where TPublic : struct
		{
			to = default;

			TInternal? fromInternal;
			Get(from, out fromInternal);

			if (fromInternal.HasValue)
			{
				TPublic toPublic;
				fromInternal.Value.Get(out toPublic);

				to = toPublic;
			}
		}

		internal static void Get<TInternal, TPublic>(ref TInternal from, out TPublic to, out IntPtr clientDataPointer)
			where TInternal : struct, ICallbackInfoInternal, IGettable<TPublic>
			where TPublic : struct
		{
			from.Get(out to);
			clientDataPointer = from.ClientDataPointer;
		}
	}
}