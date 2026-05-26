// Copyright Epic Games, Inc. All Rights Reserved.

using Epic.OnlineServices.UI;
using System;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Epic.OnlineServices
{
	public sealed partial class Helper
	{
		internal static void Set<T>(T from, ref T? to)
			where T : struct
		{
			to = from;
		}

		internal static void Set<T>(T? from, ref T to)
			where T : struct
		{
			to = default;

			if (from.HasValue)
			{
				to = from.Value;
			}
		}

		internal static void Set<T>(T? from, ref T? to)
			where T : struct
		{
			to = from;
		}

		internal static void Set(bool? from, ref int to)
		{
			to = default;

			if (from.HasValue)
			{
				Convert(from.Value, out to);
			}
		}

		internal static void Set<T>(T from, ref IntPtr to)
			where T : struct
        {
            Dispose(ref to);

            to = AddAllocation(Marshal.SizeOf(typeof(T)), from);
			Marshal.StructureToPtr(from, to, false);
		}

		internal static void Set<T>(T? from, ref IntPtr to)
			where T : struct
		{
			Dispose(ref to);

			if (from.HasValue)
			{
				to = AddAllocation(Marshal.SizeOf(typeof(T)), from);
				Marshal.StructureToPtr(from.Value, to, false);
			}
		}

		internal static void Set(object from, ref IntPtr to)
		{
			Dispose(ref to);
			AddCallback(out to, from);
		}

		internal static void Set(Utf8String from, ref IntPtr to)
		{
			Dispose(ref to);
			to = AddPinnedBuffer(from);
		}

		internal static void Set(Handle from, ref IntPtr to)
		{
			Convert(from, out to);
		}

		internal static void Set<T>(T[] from, ref IntPtr to, bool isArrayItemAllocated)
		{
			Dispose(ref to);
			to = AddAllocation(from, isArrayItemAllocated);
		}

		internal static void Set(ArraySegment<byte> from, ref IntPtr to, out uint arrayLength)
		{
			Dispose(ref to);
			to = AddPinnedBuffer(from);
			Get(from, out arrayLength);
		}

		internal static void Set<T>(T[] from, ref IntPtr to, out int arrayLength, bool isArrayItemAllocated)
		{
			Set(from, ref to, isArrayItemAllocated);
			Get(from, out arrayLength);
		}

		internal static void Set<T>(T[] from, ref IntPtr to, out uint arrayLength, bool isArrayItemAllocated)
		{
			Set(from, ref to, isArrayItemAllocated);
			Get(from, out arrayLength);
		}

		internal static void Set(DateTimeOffset? from, ref long to)
		{
			Convert(from, out to);
		}

		internal static void Set(bool from, ref int to)
		{
			Convert(from, out to);
		}

		internal static void Set(Utf8String from, ref byte[] to, int stringLength)
		{
			Convert(from, out to, stringLength);
		}

		internal static void Set<TPublic, TInternal>(ref TPublic from, ref TInternal to)
			where TPublic : struct
			where TInternal : struct, ISettable<TPublic>
		{
			to.Set(ref from);
		}

		internal static void Set<TPublic, TInternal>(TPublic? from, ref IntPtr to)
			where TPublic : struct
			where TInternal : struct, ISettable<TPublic>
		{
			Dispose(ref to);
			to = default;

			if (from.HasValue)
			{
				TInternal toInternal = default;
				var fromValue = from.Value;
				toInternal.Set(ref fromValue);
				to = AddAllocation(Marshal.SizeOf(typeof(TInternal)), toInternal);
			}
		}

		internal static void Set<TPublic, TInternal>(TPublic? from, ref TInternal to)
			where TPublic : struct
			where TInternal : struct, ISettable<TPublic>
		{
			Dispose(ref to);
			to = default;

			if (from.HasValue)
			{
				var fromValue = from.Value;
				to.Set(ref fromValue);
			}
		}

		internal static void Set(Utf8String[] from, ref IntPtr to, out int arrayLength, bool isArrayItemAllocated)
		{
			Dispose(ref to);

			to = AddAllocation(from, isArrayItemAllocated);
			Get(from, out arrayLength);
		}

		internal static void Set(Utf8String[] from, ref IntPtr to, out uint arrayLength, bool isArrayItemAllocated)
		{
			int arrayLengthIntermediate;
			Set(from, ref to, out arrayLengthIntermediate, isArrayItemAllocated);
			arrayLength = (uint)arrayLengthIntermediate;
		}

		internal static void Set<TPublic, TInternal>(TPublic from, ref IntPtr to)
			where TPublic : struct
			where TInternal : struct, ISettable<TPublic>
		{
			Dispose(ref to);

			TInternal toInternal = default;
			toInternal.Set(ref from);

			to = AddAllocation(Marshal.SizeOf(typeof(TInternal)), toInternal);
		}

		internal static void Set<TPublic, TInternal>(TPublic[] from, ref IntPtr to, out int arrayLength, bool isArrayItemAllocated)
			where TPublic : struct
			where TInternal : struct, ISettable<TPublic>
		{
			Dispose(ref to);
			arrayLength = 0;

			if (from != null)
			{
				TInternal[] toInternal = new TInternal[from.Length];
				for (int index = 0; index < from.Length; ++index)
				{
					toInternal[index].Set(ref from[index]);
				}

				Set(toInternal, ref to, isArrayItemAllocated);
				Get(from, out arrayLength);
			}
		}

		internal static void Set<TPublic, TInternal>(TPublic[] from, ref IntPtr to, out uint arrayLength, bool isArrayItemAllocated)
			where TPublic : struct
			where TInternal : struct, ISettable<TPublic>
		{
			int arrayLengthIntermediate;
			Set<TPublic, TInternal>(from, ref to, out arrayLengthIntermediate, isArrayItemAllocated);
			arrayLength = (uint)arrayLengthIntermediate;
		}
	}
}