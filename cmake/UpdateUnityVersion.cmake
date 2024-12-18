# Define the default Unity version
set(DEFAULT_UNITY_VERSION "2021.3.16f1")

# Use the default Unity version if UNITY_VERSION is not specified
if (NOT DEFINED UNITY_VERSION)
    set(UNITY_VERSION "${DEFAULT_UNITY_VERSION}")
    message(STATUS "UNITY_VERSION is not specified. Defaulting to ${UNITY_VERSION}.")
endif()

# Path to ProjectVersion.txt
set(PROJECT_VERSION_FILE "${CMAKE_SOURCE_DIR}/ProjectSettings/ProjectVersion.txt")

# Check if the ProjectVersion.txt file exists
if (NOT EXISTS "${PROJECT_VERSION_FILE}")
    message(FATAL_ERROR "ProjectVersion.txt file not found at: ${PROJECT_VERSION_FILE}")
endif()

# Read the content of ProjectVersion.txt
file(READ "${PROJECT_VERSION_FILE}" PROJECT_VERSION_CONTENTS)

# Split the file into lines
string(REPLACE "\n" ";" PROJECT_VERSION_LINES "${PROJECT_VERSION_CONTENTS}")

# Prepare updated content
set(UPDATED_CONTENTS "")
foreach(LINE IN LISTS PROJECT_VERSION_LINES)
    if (LINE MATCHES "^m_EditorVersion: ")
        set(LINE "m_EditorVersion: ${UNITY_VERSION}")
    elseif (LINE MATCHES "^m_EditorVersionWithRevision: ")
        set(LINE "m_EditorVersionWithRevision: ${UNITY_VERSION} (unknown_revision)")
    endif()
    # Append the line (modified or unmodified) to the updated content
    list(APPEND UPDATED_CONTENTS "${LINE}")
endforeach()

# Join the updated lines back into a single string
string(REPLACE ";" "\n" FINAL_CONTENTS "${UPDATED_CONTENTS}")

# Ensure a trailing newline
set(FINAL_CONTENTS "${FINAL_CONTENTS}\n")

# Write the updated content back to ProjectVersion.txt
file(WRITE "${PROJECT_VERSION_FILE}" "${FINAL_CONTENTS}")

message(STATUS "Updated Unity version to ${UNITY_VERSION} in ProjectVersion.txt")
