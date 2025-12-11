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
    using System;
    using System.Collections.Generic;

    using UnityEngine;
    using UnityEngine.EventSystems;
    using UnityEngine.UI;

    using Epic.OnlineServices;
    using Epic.OnlineServices.Reports;

    public class UIPlayerReportMenu : SampleMenuWithFriends
    {
        [Header("Reports")]
        public Text PlayerName;
        public Dropdown CategoryList;
        public InputField Message;

        [Header("Sanctions")]
        public GameObject SanctionsListContentParent;
        public GameObject UISanctionsEntryPrefab;

        private ProductUserId currentProductUserId;

        private EOSReportsManager ReportsManager;
        private EOSFriendsManager FriendsManager;
        
        private const string DateFormat="dd/MM/yyyy HH:mm";
        
        protected override void Awake()
        {
            base.Awake();
            ReportsManager = EOSManager.Instance.GetOrCreateManager<EOSReportsManager>();
            FriendsManager = EOSManager.Instance.GetOrCreateManager<EOSFriendsManager>();
        }

        protected override void OnDestroy()
        {
            EOSManager.Instance.RemoveManager<EOSReportsManager>();
            EOSManager.Instance.RemoveManager<EOSFriendsManager>();
            base.OnDestroy();
        }

        public void ReportButtonOnClick(ProductUserId userId, string playerName)
        {
            if(userId == null)
            {
                Debug.LogError("UIPlayerReportMenu (ReportButtonOnClick): ProductUserId is null!");
                return;
            }    

            PlayerName.text = playerName;
            currentProductUserId = userId;
            UIParent.SetActive(true);

            if (UIFirstSelected.activeInHierarchy)
            {
                EventSystem.current.SetSelectedGameObject(UIFirstSelected);
            }

            ReportsManager.QueryActivePlayerSanctions(userId, (result) =>
            {
                QueryActivePlayerSanctionsCompleted(result, userId);
            });

        }

        public void PlayerSanctionsRefreshOnClick()
        {
            // Start Search for Sanctions
            var targetUserId = currentProductUserId;
            ReportsManager.QueryActivePlayerSanctions(targetUserId, (result) =>
            {
                QueryActivePlayerSanctionsCompleted(result, targetUserId);
            });

        }

        //Display only sanctions, reports are not visible
        private void QueryActivePlayerSanctionsCompleted(Result result, ProductUserId userId)
        {
            if(result != Result.Success)
            {
                Debug.LogError($"{nameof(UIPlayerReportMenu)} {nameof(QueryActivePlayerSanctionsCompleted)}: Query failed with result: {result}");
                return;
            }

            foreach (Transform child in SanctionsListContentParent.transform)
            {
                GameObject.Destroy(child.gameObject);
            }

            if (!ReportsManager.GetCachedPlayerSanctions(out Dictionary<ProductUserId, List<Sanction>> sanctionLookup)
                || sanctionLookup == null || !sanctionLookup.TryGetValue(userId, out var sanctions) || sanctions == null || sanctions.Count == 0)
            {
                CreateSanctionEntry(string.Empty, "No Sanctions Found.");
                return;
            }

            foreach (var s in sanctions)
            {
                var timeText = s.TimePlaced.ToString(DateFormat); 
                CreateSanctionEntry(timeText, s.Action);
            }

        }

        // Helper method to create an entry in the sanctions list
        private void CreateSanctionEntry(string timeText, string actionText)
        {
            var sanctionUIObj = Instantiate(UISanctionsEntryPrefab, SanctionsListContentParent.transform);
            var uiEntry = sanctionUIObj.GetComponent<UISanctionEntry>();

            uiEntry.TimePlaced.text = timeText ?? string.Empty;
            uiEntry.Action.text = actionText ?? string.Empty;
        }

        public void SubmitReportButtonOnClick()
        {
            if(currentProductUserId == null || !currentProductUserId.IsValid())
            {
                Debug.LogError("UIPlayerReportMenu (ReportButtonOnClick): ProductUserId is not valid!");
                return;
            }

            string categoryStr = CategoryList.options[CategoryList.value].text;
            PlayerReportsCategory category = PlayerReportsCategory.Invalid;
            var categoryParsed = Enum.Parse(typeof(PlayerReportsCategory), categoryStr);
            if(categoryParsed != null)
            {
                category = (PlayerReportsCategory)categoryParsed;
            }

            if (ReportsManager != null)
            {
                ReportsManager.SendPlayerBehaviorReport(currentProductUserId, category, Message.text);
                ResetPopUp();
            }
        }

        public void CancelButtonOnClick()
        {
            ResetPopUp();
        }

        private void ResetPopUp()
        {
            PlayerName.text = string.Empty;
            CategoryList.value = 0;
            Message.text = string.Empty;
            currentProductUserId = null;
            UIParent.SetActive(false);
        }

        public override FriendInteractionState GetFriendInteractionState(FriendData friendData)
        {
            return FriendInteractionState.Enabled;
        }

        public override string GetFriendInteractButtonText()
        {
            return "Report";
        }

        public override void OnFriendInteractButtonClicked(FriendData friendData)
        {
            ReportButtonOnClick(friendData.UserProductUserId, friendData.Name);
        }

        protected override void ShowInternal()
        {
            ResetPopUp();
            UIParent.SetActive(true);
        }

        protected override void HideInternal()
        {
            ResetPopUp();
        }
    }
}
