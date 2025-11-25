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
    using System.Collections.Generic;
    using System.Linq;
    using UnityEngine;
    using UnityEngine.UI;
    using UnityEngine.EventSystems;
    using Epic.OnlineServices.Ecom;
    public class UIStoreMenu : SampleMenu
    {
        [Header("Store UI")]
        public Button queryOffersButton;

        public Text catalogueItem0;
        public Button checkOutButton0;

        public Text catalogueItem1;
        public Button checkOutButton1;

        [Header("Entitlements")]
        public Text entitlementsListText;

        [Header("Ownership")]
        public Text ownershipListText;

        [Tooltip("CatalogItemIds in SPT")]
        public string[] durableCatalogItemIds;

        private EOSStoreManager StoreManager;

        protected override void OnEnable()
        {
            base.OnEnable();
            StoreManager = EOSManager.Instance.GetOrCreateManager<EOSStoreManager>();
            checkOutButton0.onClick.AddListener(() => CheckOutButton(0));
            checkOutButton1.onClick.AddListener(() => CheckOutButton(1));
        }

        protected override void OnDestroy()
        {
            EOSManager.Instance.RemoveManager<EOSStoreManager>();
            base.OnDestroy();
        }

        protected override void Update()
        {
            base.Update();
            if (StoreManager.GetCatalogOffers(out List<CatalogOffer> CatalogOffers))
            {
                // Generate UI for offers
                // Hard-code for demo
                if (CatalogOffers.Count > 0)
                {
                    catalogueItem0.text = string.Format("{0}, ${1}", CatalogOffers[0].TitleText, StoreManager.GetCurrentPriceAsString(CatalogOffers[0]));
                }

                if (CatalogOffers.Count > 1)
                {
                    catalogueItem1.text = string.Format("{0}, ${1}", CatalogOffers[1].TitleText, StoreManager.GetCurrentPriceAsString(CatalogOffers[1]));
                }
            }
            // Entitlements
            if (StoreManager.GetEntitlements(out var ents))
            {
                if (entitlementsListText != null)
                {
                    if (ents.Count == 0)
                        entitlementsListText.text = "No entitlements.";
                    else
                    {
                        System.Text.StringBuilder sb = new System.Text.StringBuilder();
                        foreach (var e in ents)
                        {
                            sb.AppendLine($"{e.EntitlementName}  | ItemId: {e.CatalogItemId} | Redeemed: {e.Redeemed}");
                        }
                        entitlementsListText.text = sb.ToString();
                    }
                }
            }
            // Ownership
            if (StoreManager.GetOwnedDurables(out var ownedIds))
            {
                if (ownershipListText != null)
                {
                    if (ownedIds.Count == 0)
                        ownershipListText.text = "No durable items owned.";
                    else
                        ownershipListText.text = string.Join("\n", ownedIds);
                }
            }
        }

        public void OnQueryOffersClick()
        {
            print("OnQueryOffersClick: IsValid=" + EOSManager.Instance.GetLocalUserId().IsValid() + ", accountId" + EOSManager.Instance.GetLocalUserId().ToString());
            StoreManager.QueryOffers();
        }

        public void OnQueryEntitlementsClick()
        {
            Debug.Log("[UI] QueryEntitlements CLICKED.");
            StoreManager.QueryEntitlements(includeRedeemed: true);
        }

        public void OnQueryOwnershipClick()
        {
            var fromOffers = StoreManager.DurableItemIdsFromOffers;
            string[] ids = (fromOffers != null && fromOffers.Count > 0)
                ? fromOffers.ToArray()
                : (durableCatalogItemIds ?? System.Array.Empty<string>());

            if (ids.Length == 0)
            {
                Debug.LogWarning("[UI] No durable IDs.");
                return;
            }
            StoreManager.QueryOwnership(ids);
        }

        public void CheckOutButton(int index)
        {
            StoreManager.CheckOutOverlay(index);
        }

        public void QueryOwnershipFromOfferIndex(int offerIndex)
        {
            var hasOffers = StoreManager.GetCatalogOffers(out var offers);
            if (!hasOffers || offerIndex < 0 || offerIndex >= offers.Count)
            {
                Debug.LogWarning($"[UI] Invalid offer index {offerIndex} or no offers loaded.");
                return;
            }

            var offer = offers[offerIndex];

            // Step 1: Find entitlements associated with this offer
            if (!StoreManager.GetEntitlements(out var entitlements))
            {
                Debug.LogWarning("[UI] No entitlements in cache yet, must call QueryEntitlements first.");
                return;
            }

            List<string> itemIds = new List<string>();

            foreach (var ent in entitlements)
            {
                // Match via CatalogItemId or via OfferId metadata
                if (ent.CatalogItemId != null && ent.CatalogItemId.Length > 0)
                    itemIds.Add(ent.CatalogItemId);
            }

            if (itemIds.Count == 0)
            {
                Debug.LogWarning($"[UI] No entitlements related to Offer {offer.Id}. Player likely does not own any items from this offer.");
                return;
            }

            Debug.Log($"[UI] QueryOwnershipFromOfferIndex({offerIndex}) → {string.Join(", ", itemIds)}");
            StoreManager.QueryOwnership(itemIds.ToArray());
        }

        public void QueryEntitlementsFromOfferIndex(int offerIndex)
        {
            var hasOffers = StoreManager.GetCatalogOffers(out var offers);
            if (!hasOffers || offerIndex < 0 || offerIndex >= offers.Count)
            {
                Debug.LogWarning($"[UI] Invalid offer index {offerIndex} or no offers loaded.");
                return;
            }

            Debug.Log($"[UI] QueryEntitlementsFromOfferIndex({offerIndex}) → Will refresh entitlements.");
            StoreManager.QueryEntitlements(includeRedeemed: true);
        }

        public void OnRedeemAllPendingClick()
        {
            if (!StoreManager.GetEntitlements(out var ents) || ents.Count == 0)
            {
                return;
            }

            var toRedeem = ents
                .Where(e => !e.Redeemed)
                .Select(e => (string)e.EntitlementId)
                .ToArray();

            if (toRedeem.Length > 0)
            {
                StoreManager.RedeemEntitlements(toRedeem);
            }
            else
            {
                Debug.Log("[UI] No pending entitlements to redeem.");
            }

        }
    }
}