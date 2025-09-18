// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.Linq;

namespace Epic.OnlineServices
{
	public sealed partial class Helper
	{
		/// <summary>
		/// Adds a callback to the wrapper.
		/// </summary>
		/// <param name="clientDataPointer">The generated client data pointer.</param>
		/// <param name="clientData">The client data of the callback.</param>
		/// <param name="delegates">The delegates to add.</param>
		internal static void AddCallback(out IntPtr clientDataPointer, object clientData, params Delegate[] delegates)
		{
			clientDataPointer = AddClientData(clientData);

			lock (s_Callbacks)
			{
				s_Callbacks.Add(clientDataPointer, new DelegateHolder(delegates));
			}
		}

		/// <summary>
		/// Adds a callback to the wrapper with an existing client data pointer.
		/// </summary>
		/// <param name="clientDataPointer">The client data pointer.</param>
		/// <param name="delegates">The delegates to add.</param>
		internal static void AddCallback(IntPtr clientDataPointer, params Delegate[] delegates)
		{
			lock (s_Callbacks)
			{
				DelegateHolder delegateHolder;
				if (s_Callbacks.TryGetValue(clientDataPointer, out delegateHolder))
				{
					delegateHolder.Delegates.AddRange(delegates.Where(d => d != null));
				}
			}
		}

		/// <summary>
		/// Removes a callback from the wrapper.
		/// </summary>
		/// <param name="clientDataPointer">The client data pointer of the callback.</param>
		internal static void RemoveCallback(IntPtr clientDataPointer)
		{
			lock (s_Callbacks)
			{
				s_Callbacks.Remove(clientDataPointer);
			}

			RemoveClientData(clientDataPointer);
		}

		/// <summary>
		/// Tries to get the callback associated with the given internal callback info.
		/// </summary>
		/// <typeparam name="TCallbackInfoInternal">The internal callback info type.</typeparam>
		/// <typeparam name="TCallback">The callback type.</typeparam>
		/// <typeparam name="TCallbackInfo">The callback info type.</typeparam>
		/// <param name="callbackInfoInternal">The internal callback info.</param>
		/// <param name="callback">The callback associated with the internal callback info.</param>
		/// <param name="callbackInfo">The callback info.</param>
		/// <returns>Whether the callback was successfully retrieved.</returns>
		internal static bool TryGetCallback<TCallbackInfoInternal, TCallback, TCallbackInfo>(ref TCallbackInfoInternal callbackInfoInternal, out TCallback callback, out TCallbackInfo callbackInfo)
			where TCallbackInfoInternal : struct, ICallbackInfoInternal, IGettable<TCallbackInfo>
			where TCallback : class
			where TCallbackInfo : struct, ICallbackInfo
		{
			IntPtr clientDataPointer;
			Get(ref callbackInfoInternal, out callbackInfo, out clientDataPointer);

			callback = null;

			lock (s_Callbacks)
			{
				DelegateHolder delegateHolder;
				if (s_Callbacks.TryGetValue(clientDataPointer, out delegateHolder))
				{
					callback = delegateHolder.Delegates.FirstOrDefault(d => d.GetType() == typeof(TCallback)) as TCallback;
					return callback != null;
				}
			}

			return false;
		}

		/// <summary>
		/// Tries to get the callback associated with the given internal callback info, and then removes it from the wrapper if applicable.
		/// Single-use callbacks will be cleaned up by this function.
		/// </summary>
		/// <typeparam name="TCallbackInfoInternal">The internal callback info type.</typeparam>
		/// <typeparam name="TCallback">The callback type.</typeparam>
		/// <typeparam name="TCallbackInfo">The callback info type.</typeparam>
		/// <param name="callbackInfoInternal">The internal callback info.</param>
		/// <param name="callback">The callback associated with the internal callback info.</param>
		/// <param name="callbackInfo">The callback info.</param>
		/// <returns>Whether the callback was successfully retrieved.</returns>
		internal static bool TryGetAndRemoveCallback<TCallbackInfoInternal, TCallback, TCallbackInfo>(ref TCallbackInfoInternal callbackInfoInternal, out TCallback callback, out TCallbackInfo callbackInfo)
			where TCallbackInfoInternal : struct, ICallbackInfoInternal, IGettable<TCallbackInfo>
			where TCallback : class
			where TCallbackInfo : struct, ICallbackInfo
		{
			IntPtr clientDataPointer;
			Get(ref callbackInfoInternal, out callbackInfo, out clientDataPointer);

			callback = null;
			ulong? notificationId = null;

			lock (s_Callbacks)
			{
				DelegateHolder delegateHolder;
				if (s_Callbacks.TryGetValue(clientDataPointer, out delegateHolder))
				{
					callback = delegateHolder.Delegates.FirstOrDefault(d => d.GetType() == typeof(TCallback)) as TCallback;
					notificationId = delegateHolder.NotificationId;
				}
			}

			if (callback != null)
			{
				// If this delegate was added with an AddNotify, we should only ever remove it on RemoveNotify.
				if (notificationId.HasValue)
				{
				}

				// If the operation is complete, it's safe to remove.
				else if (callbackInfo.GetResultCode().HasValue && Common.IsOperationComplete(callbackInfo.GetResultCode().Value))
				{
					RemoveCallback(clientDataPointer);
				}

				return true;
			}

			return false;
		}

