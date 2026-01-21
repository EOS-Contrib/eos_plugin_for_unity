/*
 -Copyright-
 */

namespace PlayEveryWare.EpicOnlineServices.Samples
{
    using UnityEngine;
    using UnityEngine.UI;

#if ENABLE_INPUT_SYSTEM
    using UnityEngine.InputSystem;
#endif

    /// <summary>
    /// Controls the sidebar menu UI behavior.
    /// </summary>

    public class UISideBarMenu : MonoBehaviour
    {
        /// <summary>
        /// Reference to the Exit button in the UI.
        /// </summary>
        public Button ExitButton;

        private void Awake()
        {
            // On some platforms, the application cannot be quit manually,
            // so hide the Exit button to avoid exposing unsupported functionality.
            ExitButton.gameObject.SetActive(EOSManagerPlatformSpecificsSingleton.Instance.CanShowExitButton());
        }

        /// <summary>
        /// Triggered when the Exit button is clicked.
        /// Quits the application in standalone builds and exits play mode in the Unity Editor.
        /// </summary>
        public void OnExitButtonClick()
        {
            Application.Quit();

#if UNITY_EDITOR
            UnityEditor.EditorApplication.ExitPlaymode();
#endif
        }
    }
}
