/*
* Copyright (c) 2021 PlayEveryWare
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

namespace PlayEveryWare.EpicOnlineServices.Samples
{
    using System.Linq;
    using System.Collections.Generic;
    using UnityEngine;
    using Epic.OnlineServices;
    using Epic.OnlineServices.Ecom;
    using Epic.OnlineServices.Auth;
    
    /// <summary>
    /// Class <c>EOSStoreManager</c> is a simplified wrapper for EOS [Ecom Interface](https://dev.epicgames.com/docs/services/en-US/Interfaces/Ecom/index.html).
    /// </summary>

    public class EOSStoreManager : IEOSSubManager, IAuthInterfaceEventListener
    {
        private List<CatalogOffer> CatalogOffers;
        private bool CatalogOffersDirty;
  
        private List<Entitlement> _entitlements = new List<Entitlement>();
        private bool _entitlementsDirty = false;
       
        private List<string> _ownedDurableItemIds = new List<string>();
        private bool _ownershipDirty = false;

        private readonly List<string> _durableItemIdsFromOffers = new();
        public IReadOnlyList<string> DurableItemIdsFromOffers => _durableItemIdsFromOffers;

        public EOSStoreManager()
        {
            CatalogOffers = new List<CatalogOffer>();
            CatalogOffersDirty = false;
        }
       
        public void OnLoggedIn()
        {
            QueryOffers();
        }

#if !EOS_DISABLE
        public void OnAuthLogin(LoginCallbackInfo loginCallbackInfo)
        {
            OnLoggedIn();
        }

        public void OnAuthLogout(LogoutCallbackInfo logoutCallbackInfo)
        {
            OnLoggedOut();
        }
#endif

        public void OnLoggedOut()
        {
            CatalogOffers.Clear();
            CatalogOffersDirty = true;

            _entitlements.Clear();
            _entitlementsDirty = true;

            _ownedDurableItemIds.Clear();
            _ownershipDirty = true;

        }

        public bool GetCatalogOffers(out List<CatalogOffer> CatalogOffers)
        {
            CatalogOffers = this.CatalogOffers;
            return CatalogOffersDirty;
        }

        public bool GetEntitlements(out List<Entitlement> entitlements)
        {
            entitlements = _entitlements;
            return _entitlementsDirty;
        }

        public bool GetOwnedDurables(out List<string> ownedItemIds)
        {
            ownedItemIds = _ownedDurableItemIds;
            return _ownershipDirty;
        }

        private void BuildItemCachesFromOffers()
        {
            _durableItemIdsFromOffers.Clear();

            var ecom = EOSManager.Instance.GetEOSEcomInterface();
            var uid = EOSManager.Instance.GetLocalUserId();

            for (int offerIndex = 0; offerIndex < CatalogOffers.Count; offerIndex++)
            {
                var offer = CatalogOffers[offerIndex];

                var itemCountOptions = new GetOfferItemCountOptions
                {
                    LocalUserId = uid,
                    OfferId = offer.Id
                };

                uint itemCount = ecom.GetOfferItemCount(ref itemCountOptions);

                for (uint itemIndex = 0; itemIndex < itemCount; itemIndex++)
                {
                    var copyItemOptions = new CopyOfferItemByIndexOptions
                    {
                        LocalUserId = uid,
                        OfferId = offer.Id,
                        ItemIndex = itemIndex
                    };

                    var result = ecom.CopyOfferItemByIndex(ref copyItemOptions, out CatalogItem? item);

                    if (result == Epic.OnlineServices.Result.Success && item.HasValue)
                    {
                        if (item.Value.ItemType == EcomItemType.Durable)
                        {
                            _durableItemIdsFromOffers.Add(item.Value.Id.ToString());
                        }
                    }
                }
            }
        }

        public void QueryEntitlements(bool includeRedeemed = true)
        {
            var uid = EOSManager.Instance.GetLocalUserId();

            Debug.Log(
                $"[StoreManager] QueryEntitlements() → LocalUserId: {uid}, IncludeRedeemed: {includeRedeemed}"
            );

            var options = new QueryEntitlementsOptions
            {
                LocalUserId = uid,
                EntitlementNames = null,
                IncludeRedeemed = includeRedeemed
            };

            EOSManager.Instance
                .GetEOSEcomInterface()
                .QueryEntitlements(ref options, null, OnQueryEntitlements);
        }

        private void OnQueryEntitlements(ref QueryEntitlementsCallbackInfo info)
        {
            Debug.Log($"[StoreManager] OnQueryEntitlements() → Result: {info.ResultCode}");

            _entitlements.Clear();

            if (info.ResultCode != Result.Success)
            {
                Debug.LogError($"[StoreManager] QueryEntitlements FAILED → {info.ResultCode}");
                return;
            }

            var uid = EOSManager.Instance.GetLocalUserId();
            var ecom = EOSManager.Instance.GetEOSEcomInterface();

            var countOptions = new GetEntitlementsCountOptions
            {
                LocalUserId = uid
            };

            uint count = ecom.GetEntitlementsCount(ref countOptions);

            Debug.Log($"[StoreManager] Entitlements Count: {count}");

            for (uint index = 0; index < count; index++)
            {
                var copyOptions = new CopyEntitlementByIndexOptions
                {
                    LocalUserId = uid,
                    EntitlementIndex = index
                };

                var result = ecom.CopyEntitlementByIndex(ref copyOptions, out Entitlement? entitlement);

                if (result == Result.Success && entitlement.HasValue)
                {
                    Debug.Log(
                        $"  - [{index}] Name={entitlement.Value.EntitlementName}, " +
                        $"ItemId={entitlement.Value.CatalogItemId}, Redeemed={entitlement.Value.Redeemed}"
                    );

                    _entitlements.Add(entitlement.Value);
                }
                else
                {
                    Debug.LogWarning(
                        $"  - CopyEntitlementByIndex FAILED at index {index}: {result}"
                    );
                }
            }

            _entitlementsDirty = true;
        }

        public void QueryOwnership(string[] catalogItemIds)
        {
            var uid = EOSManager.Instance.GetLocalUserId();

            Debug.Log(
                $"[StoreManager] QueryOwnership() → LocalUserId: {uid}, RequestedIds: {string.Join(", ", catalogItemIds)}"
            );

            if (catalogItemIds == null || catalogItemIds.Length == 0)
            {
                Debug.LogWarning("[StoreManager] QueryOwnership called but CatalogItemIds list is EMPTY.");
                _ownedDurableItemIds.Clear();
                _ownershipDirty = true;
                return;
            }

            // Convert strings to Utf8String[]
            var utf8Ids = catalogItemIds
                .Select(id => (Epic.OnlineServices.Utf8String)id)
                .ToArray();

            var options = new QueryOwnershipOptions
            {
                LocalUserId = uid,
                CatalogItemIds = utf8Ids
            };

            EOSManager.Instance
                .GetEOSEcomInterface()
                .QueryOwnership(ref options, null, OnQueryOwnership);
        }

        private void OnQueryOwnership(ref QueryOwnershipCallbackInfo info)
        {
            Debug.Log($"[StoreManager] OnQueryOwnership() → ResultCode: {info.ResultCode}");

            _ownedDurableItemIds.Clear();

            if (info.ResultCode != Result.Success)
            {
                Debug.LogError($"[StoreManager] QueryOwnership FAILED → {info.ResultCode}");
                return;
            }

            if (info.ItemOwnership == null || info.ItemOwnership.Length == 0)
            {
                Debug.Log("[StoreManager] QueryOwnership → No items returned.");
                _ownershipDirty = true;
                return;
            }

            foreach (var item in info.ItemOwnership)
            {
                Debug.Log(
                    $"  - ItemId={item.Id}, Status={item.OwnershipStatus}"
                );

                if (item.OwnershipStatus == OwnershipStatus.Owned)
                {
                    _ownedDurableItemIds.Add(item.Id.ToString());
                }
            }

            _ownershipDirty = true;
        }
        
        public void QueryOffers()
        {
            var queryOfferOptions = new QueryOffersOptions();
            queryOfferOptions.LocalUserId = EOSManager.Instance.GetLocalUserId();
            queryOfferOptions.OverrideCatalogNamespace = null;
            EOSManager.Instance.GetEOSEcomInterface().QueryOffers(ref queryOfferOptions, null, OnQueryOffers);
        }

        private void OnQueryOffers(ref QueryOffersCallbackInfo info)
        {
            CatalogOffers.Clear();

            Debug.Log($"[StoreManager] OnQueryOffers() → ResultCode: {info.ResultCode}");

            if (info.ResultCode != Result.Success)
            {
                Debug.LogError($"[StoreManager] QueryOffers FAILED → {info.ResultCode}");
                BuildItemCachesFromOffers();
                return;
            }

            var uid = EOSManager.Instance.GetLocalUserId();
            var ecom = EOSManager.Instance.GetEOSEcomInterface();

            var countOptions = new GetOfferCountOptions
            {
                LocalUserId = uid
            };

            uint offerCount = ecom.GetOfferCount(ref countOptions);

            Debug.Log($"[StoreManager] QueryOffers → {offerCount} offer(s) found.");

            for (uint offerIndex = 0; offerIndex < offerCount; offerIndex++)
            {
                var copyOptions = new CopyOfferByIndexOptions
                {
                    LocalUserId = uid,
                    OfferIndex = offerIndex
                };

                var result = ecom.CopyOfferByIndex(ref copyOptions, out CatalogOffer? catalogOffer);

                switch (result)
                {
                    case Result.Success:
                    case Result.EcomCatalogOfferPriceInvalid:
                    case Result.EcomCatalogOfferStale:
                    {
                        Debug.Log(
                            $"  - Offer[{offerIndex}] → {result}, " +
                            $"Id={catalogOffer?.Id}, " +
                            $"Title={catalogOffer?.TitleText}, " +
                            $"PriceResult={catalogOffer?.PriceResult}, " +
                            $"Current={GetCurrentPriceAsString(catalogOffer)}, " +
                            $"Original={GetOriginalPriceAsString(catalogOffer)}"
                        );

                        if (catalogOffer.HasValue)
                        {
                            CatalogOffers.Add(catalogOffer.Value);
                        }
                        break;
                    }

                    default:
                    {
                        Debug.LogWarning(
                            $"  - Offer[{offerIndex}] invalid → {result}"
                        );
                        break;
                    }
                }
            }

            CatalogOffersDirty = true;
            BuildItemCachesFromOffers();
        }
        
        public void CheckOutOverlay(int index)
        {
        #if UNITY_EDITOR
            Debug.LogError(
                "[StoreManager] CheckOutOverlay() → Cannot display checkout overlay in the Unity Editor."
            );
        #endif

            CheckoutEntry checkoutEntry = new CheckoutEntry();
            checkoutEntry.OfferId = CatalogOffers[index].Id;
            CheckoutOptions checkoutOptions = new CheckoutOptions();
            checkoutOptions.LocalUserId = EOSManager.Instance.GetLocalUserId();
            checkoutOptions.Entries = new CheckoutEntry[] { checkoutEntry };
            EOSManager.Instance.GetEOSEcomInterface().Checkout(ref checkoutOptions, null, OnCheckout);
        }
        
        public void OnCheckout(ref CheckoutCallbackInfo checkoutCallbackInfo)
        {
            Debug.Log($"Checkout {checkoutCallbackInfo.ResultCode}");
            if (checkoutCallbackInfo.ResultCode == Epic.OnlineServices.Result.Success)
            {
                //Refresh entitlement and ownership views for the AC
                QueryEntitlements(includeRedeemed: true);
                if (DurableItemIdsFromOffers.Count > 0)
                    QueryOwnership(DurableItemIdsFromOffers.ToArray());
            }
        }
        //-------------------------------------------------------------------------
        // Wrapper to handle API differences in EOS 1.12 vs 1.11
        public string GetCurrentPriceAsString(CatalogOffer catalogOffer)
        {
            return string.Format("{0}", catalogOffer.CurrentPrice64);
        }

        //-------------------------------------------------------------------------
        // Wrapper to handle API differences in EOS 1.12 vs 1.11
        public string GetCurrentPriceAsString(CatalogOffer? catalogOffer)
        {
            return string.Format("{0}", catalogOffer?.CurrentPrice64);
        }

        //-------------------------------------------------------------------------
        // Wrapper to handle API differences in EOS 1.12 vs 1.11
        public string GetOriginalPriceAsString(CatalogOffer catalogOffer)
        {
            return string.Format("{0}", catalogOffer.OriginalPrice64);
        }

        //-------------------------------------------------------------------------
        // Wrapper to handle API differences in EOS 1.12 vs 1.11
        public string GetOriginalPriceAsString(CatalogOffer? catalogOffer)
        {
            return string.Format("{0}", catalogOffer?.OriginalPrice64);
        }
        public void RedeemEntitlements(IEnumerable<string> entitlementIds)
        {
            var uid = EOSManager.Instance.GetLocalUserId();

            if (!uid.IsValid())
            {
                Debug.LogWarning("[StoreManager] RedeemEntitlements() → LocalUserId is invalid.");
                return;
            }

            var idsArray = entitlementIds?
                .Select(x => (Epic.OnlineServices.Utf8String)x).ToArray()?? 
                System.Array.Empty<Epic.OnlineServices.Utf8String>();

            var options = new Epic.OnlineServices.Ecom.RedeemEntitlementsOptions
            {
                LocalUserId = uid,
                EntitlementIds = idsArray
            };

            Debug.Log($"[StoreManager] RedeemEntitlements() → Redeeming {idsArray.Length} entitlement(s).");

            EOSManager.Instance
                .GetEOSEcomInterface()
                .RedeemEntitlements(ref options, null, (ref Epic.OnlineServices.Ecom.RedeemEntitlementsCallbackInfo info) =>
                {
                    Debug.Log($"[StoreManager] RedeemEntitlements callback → Result: {info.ResultCode}");

                    // Always refresh entitlements after redeeming
                    QueryEntitlements(includeRedeemed: true);
                });
        }
    }
}