		/// <summary>
		/// Tries to get the struct callback associated with the given internal callback info.
		/// </summary>
		/// <typeparam name="TCallbackInfoInternal">The internal callback info type.</typeparam>
		/// <typeparam name="TCallback">The callback type.</typeparam>
		/// <typeparam name="TCallbackInfo">The callback info type.</typeparam>
		/// <param name="callbackInfoInternal">The internal callback info.</param>
		/// <param name="callback">The callback associated with the internal callback info.</param>
		/// <param name="callbackInfo">The callback info.</param>
		/// <returns>Whether the callback was successfully retrieved.</returns>
		internal static bool TryGetStructCallback<TCallbackInfoInternal, TCallback, TCallbackInfo>(ref TCallbackInfoInternal callbackInfoInternal, out TCallback callback, out TCallbackInfo callbackInfo)
			where TCallbackInfoInternal : struct, ICallbackInfoInternal, IGettable<TCallbackInfo>
			where TCallback : class
			where TCallbackInfo : struct
		{
			IntPtr clientDataPointer;
			Get(ref callbackInfoInternal, out callbackInfo, out clientDataPointer);

			callback = null;
			lock (s_Callbacks)
			{
				DelegateHolder delegateHolder;
				if (s_Callbacks.TryGetValue(clientDataPointer, out delegateHolder))
				{
					callback = delegateHolder.Delegates.FirstOrDefault(d => d.GetType() == typeof(TCallback)) as TCallback;
					if (callback != null)
					{
						return true;
					}
				}
			}

			return false;
		}

		/// <summary>
		/// Removes a callback from the wrapper by an associated notification id.
		/// </summary>
		/// <param name="notificationId">The notification id associated with the callback.</param>
		internal static void RemoveCallbackByNotificationId(ulong notificationId)
		{
			IntPtr clientDataPointer = IntPtr.Zero;

			lock (s_Callbacks)
			{
				clientDataPointer = s_Callbacks.SingleOrDefault(pair => pair.Value.NotificationId.HasValue && pair.Value.NotificationId == notificationId).Key;
			}

			RemoveCallback(clientDataPointer);
		}

		/// <summary>
		/// Adds a static callback to the wrapper.
		/// </summary>
		/// <param name="key">The key of the callback.</param>
		/// <param name="publicDelegate">The public delegate of the callback.</param>
		/// <param name="privateDelegate">The private delegate of the callback</param>
		internal static void AddStaticCallback(string key, params Delegate[] delegates)
		{
			lock (s_StaticCallbacks)
			{
				s_StaticCallbacks.Remove(key);
				s_StaticCallbacks.Add(key, new DelegateHolder(delegates));
			}
		}

		/// <summary>
		/// Tries to get the static callback associated with the given key.
		/// </summary>
		/// <typeparam name="TCallback">The callback type.</typeparam>
		/// <param name="key">The key of the callback.</param>
		/// <param name="callback">The callback associated with the key.</param>
		/// <returns>Whether the callback was successfully retrieved.</returns>
		internal static bool TryGetStaticCallback<TCallback>(string key, out TCallback callback)
			where TCallback : class
		{
			callback = null;

			lock (s_StaticCallbacks)
			{
				DelegateHolder delegateHolder;
				if (s_StaticCallbacks.TryGetValue(key, out delegateHolder))
				{
					callback = delegateHolder.Delegates.FirstOrDefault(d => d.GetType() == typeof(TCallback)) as TCallback;
					if (callback != null)
					{
						return true;
					}
				}
			}

			return false;
		}

		/// <summary>
		/// Assigns a notification id to a callback by client data pointer associated with the callback.
		/// </summary>
		/// <param name="clientDataAddress">The client data address associated with the callback.</param>
		/// <param name="notificationId">The notification id to assign.</param>
		internal static void AssignNotificationIdToCallback(IntPtr clientDataPointer, ulong notificationId)
		{
			if (notificationId == 0)
			{
				RemoveCallback(clientDataPointer);
				return;
			}

			lock (s_Callbacks)
			{
				DelegateHolder delegateHolder;
				if (s_Callbacks.TryGetValue(clientDataPointer, out delegateHolder))
				{
					delegateHolder.NotificationId = notificationId;
				}
			}
		}

		/// <summary>
		/// Adds client data to the wrapper.
		/// </summary>
		/// <param name="clientData">The client data to add.</param>
		/// <returns>The pointer of the added client data.</returns>
		private static IntPtr AddClientData(object clientData)
		{
			lock (s_ClientDatas)
			{
				long clientDataId = ++s_LastClientDataId;
				IntPtr clientDataPointer = new IntPtr(clientDataId);
				s_ClientDatas.Add(clientDataPointer, clientData);
				return clientDataPointer;
			}
		}

		/// <summary>
		/// Removes a client data from the wrapper.
		/// </summary>
		/// <param name="clientDataPointer">The pointer of the client data to remove.</param>
		private static void RemoveClientData(IntPtr clientDataPointer)
		{
			lock (s_ClientDatas)
			{
				s_ClientDatas.Remove(clientDataPointer);
			}
		}

		/// <summary>
		/// Gets client data by its pointer.
		/// </summary>
		/// <param name="clientDataPointer">The pointer of the client data.</param>
		/// <returns>Th client data associated with the pointer.</returns>
		private static object GetClientData(IntPtr clientDataPointer)
		{
			lock (s_ClientDatas)
			{
				object clientData;
				s_ClientDatas.TryGetValue(clientDataPointer, out clientData);
				return clientData;
			}
		}
	}
}