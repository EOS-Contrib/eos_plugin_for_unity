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
    using Epic.OnlineServices;
    using UnityEngine;
    using UnityEngine.UI;
    using Epic.OnlineServices.Presence;

    public class UIHighFrequencyPeer2PeerMenu : SampleMenuWithFriends
    {
        [Header("Peer 2 Peer UI")]
        public GameObject ChatWindow;

        // UI
        public Button CloseChatButton;
        public Text NATTypeText;
        public Text CurrentChatUserText;

        public GameObject ChatEntriesContentParent;
        public GameObject ChatEntryPrefab;

        public UIConsoleInputField ChatMessageInput;
        public UIConsoleInputField ProductUserIdInput;
        public UIPeer2PeerParticleController ParticleManager;

        private EOSHighFrequencyPeer2PeerManager Peer2PeerManager;
        private EOSFriendsManager FriendsManager;

        private string currentChatDisplayName;
        private ProductUserId currentChatProductUserId;

        private Camera uiCamera;

        void Start()
        {
            Peer2PeerManager = EOSManager.Instance.GetOrCreateManager<EOSHighFrequencyPeer2PeerManager>();
            FriendsManager = EOSManager.Instance.GetOrCreateManager<EOSFriendsManager>();
            Peer2PeerManager.ParticleController = ParticleManager;
            Peer2PeerManager.owner = this;
            Peer2PeerManager.parent = this.transform;
            uiCamera = Camera.main;
        }

        protected override void OnDestroy()
        {
            base.OnDestroy();
            EOSManager.Instance.RemoveManager<EOSHighFrequencyPeer2PeerManager>();
            EOSManager.Instance.RemoveManager<EOSFriendsManager>();
        }

        public void ChatMessageEndEdit(string arg0)
        {
            SendOnClick();
        }

        public void ToggleHighFrequencySending()
        {
            if (Peer2PeerManager != null)
            {
                Peer2PeerManager.sendActive = !Peer2PeerManager.sendActive;
            }
        }
        protected override void Update()
        {
            base.Update();
            ProductUserId messageFromPlayer = Peer2PeerManager.HandleReceivedMessages();
            if (messageFromPlayer != null)
            {
                IncomingChat(messageFromPlayer);
            }

            if (Input.GetMouseButtonDown(0))
            {
                ParticlesOnClick();
            }

            if (Peer2PeerManager.sendActive)
            {
                Peer2PeerManager.P2PUpdate();
            }

        }

        public override FriendInteractionState GetFriendInteractionState(FriendData friendData)
        {
            return friendData.IsFriend() && friendData.IsOnline() ? FriendInteractionState.Enabled : FriendInteractionState.Hidden;
        }

        public override void OnFriendInteractButtonClicked(FriendData friendData)
        {
            ChatButtonOnClick(friendData.UserId);
        }

        public override string GetFriendInteractButtonText()
        {
            return "Chat";
        }

        public ProductUserId GetCurrentFriendId()
        {
            return currentChatProductUserId;
        }
        public void ChatButtonOnClick(EpicAccountId userId)
        {
            // Set Current chat

            FriendsManager.GetCachedFriends(out Dictionary<EpicAccountId, FriendData> friends);

            if (friends.TryGetValue(userId, out FriendData friend))
            {
                if (friend.UserProductUserId == null || !friend.UserProductUserId.IsValid())
                {
                    Debug.LogError("UIPeer2PeerMenu (ChatButtonOnClick): UserProductUserId is not valid!");
                    return;
                }

                currentChatDisplayName = friend.Name;
                currentChatProductUserId = friend.UserProductUserId;
                CurrentChatUserText.text = currentChatDisplayName;

                ForceInitialHFPacket();
            }
            else
            {
                Debug.LogError("UIPeer2PeerMenu (ChatButtonOnClick): Friend not found in cached data.");
            }
        }

        public void IncomingChat(ProductUserId productUserId)
        {
            if (!productUserId.IsValid())
            {
                Debug.LogError("UIPeer2PeerMenu (IncomingChat): productUserId is not valid!");
                return;
            }

            if (currentChatProductUserId == null)
            {
                // Open chat window if no window is open
                FriendsManager.GetCachedFriends(out Dictionary<EpicAccountId, FriendData> friends);

                string displayName = string.Empty;
                foreach (FriendData friend in friends.Values)
                {
                    if (productUserId == friend.UserProductUserId)
                    {
                        // Found friend
                        ChatButtonOnClick(friend.UserId);
                        return;
                    }
                }

                currentChatDisplayName = productUserId.ToString();
                currentChatProductUserId = productUserId;
                CurrentChatUserText.text = currentChatDisplayName;
            }
            else
            {
                // TODO: Show notification in friends list of new message from friend other than current chat window
            }
        }

        public void SetIdOnClick()
        {
            var productUserIdText = ProductUserIdInput.InputField.text;
            var productUserId = ProductUserId.FromString(productUserIdText);
            if (!productUserId.IsValid())
            {
                Debug.LogError("UIPeer2PeerMenu (SetIdOnClick): Invalid ProductUserId.");
                return;
            }
            currentChatDisplayName = productUserIdText;
            currentChatProductUserId = productUserId;
            CurrentChatUserText.text = productUserIdText;
        }

        public void SendOnClick()
        {
            if (string.IsNullOrEmpty(currentChatDisplayName) && currentChatProductUserId == null)
            {
                Debug.LogError("UIPeer2PeerMenu (SendOnClick): No chat window open.");
                return;
            }

            if (string.IsNullOrEmpty(ChatMessageInput.InputField.text))
            {
                Debug.LogError("UIPeer2PeerMenu (SendOnClick): Message is empty.");
                return;
            }
            messageData message;
            message.textData = ChatMessageInput.InputField.text;
            message.type = messageType.textMessage;
            message.xPos = 0;
            message.yPos = 0;

            if (!HasValidCurrentProductId())
            {
                return;
            }

            Peer2PeerManager.SendMessage(currentChatProductUserId, message.ToString());
            ChatMessageInput.InputField.text = string.Empty;
        }

        protected override void ShowInternal()
        {
            EOSManager.Instance.GetOrCreateManager<EOSPeer2PeerManager>().OnLoggedIn();

            var presenceInterface = EOSManager.Instance.GetEOSPresenceInterface();
            var presenceModificationOptions = new CreatePresenceModificationOptions();
            presenceModificationOptions.LocalUserId = EOSManager.Instance.GetLocalUserId();

            Result result = presenceInterface.CreatePresenceModification(ref presenceModificationOptions, out PresenceModification presenceModificationHandle);

            if (result == Result.Success)
            {

                //mark user as online
                var presenceModificationSetStatusOptions = new PresenceModificationSetStatusOptions();
                presenceModificationSetStatusOptions.Status = Status.Online;
                presenceModificationHandle.SetStatus(ref presenceModificationSetStatusOptions);

                var presenceModificationSetJoinOptions = new PresenceModificationSetJoinInfoOptions();

                presenceModificationSetJoinOptions.JoinInfo = "Custom Invite";
                presenceModificationHandle.SetJoinInfo(ref presenceModificationSetJoinOptions);

                // actually update all the status changes
                var setPresenceOptions = new Epic.OnlineServices.Presence.SetPresenceOptions();
                setPresenceOptions.LocalUserId = EOSManager.Instance.GetLocalUserId();
                setPresenceOptions.PresenceModificationHandle = presenceModificationHandle;
                presenceInterface.SetPresence(ref setPresenceOptions, null, (ref SetPresenceCallbackInfo data) => { });
                presenceModificationHandle.Release();
            }

        }

        public void SetRefreshRate(string hz)
        {
            if (string.IsNullOrEmpty(hz))
            {
                Debug.Log("Invalid value: empty or null input.");
                return;
            }

            bool attemptParse = int.TryParse(hz, out int refreshRate);

            if (attemptParse)
            {
                if (refreshRate < 0)
                {
                    Debug.Log("Invalid value (negative): " + hz);
                    return;
                }
                Peer2PeerManager.refreshRate = refreshRate;
                Debug.Log("Updated refresh rate to " + refreshRate + " Hz.");
            }
            else
            {
                Debug.Log("Invalid value: " + hz);
            }
        }

        public void SetPacketSize(string mb)
        {
            if (string.IsNullOrEmpty(mb))
            {
                Debug.Log("Invalid value: empty or null input.");
                return;
            }

            bool attemptParse = float.TryParse(mb, out float packetSizeMB);

            if (attemptParse)
            {
                if (packetSizeMB < 0)
                {
                    Debug.Log("Invalid value (negative): " + mb);
                    return;
                }
                Peer2PeerManager.packetSizeMB = packetSizeMB;
                Peer2PeerManager.updatePacketSize();
                Debug.Log("UIPeer2PeerMenu (SetPacketSize):Updated packet size to " + Peer2PeerManager.packetSizeMB + " Mb.");
            }
            else
            {
                Debug.Log("Invalid value: " + mb);
            }
        }

        protected override void HideInternal()
        {
            Peer2PeerManager?.OnLoggedOut();
        }

        public void ParticlesOnClick()
        {
            if (!HasValidCurrentProductId())
            {
                return;
            }
            Debug.Log($"{nameof(UIHighFrequencyPeer2PeerMenu)} {nameof(ParticlesOnClick)} Mouse click received");
            Vector3 mousePos = Input.mousePosition;
            Vector3 viewportPos = uiCamera.ScreenToViewportPoint(mousePos);
            messageData message;
            message.type = messageType.coordinatesMessage;
            message.xPos = viewportPos.x;
            message.yPos = viewportPos.y;
            message.textData = null;
            string coordinatePayload = EOSHighFrequencyPeer2PeerManager.CoordinateMessagePrefix + message.xPos + "," + message.yPos;
            Peer2PeerManager.SendMessage(currentChatProductUserId, coordinatePayload);
        }
        private bool HasValidCurrentProductId()
        {
            if (currentChatProductUserId == null || !currentChatProductUserId.IsValid())
            {
                Debug.LogError($"{nameof(UIHighFrequencyPeer2PeerMenu)} {nameof(HasValidCurrentProductId)}: ProductUserId for '{currentChatDisplayName}' is not valid!");
                return false;
            }
            
            return true;
        }
        private void ForceInitialHFPacket()
        {
            if (!HasValidCurrentProductId())
            {
                Debug.LogWarning($"{nameof(UIHighFrequencyPeer2PeerMenu)} {nameof(ForceInitialHFPacket)}: Cannot send initial HF packet. Invalid ProductUserId.");
                return;
            }

            Peer2PeerManager.SendMessage(
                currentChatProductUserId,
                "hf_init"
            );

        }
    }
}