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

// dllmain.cpp : Defines the entry point for the DLL application.
// This file does some *magick* to load the EOS Overlay DLL.
// This is apparently needed so that the Overlay can render properly
#include "pch.h"

#include <iterator>
#include <sstream>
#include <string>
#include "config_legacy.h"
#include "logging.h"
#include <eos_library_helpers.h>
#include <eos_helpers.h>
#include "io_helpers.h"
#include "Config/WindowsConfig.hpp"

using namespace pew::eos;
using namespace pew::eos::eos_library_helpers;

using FSig_ApplicationWillShutdown = void (__stdcall *)(void);
FSig_ApplicationWillShutdown FuncApplicationWillShutdown = nullptr;

/**
 * \brief Applies any command line arguments that may have been provided.
 * \param platform_config The platform config whose values may need to be
 * overridden by command line arguments.
 * \param product_config The product config. This is used to warn the user if
 * the provided sandbox id or deployment id is not defined in the product
 * config. If they are not defined, they will still be applied.
 */
static void apply_cli_arguments(config::PlatformConfig& platform_config, const config::ProductConfig& product_config)
{
    //support sandbox and deployment id override via command line arguments
    auto argument_stream = std::stringstream(GetCommandLineA());
    const std::istream_iterator<std::string> argument_stream_begin(argument_stream);
    const std::istream_iterator<std::string> argument_stream_end;
    const std::vector argument_strings(argument_stream_begin, argument_stream_end);

    std::string sandbox_id_override;
    if (io_helpers::try_get_command_line_argument(argument_strings, sandbox_id_override, "epicsandboxid", "eossandboxid"))
    {
        if (!product_config.environments.is_sandbox_defined(sandbox_id_override))
        {
            logging::log_warn(
              "Sandbox Id \"" + sandbox_id_override + "\" was provided on the "
              "command line, but is not found in the product config. Attempting "
              "to use it regardless.");
        }
        platform_config.deployment.sandbox.id = sandbox_id_override;
    }

    std::string deployment_id_override;
    if (io_helpers::try_get_command_line_argument(argument_strings, deployment_id_override, "eosdeploymentid", "epicdeploymentid"))
    {
        if (!product_config.environments.is_deployment_defined(deployment_id_override))
        {
            logging::log_warn(
              "Deployment Id \"" + deployment_id_override + "\" was provided on the "
              "command line, but is not found in the product config. Attempting "
              "to use it regardless.");
        }
        platform_config.deployment.id = deployment_id_override;
    }
}

// Called by unity on load. It kicks off the work to load the DLL for Overlay
#if PLATFORM_32BITS
#pragma comment(linker, "/export:UnityPluginLoad=_UnityPluginLoad@4")
#endif
PEW_EOS_API_FUNC(void) UnityPluginLoad(void* arg)
{
#if _DEBUG
    if (!IsDebuggerPresent())
    {
        logging::show_log_as_dialog("You may attach a debugger to the DLL");
    }
    logging::global_log_open("gfx_log.txt");
#endif

    std::filesystem::path DllPath;
    logging::log_inform("On UnityPluginLoad");

    // Acquire pointer to EOS SDK library
    s_eos_sdk_lib_handle = load_library_at_path(io_helpers::get_path_relative_to_current_module(SDK_DLL_NAME));

    // If acquisition was successful.
    if (s_eos_sdk_lib_handle)
    {
        // Make use of the library handle to get pointers to all the functions
        // that are needed.
        FetchEOSFunctionPointers();

        // If the initialize function pointer is not null
        if (EOS_Initialize_ptr)
        {
            logging::log_inform("start eos init");

            const auto product_config = config::Config::get<config::ProductConfig>();
            const auto windows_config = config::Config::get<config::WindowsConfig>();

            apply_cli_arguments(*windows_config, *product_config);

            eos_init(*windows_config, *product_config);
            eos_set_loglevel_via_config();
            eos_create(*windows_config, *product_config);

            // Free function pointers and library handle.
            s_eos_sdk_lib_handle = nullptr;
            EOS_Initialize_ptr = nullptr;
            EOS_Shutdown_ptr = nullptr;
            EOS_Platform_Create_ptr = nullptr;
        }
        else
        {
            logging::log_warn("unable to find EOS_Initialize");
        }
    }
    else
    {
        logging::log_warn("Couldn't find dll "  SDK_DLL_NAME);
    }
}

//-------------------------------------------------------------------------
#if PLATFORM_32BITS
#pragma comment(linker, "/export:_UnityPluginUnload=_UnityPluginUnload@0")
#endif
PEW_EOS_API_FUNC(void) UnityPluginUnload()
{
    if (FuncApplicationWillShutdown != nullptr)
    {
        FuncApplicationWillShutdown();
    }
    unload_library(s_eos_sdk_overlay_lib_handle);
    s_eos_sdk_overlay_lib_handle = nullptr;

    logging::global_log_close();
}
