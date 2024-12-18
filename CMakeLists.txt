# Minimum required CMake version
cmake_minimum_required(VERSION 3.15)

project(UnityProjectSetup)

# Default Unity version
set(DEFAULT_UNITY_VERSION "2021.3.16f1")

# Use the provided UNITY_VERSION or default
if (NOT DEFINED UNITY_VERSION)
    set(UNITY_VERSION "${DEFAULT_UNITY_VERSION}")
    message(STATUS "UNITY_VERSION is not specified. Defaulting to ${UNITY_VERSION}.")
else()
    message(STATUS "Using Unity version: ${UNITY_VERSION}")
endif()

# Path to the UpdateUnityVersion.cmake script
set(UPDATE_SCRIPT "${CMAKE_SOURCE_DIR}/cmake/UpdateUnityVersion.cmake")

# Ensure the script exists
if (NOT EXISTS "${UPDATE_SCRIPT}")
    message(FATAL_ERROR "The CMake script ${UPDATE_SCRIPT} does not exist.")
endif()

# Call the script directly (no build setup)
execute_process(
    COMMAND ${CMAKE_COMMAND} -DUNITY_VERSION=${UNITY_VERSION} -P "${UPDATE_SCRIPT}"
    RESULT_VARIABLE RESULT
)

# Check if the script ran successfully
if (NOT "${RESULT}" STREQUAL "0")
    message(FATAL_ERROR "Failed to execute ${UPDATE_SCRIPT}.")
endif()

message(STATUS "Executed ${UPDATE_SCRIPT} successfully.")
