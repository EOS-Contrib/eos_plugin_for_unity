# Allow users to specify the Unity version
if (NOT DEFINED UNITY_VERSION)
    set(UNITY_VERSION "2021.3.16f1") # Default Unity version
endif()

# Path to ProjectSettings.asset
set(PROJECT_SETTINGS_FILE "${CMAKE_CURRENT_LIST_DIR}/ProjectSettings/ProjectSettings.asset")

# Check if the ProjectSettings.asset file exists
if (NOT EXISTS "${PROJECT_SETTINGS_FILE}")
    message(FATAL_ERROR "ProjectSettings.asset file not found at: ${PROJECT_SETTINGS_FILE}")
endif()

# Create a backup of the ProjectSettings.asset file
execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SETTINGS_FILE}" "${PROJECT_SETTINGS_FILE}.bak"
    COMMAND_ECHO STDOUT
)

# Update the Unity version in ProjectSettings.asset
file(READ "${PROJECT_SETTINGS_FILE}" PROJECT_SETTINGS_CONTENTS)
string(REGEX REPLACE "m_EditorVersion: .*" "m_EditorVersion: ${UNITY_VERSION}" UPDATED_CONTENTS "${PROJECT_SETTINGS_CONTENTS}")
file(WRITE "${PROJECT_SETTINGS_FILE}" "${UPDATED_CONTENTS}")

message(STATUS "Updated Unity version to ${UNITY_VERSION} in ProjectSettings.asset")
