// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace Epic.OnlineServices
{
	// In earlier versions of .NET and some versions of unity, IntPtr is not equatable and therefore cannot be used as a key
	// for dictionaries without causing memory allocations when comparing two IntPtr.
	// We therefore have to fall back on using an long int representation of pointers.
	using PointerType = UInt64;

	internal class AllocationException : Exception
	{
		public AllocationException(string message)
			: base(message)
		{
		}
	}

	internal class ExternalAllocationException : AllocationException
	{
		public ExternalAllocationException(IntPtr pointer, Type type)
			: base(string.Format("Attempting to allocate '{0}' over externally allocated memory at {1}", type, pointer.ToString("X")))
		{
		}
	}

	internal class CachedTypeAllocationException : AllocationException
	{
		public CachedTypeAllocationException(IntPtr pointer, Type foundType, Type expectedType)
			: base(string.Format("Cached allocation is '{0}' but expected '{1}' at {2}", foundType, expectedType, pointer.ToString("X")))
		{
		}
	}

	internal class CachedArrayAllocationException : AllocationException
	{
		public CachedArrayAllocationException(IntPtr pointer, int foundLength, int expectedLength)
			: base(string.Format("Cached array allocation has length {0} but expected {1} at {2}", foundLength, expectedLength, pointer.ToString("X")))
		{
		}
	}

	internal class DynamicBindingException : Exception
	{
		public DynamicBindingException(string bindingName)
			: base(string.Format("Failed to hook dynamic binding for '{0}'", bindingName))
		{
		}
	}

	/// <summary>
	/// A helper class that manages memory in the wrapper.
	/// </summary>
	public sealed partial class Helper
	{
		private struct Allocation
		{
			public int Size { get; private set; }

			public object Cache { get; private set; }

			public bool? IsArrayItemAllocated { get; private set; }

			public Allocation(int size, object cache, bool? isArrayItemAllocated = null)
			{
				Size = size;
				Cache = cache;
				IsArrayItemAllocated = isArrayItemAllocated;
			}
		}
		private struct PinnedBuffer
		{
			public GCHandle Handle { get; private set; }

			public int RefCount { get; set; }

			public PinnedBuffer(GCHandle handle)
			{
				Handle = handle;
				RefCount = 1;
			}
		}

		private class DelegateHolder
		{
			public List<Delegate> Delegates { get; private set; } = new List<Delegate>();
			public ulong? NotificationId { get; set; }

			public DelegateHolder(params Delegate[] delegates)
			{
				Delegates.AddRange(delegates.Where(d => d != null));
			}
		}

		private static Dictionary<PointerType, Allocation> s_Allocations = new Dictionary<PointerType, Allocation>();
		private static Dictionary<PointerType, PinnedBuffer> s_PinnedBuffers = new Dictionary<PointerType, PinnedBuffer>();
		private static Dictionary<IntPtr, DelegateHolder> s_Callbacks = new Dictionary<IntPtr, DelegateHolder>();
		private static Dictionary<string, DelegateHolder> s_StaticCallbacks = new Dictionary<string, DelegateHolder>();
		private static long s_LastClientDataId = 0;
		private static Dictionary<IntPtr, object> s_ClientDatas = new Dictionary<IntPtr, object>();

		/// <summary>
		/// Gets the number of unmanaged allocations and other stored values in the wrapper. Use this to find leaks related to the usage of wrapper code.
		/// </summary>
		/// <returns>The number of unmanaged allocations currently active within the wrapper.</returns>
		public static int GetAllocationCount()
		{
			return s_Allocations.Count + s_PinnedBuffers.Aggregate(0, (acc, x) => acc + x.Value.RefCount) + s_Callbacks.Count + s_ClientDatas.Count;
		}

		internal static void Copy(byte[] from, IntPtr to)
		{
			if (from != null && to != IntPtr.Zero)
			{
				Marshal.Copy(from, 0, to, from.Length);
			}
		}

		internal static void Copy(ArraySegment<byte> from, IntPtr to)
		{
			if (from.Count != 0 && to != IntPtr.Zero)
			{
				Marshal.Copy(from.Array, from.Offset, to, from.Count);
			}
		}

		internal static void Dispose(ref IntPtr value)
		{
			RemoveCallback(value);
			RemoveAllocation(ref value);
			RemovePinnedBuffer(ref value);
			value = default;
		}

		internal static void Dispose(ref IDisposable disposable)
		{
			disposable?.Dispose();
		}

		internal static void Dispose<TDisposable>(ref TDisposable disposable)
			where TDisposable : struct, IDisposable
		{
			disposable.Dispose();
		}

		private static int GetAnsiStringLength(byte[] bytes)
		{
			int length = 0;
			foreach (byte currentByte in bytes)
			{
				if (currentByte == 0)
				{
					break;
				}

				++length;
			}

			return length;
		}

		private static int GetAnsiStringLength(IntPtr pointer)
		{
			int length = 0;
			while (Marshal.ReadByte(pointer, length) != 0)
			{
				++length;
			}

			return length;
		}
		private static void GetAllocation<T>(IntPtr source, out T target)
		{
			target = default;

			if (source == IntPtr.Zero)
			{
				return;
			}

			object allocationCache;
			if (TryGetAllocationCache(source, out allocationCache))
			{
				if (allocationCache != null)
				{
					if (allocationCache.GetType() == typeof(T))
					{
						target = (T)allocationCache;
						return;
					}
					else
					{
						throw new CachedTypeAllocationException(source, allocationCache.GetType(), typeof(T));
					}
				}
			}

			target = (T)Marshal.PtrToStructure(source, typeof(T));
		}

		private static void GetAllocation<T>(IntPtr source, out T? target)
			where T : struct
		{
			target = default;

			if (source == IntPtr.Zero)
			{
				return;
			}

			// If this is an allocation containing cached data, we should be able to fetch it from the cache
			object allocationCache;
			if (TryGetAllocationCache(source, out allocationCache))
			{
				if (allocationCache != null)
				{
					if (allocationCache.GetType() == typeof(T))
					{
						target = (T?)allocationCache;
						return;
					}
					else
					{
						throw new CachedTypeAllocationException(source, allocationCache.GetType(), typeof(T));
					}
				}
			}

			if (typeof(T).IsEnum)
			{
				target = (T)Marshal.PtrToStructure(source, Enum.GetUnderlyingType(typeof(T)));
			}
			else
			{
				target = (T?)Marshal.PtrToStructure(source, typeof(T));
			}
		}

		private static void GetAllocation<THandle>(IntPtr source, out THandle[] target, int arrayLength)
			where THandle : Handle, new()
		{
			target = null;

			if (source == IntPtr.Zero)
			{
				return;
			}

			// If this is an allocation containing cached data, we should be able to fetch it from the cache
			object allocationCache;

			if (TryGetAllocationCache(source, out allocationCache))
			{
				if (allocationCache != null)
				{
					if (allocationCache.GetType() == typeof(THandle[]))
					{
						var cachedArray = (Array)allocationCache;
						if (cachedArray.Length == arrayLength)
						{
							target = cachedArray as THandle[];
							return;
						}
						else
						{
							throw new CachedArrayAllocationException(source, cachedArray.Length, arrayLength);
						}
					}
					else
					{
						throw new CachedTypeAllocationException(source, allocationCache.GetType(), typeof(THandle[]));
					}
				}
			}

			var itemSize = Marshal.SizeOf(typeof(IntPtr));

			List<THandle> items = new List<THandle>();
			for (int itemIndex = 0; itemIndex < arrayLength; ++itemIndex)
			{
				IntPtr itemPointer = new IntPtr(source.ToInt64() + itemIndex * itemSize);
				itemPointer = Marshal.ReadIntPtr(itemPointer);
				THandle item;
				Convert(itemPointer, out item);
				items.Add(item);
			}

			target = items.ToArray();
		}

		private static void GetAllocation<T>(IntPtr from, out T[] to, int arrayLength, bool isArrayItemAllocated)
		{
			to = null;

			if (from == IntPtr.Zero)
			{
				return;
			}

			// If this is an allocation containing cached data, we should be able to fetch it from the cache
			object allocationCache;
			if (TryGetAllocationCache(from, out allocationCache))
			{
				if (allocationCache != null)
				{
					if (allocationCache.GetType() == typeof(T[]))
					{
						var cachedArray = (Array)allocationCache;
						if (cachedArray.Length == arrayLength)
						{
							to = cachedArray as T[];
							return;
						}
						else
						{
							throw new CachedArrayAllocationException(from, cachedArray.Length, arrayLength);
						}
					}
					else
					{
						throw new CachedTypeAllocationException(from, allocationCache.GetType(), typeof(T[]));
					}
				}
			}

			int itemSize;
			if (isArrayItemAllocated)
			{
				itemSize = Marshal.SizeOf(typeof(IntPtr));
			}
			else
			{
				itemSize = Marshal.SizeOf(typeof(T));
			}

			List<T> items = new List<T>();
			for (int itemIndex = 0; itemIndex < arrayLength; ++itemIndex)
			{
				IntPtr itemPointer = new IntPtr(from.ToInt64() + itemIndex * itemSize);

				if (isArrayItemAllocated)
				{
					itemPointer = Marshal.ReadIntPtr(itemPointer);
				}

				T item;
				if (typeof(T) == typeof(Utf8String))
				{
					Utf8String str;
					GetAllocation(itemPointer, out str);
					item = (T)(object)(str);
				}
				else
				{
					GetAllocation(itemPointer, out item);
				}
				items.Add(item);
			}

			to = items.ToArray();
		}

		private static void GetAllocation(IntPtr source, out Utf8String target)
		{
			target = null;

			if (source == IntPtr.Zero)
			{
				return;
			}

			// C style strlen
			int length = GetAnsiStringLength(source);

			// +1 byte for the null terminator.
			byte[] bytes = new byte[length + 1];
			Marshal.Copy(source, bytes, 0, length + 1);

			target = new Utf8String(bytes);
		}

		internal static IntPtr AddAllocation(int size)
		{
			if (size == 0)
			{
				return IntPtr.Zero;
			}

			IntPtr pointer = Marshal.AllocHGlobal(size);
			Marshal.WriteByte(pointer, 0, 0);

			lock (s_Allocations)
			{
				s_Allocations.Add((PointerType)pointer, new Allocation(size, null));
			}

			return pointer;
		}

		internal static IntPtr AddAllocation(uint size)
		{
			return AddAllocation((int)size);
		}

		private static IntPtr AddAllocation<T>(int size, T cache)
		{
			if (size == 0 || cache == null)
			{
				return IntPtr.Zero;
			}

			IntPtr pointer = Marshal.AllocHGlobal(size);
			Marshal.StructureToPtr(cache, pointer, false);

			lock (s_Allocations)
			{
				s_Allocations.Add((PointerType)pointer, new Allocation(size, cache));
			}

			return pointer;
		}

		private static IntPtr AddAllocation<T>(int size, T[] cache, bool? isArrayItemAllocated)
		{
			if (size == 0 || cache == null)
			{
				return IntPtr.Zero;
			}

			IntPtr pointer = Marshal.AllocHGlobal(size);
			Marshal.WriteByte(pointer, 0, 0);

			lock (s_Allocations)
			{
				s_Allocations.Add((PointerType)pointer, new Allocation(size, cache, isArrayItemAllocated));
			}

			return pointer;
		}

		private static IntPtr AddAllocation<T>(T[] array, bool isArrayItemAllocated)
		{
			if (array == null)
			{
				return IntPtr.Zero;
			}

			int itemSize;
			if (isArrayItemAllocated || typeof(T).BaseType == typeof(Handle))
			{
				itemSize = Marshal.SizeOf(typeof(IntPtr));
			}
			else
			{
				itemSize = Marshal.SizeOf(typeof(T));
			}

			IntPtr newArrayPointer = AddAllocation(array.Length * itemSize, array, isArrayItemAllocated);

			for (int itemIndex = 0; itemIndex < array.Length; ++itemIndex)
			{
				var item = (T)array.GetValue(itemIndex);

				if (isArrayItemAllocated)
				{
					IntPtr newItemPointer;
					if (typeof(T) == typeof(Utf8String))
					{
						newItemPointer = AddPinnedBuffer((Utf8String)(object)item);
					}
					else
					{
						newItemPointer = AddAllocation(Marshal.SizeOf(typeof(T)), item);
					}

					// Copy the item's pointer into the array
					IntPtr itemPointer = new IntPtr(newArrayPointer.ToInt64() + itemIndex * itemSize);
					Marshal.StructureToPtr(newItemPointer, itemPointer, false);
				}
				else
				{
					// Copy the data straight into memory
					IntPtr itemPointer = new IntPtr(newArrayPointer.ToInt64() + itemIndex * itemSize);
					if (typeof(T).BaseType == typeof(Handle))
					{
						IntPtr newItemPointer;
						Convert((Handle)(object)item, out newItemPointer);
						Marshal.StructureToPtr(newItemPointer, itemPointer, false);
					}
					else
					{
						Marshal.StructureToPtr(item, itemPointer, false);
					}
				}
			}

			return newArrayPointer;
		}

		private static void RemoveAllocation(ref IntPtr pointer)
		{
			if (pointer == IntPtr.Zero)
			{
				return;
			}

			Allocation allocation;
			lock (s_Allocations)
			{
				if (!s_Allocations.TryGetValue((PointerType)pointer, out allocation))
				{
					return;
				}

				s_Allocations.Remove((PointerType)pointer);
			}

			// If the allocation is an array, dispose and release its items as needbe.
			if (allocation.IsArrayItemAllocated.HasValue)
			{
				int itemSize;
				if (allocation.IsArrayItemAllocated.Value || allocation.Cache.GetType().GetElementType().BaseType == typeof(Handle))
				{
					itemSize = Marshal.SizeOf(typeof(IntPtr));
				}
				else
				{
					itemSize = Marshal.SizeOf(allocation.Cache.GetType().GetElementType());
				}

				var array = allocation.Cache as Array;
				for (int itemIndex = 0; itemIndex < array.Length; ++itemIndex)
				{
					if (allocation.IsArrayItemAllocated.Value)
					{
						var itemPointer = new IntPtr(pointer.ToInt64() + itemIndex * itemSize);
						itemPointer = Marshal.ReadIntPtr(itemPointer);
						Dispose(ref itemPointer);
					}
					else
					{
						var item = array.GetValue(itemIndex);
						if (item is IDisposable)
						{
							var disposable = item as IDisposable;
							if (disposable != null)
							{
								disposable.Dispose();
							}
						}
					}
				}
			}

			if (allocation.Cache is IDisposable)
			{
				var disposable = allocation.Cache as IDisposable;
				if (disposable != null)
				{
					disposable.Dispose();
				}
			}

			Marshal.FreeHGlobal(pointer);
			pointer = IntPtr.Zero;
		}

		private static bool TryGetAllocationCache(IntPtr pointer, out object cache)
		{
			cache = null;

			lock (s_Allocations)
			{
				Allocation allocation;
				if (s_Allocations.TryGetValue((PointerType)pointer, out allocation))
				{
					cache = allocation.Cache;
					return true;
				}
			}

			return false;
		}

		private static IntPtr AddPinnedBuffer(byte[] buffer, int offset)
		{
			if (buffer == null)
			{
				return IntPtr.Zero;
			}

			GCHandle handle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
			PointerType pointer = (PointerType) Marshal.UnsafeAddrOfPinnedArrayElement(buffer, offset);

			lock (s_PinnedBuffers)
			{
				// If the item is already pinned, increase the reference count.
				if (s_PinnedBuffers.ContainsKey(pointer))
				{
					// Since this is a structure, need to copy to modify the element.
					PinnedBuffer pinned = s_PinnedBuffers[pointer];
					pinned.RefCount++;
					s_PinnedBuffers[pointer] = pinned;
				}
				else
				{
					s_PinnedBuffers.Add(pointer, new PinnedBuffer(handle));
				}

				return (IntPtr)pointer;
			}
		}

		private static IntPtr AddPinnedBuffer(Utf8String str)
		{
			if (str == null || str.Bytes == null)
			{
				return IntPtr.Zero;
			}

			return AddPinnedBuffer(str.Bytes, 0);
		}

		internal static IntPtr AddPinnedBuffer(ArraySegment<byte> array)
		{
			if (array == null)
			{
				return IntPtr.Zero;
			}

			return AddPinnedBuffer(array.Array, array.Offset);
		}

		internal static IntPtr AddPinnedBuffer(byte[] array)
		{
			if (array == null)
			{
				return IntPtr.Zero;
			}

			return AddPinnedBuffer(array, 0);
		}

		private static void RemovePinnedBuffer(ref IntPtr pointer)
		{
			if (pointer == IntPtr.Zero)
			{
				return;
			}

			lock (s_PinnedBuffers)
			{
				PinnedBuffer pinnedBuffer;
				PointerType pointerKey = (PointerType)pointer;
				if (s_PinnedBuffers.TryGetValue(pointerKey, out pinnedBuffer))
				{
					// Deref the allocation.
					pinnedBuffer.RefCount--;

					// If the reference count is zero, remove the allocation from the list of tracked allocations.
					if (pinnedBuffer.RefCount == 0)
					{
						s_PinnedBuffers.Remove(pointerKey);

						// We only call free on the handle when the last reference has been dropped.
						// Otherwise, the buffer is immediately unpinned despite the fact that there are still references to it.
						pinnedBuffer.Handle.Free();
					}
					else
					{
						// Copy back the structure with the decreased reference count.
						s_PinnedBuffers[pointerKey] = pinnedBuffer;
					}
				}
			}

			pointer = IntPtr.Zero;
		}
	}
}