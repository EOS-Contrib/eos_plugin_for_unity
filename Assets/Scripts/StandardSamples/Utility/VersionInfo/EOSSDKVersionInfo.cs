namespace PlayEveryWare.EpicOnlineServices.Samples
{
    using UnityEngine;
    using UnityEngine.UI;
    public class EOSSDKVersionInfo : MonoBehaviour
    {
        public Text pluginVersion;

        void Start()
        {
            pluginVersion.text = Epic.OnlineServices.Version.VersionInterface.GetVersion();
        }
    }
}