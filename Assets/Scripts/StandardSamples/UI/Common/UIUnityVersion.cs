namespace PlayEveryWare.EpicOnlineServices.Samples
{
    using UnityEngine;
    using UnityEngine.UI;

    public class UIUnityVersion : MonoBehaviour
    {
        private void Start()
        {
            var textComp = GetComponent<Text>();
            textComp.text = $"v-{Application.unityVersion}";
        }
    }
}