#ifndef LOGGING_H
#define LOGGING_H
/*
 * Copyright (c) 2021 PlayEveryWare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <string>
#include <eos_logging.h>

#include "include/PEW_EOS_Defines.h"

/**
  * \brief Forward declarations
  */
enum class EOS_ELogLevel;

namespace pew::eos::logging
{
    typedef void (*log_flush_function_t)(const char* str);

    PEW_EOS_API_FUNC(void) set_mirror_to_stdout(const bool& mirror);

    /**
     * \brief Allows an option to be set that will mirror the log messages to
     * stdout.
     * \param mirror Whether to mirror log statements to stdout.
     */
    PEW_EOS_API_FUNC(void) set_mirror_to_stdout(const bool& mirror);

    /**
     * @brief Converts an EOS log level to its corresponding string representation.
     *
     * Maps an EOS log level enumeration to a string (e.g., "Fatal", "Error").
     * Returns `nullptr` if the log level is unrecognized.
     *
     * @param level The EOS log level to convert.
     * @return The corresponding string representation, or `nullptr` if not found.
     */
    const char* eos_loglevel_to_print_str(EOS_ELogLevel level);

    /**
     * @brief Flushes buffered log messages using a custom flush function.
     *
     * Iterates over any buffered log messages and passes each one to the specified flush function.
     * After flushing, the buffer is cleared.
     *
     * @param log_flush_function The function to call for each log message in the buffer.
     */
    PEW_EOS_API_FUNC(void) global_log_flush_with_function(log_flush_function_t log_flush_function);

    /**
     * @brief Converts a log level string to its corresponding EOS log level enumeration.
     *
     * Maps a string to an EOS log level enum value (e.g., "Error" to `EOS_LOG_Error`).
     * Returns `EOS_LOG_Verbose` if the string is not found in the map.
     *
     * @param str The log level as a string.
     * @return The corresponding EOS log level enum value, or `EOS_LOG_Verbose` if not found.
     */
    EOS_ELogLevel eos_loglevel_str_to_enum(const std::string& str);

    /**
     * @brief Displays a log message in a dialog box (Windows only).
     *
     * Opens a Windows message box displaying the provided log message as a warning.
     *
     * @param log_string The message to display in the dialog box.
     */
    void show_log_as_dialog(const char* log_string);

    /**
     * @brief Closes the global log file and clears the buffer.
     *
     * Closes the currently open log file (if any) and clears any buffered log messages.
     */
    void global_log_close();

    /**
     * @brief Writes a formatted log message to the log file or buffer.
     *
     * Logs a formatted message to the open log file, if available, or to a buffer for deferred logging.
     * Supports standard `printf` formatting.
     *
     * @param format The format string for the log message.
     * @param ... Arguments for the format string.
     */
    void global_logf(const char* format, ...);

    /**
     * @brief EOS SDK log callback function.
     *
     * Receives log messages from the EOS SDK and logs them with a timestamp, category,
     * and log level.
     *
     * @param message The log message provided by the EOS SDK.
     */
    PEW_EOS_API_FUNC(void) EOS_CALL eos_log_callback(const EOS_LogMessage* message);

    /**
     * @brief Opens a log file for writing.
     *
     * Opens a specified log file and writes any buffered log messages to it. If a log
     * file is already open, it is closed before opening the new file.
     *
     * @param filename The name of the file to open for logging.
     */
    void global_log_open(const char* filename);

    /**
     * @brief Logs a message with a timestamp and header.
     *
     * Logs a message with the specified header (e.g., "WARNING", "ERROR"),
     * and includes a timestamp if available.
     *
     * @param header The log header (e.g., "WARNING", "ERROR").
     * @param message The log message to record.
     */
    void log_base(const char* header, const char* message);

    /**
     * @brief Logs a warning message.
     *
     * Records a warning message with a "WARNING" header and, if enabled,
     * displays it in a dialog box.
     *
     * @param log_string The warning message to log.
     */
    PEW_EOS_API_FUNC(void) log_warn(const char* log_string);

    /**
     * @brief Logs a warning message.
     *
     * Records a warning message with a "WARNING" header and, if enabled,
     * displays it in a dialog box.
     *
     * @param log_string The warning message to log.
     */
    inline void log_warn(const std::string& log_string)
    {
        return log_warn(log_string.c_str());
    }

    /**
     * @brief Logs an informational message.
     *
     * Records an informational message with an "INFORM" header.
     *
     * @param log_string The informational message to log.
     */
    PEW_EOS_API_FUNC(void) log_inform(const char* log_string);

    /**
     * @brief Logs an informational message.
     *
     * Records an informational message with an "INFORM" header.
     *
     * @param log_string The informational message to log.
     */
    inline void log_inform(const std::string& log_string)
    {
        return log_inform(log_string.c_str());
    }

    /**
     * @brief Logs an error message.
     *
     * Records an error message with an "ERROR" header.
     *
     * @param log_string The error message to log.
     */
    PEW_EOS_API_FUNC(void) log_error(const char* log_string);

    /**
     * @brief Logs an error message.
     *
     * Records an error message with an "ERROR" header.
     *
     * @param log_string The error message to log.
     */
    inline void log_error(const std::string& log_string)
    {
        return log_error(log_string.c_str());
    }
}
#endif
