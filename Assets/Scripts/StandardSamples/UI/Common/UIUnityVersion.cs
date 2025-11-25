namespace PlayEveryWare.EpicOnlineServices.Samples
{
    using UnityEngine;
    using UnityEngine.UI;

    public class UIUnityVersion : MonoBehaviour
    {
        private void Start()
        {
            var textComp = GetComponent<Text>();
            if (textComp != null)
            {
                textComp.text = $"v-{Application.unityVersion}";
            }
        }
    }
}