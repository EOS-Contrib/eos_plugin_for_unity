cmake_minimum_required(VERSION 3.15)

project(ConfigureUnityVersion)

# Allow users to specify the Unity version
set(UNITY_VERSION "2023.1.0f1" CACHE STRING "Specify the Unity version to use (e.g., 2023.1.0f1)")

# Path to ProjectSettings.asset
set(PROJECT_SETTINGS_FILE "${CMAKE_SOURCE_DIR}/ProjectSettings/ProjectSettings.asset")

if (NOT EXISTS "${PROJECT_SETTINGS_FILE}")
    message(FATAL_ERROR "ProjectSettings.asset file not found at: ${PROJECT_SETTINGS_FILE}")
endif()

# Command to update the Unity version
execute_process(
    COMMAND ${CMAKE_COMMAND} -E echo "Updating Unity version in ProjectSettings.asset to ${UNITY_VERSION}"
)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SETTINGS_FILE}" "${PROJECT_SETTINGS_FILE}.bak"
)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E echo "m_EditorVersion: ${UNITY_VERSION}" >> "${PROJECT_SETTINGS_FILE}"
)
