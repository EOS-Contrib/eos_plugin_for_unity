namespace PlayEveryWare.EpicOnlineServices.Samples
{
    using UnityEngine;
    using UnityEngine.UI;

    public class UINativeLibVersion : MonoBehaviour
    {
        private void Start()
        {
            var textComp = GetComponent<Text>();
            if (textComp != null)
            {
                textComp.text = $"v-{EOSPackageInfo.NativeLibVersion}";
            }
        }
    }
}