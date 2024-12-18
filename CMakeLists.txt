cmake_minimum_required(VERSION 3.15)
project(UnityPluginWithSampleApp)

# Allow users to specify the Unity version
set(UNITY_VERSION "2023.1.0f1" CACHE STRING "Specify the Unity version to use (e.g., 2023.1.0f1)")

# Path to ProjectSettings.asset
set(PROJECT_SETTINGS_FILE "${CMAKE_SOURCE_DIR}/SampleApplication/ProjectSettings/ProjectSettings.asset")

# Function to update the Unity version in ProjectSettings.asset
add_custom_target(UpdateUnityVersion
    COMMAND ${CMAKE_COMMAND} -E echo "Updating Unity version in ProjectSettings.asset to ${UNITY_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SETTINGS_FILE}" "${PROJECT_SETTINGS_FILE}.bak"
    COMMAND ${CMAKE_COMMAND} -E echo "Creating backup: ${PROJECT_SETTINGS_FILE}.bak"
    COMMAND ${CMAKE_COMMAND} -E touch "${PROJECT_SETTINGS_FILE}" # Ensure file exists
    COMMAND ${CMAKE_COMMAND} -E echo "Modifying m_EditorVersion field in ProjectSettings.asset..."
    COMMAND ${CMAKE_COMMAND} -E cmake_replace_regex "${PROJECT_SETTINGS_FILE}" "m_EditorVersion: .*" "m_EditorVersion: ${UNITY_VERSION}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Setting Unity project to use version ${UNITY_VERSION}"
)

# Custom target to open Unity project after setting the version
add_custom_target(OpenUnityProjectAfterUpdate
    COMMAND ${CMAKE_COMMAND} --build . --target UpdateUnityVersion
    COMMAND "${UNITY_EXECUTABLE_PATH}" -projectPath "${CMAKE_SOURCE_DIR}/SampleApplication" -logFile -
    COMMENT "Opening Unity project after updating version in ProjectSettings.asset"
)
