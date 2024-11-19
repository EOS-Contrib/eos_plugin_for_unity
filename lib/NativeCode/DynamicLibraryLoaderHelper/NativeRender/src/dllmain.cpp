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

#include "config.h"
#include "logging.h"
#include <eos_helpers.h>
#include <iostream>

#include "PlatformConfig.h"
#include "WindowsConfig.h"
#include "ProductConfig.h"
#include "Config/Config.h"

using namespace pew::eos;

using FSig_ApplicationWillShutdown = void (__stdcall *)(void);
FSig_ApplicationWillShutdown FuncApplicationWillShutdown = nullptr;

extern "C"
{
    void __declspec(dllexport) __stdcall UnityPluginLoad(void* unityInterfaces);
    void __declspec(dllexport) __stdcall UnityPluginUnload();
}

/**
 * \brief Gets the value of a command line argument specified by one or more possible flags.
 * \tparam Flags Options for what flags might be permissible for the value. Exclude the leading dash and the following "=" from the flags provided.
 * \param arguments All of the command-line arguments passed in.
 * \param value The value for the indicated command line argument.
 * \param args The labels that can be used to specify the command argument.
 * \return True if the argument was provided, false otherwise.
 */
template <typename... Flags>
bool try_get_command_line_argument(const std::vector<std::string>& arguments, std::string& value, const Flags&... args)
{
    // This gathers the variadic parameters which represent parameter flags, any
    // of which indicate the same value that is being passed in on the command
    // line.
    std::vector<std::string> flag_options = { args... };

    for (const auto& argument : arguments)
    {
        const std::string* match = nullptr;

        // See if the argument matches any of the flag options provided.
        for (const std::string& flag : flag_options)
        {
            // If the argument doesn't start with the flag, move to the next.
            if (!argument._Starts_with("-" + flag + "="))
            {
                continue;
            }

            // Indicate the match and exit the loop.
            match = &flag;
            break;
        }

        // If there was a match
        if (match != nullptr)
        {
            // extract the value.
            std::string arg_value;
            arg_value = argument.substr(match->length());

            if (!arg_value.empty())
            {
                value = arg_value;
                return true;
            }
        }
    }

    // Return false if the value wasn't set on the command line.
    return false;
}

/**
 * \brief Applies any command line arguments that may have been provided.
 * \param platform_config The platform config whose values may need to be
 * overridden by command line arguments.
 * \param product_config The product config. This is used to warn the user if
 * the provided sandbox id or deployment id is not defined in the product
 * config. If they are not defined, they will still be applied.
 */
void apply_cli_arguments(config::PlatformConfig& platform_config, const config::ProductConfig& product_config)
{
    //support sandbox and deployment id override via command line arguments
    std::stringstream argument_stream = std::stringstream(GetCommandLineA());
    const std::istream_iterator<std::string> argument_stream_begin(argument_stream);
    const std::istream_iterator<std::string> argument_stream_end;
    const std::vector argument_strings(argument_stream_begin, argument_stream_end);

    std::string sandbox_id_override;
    if (try_get_command_line_argument(argument_strings, sandbox_id_override, "epicsandboxid", "eossandboxid"))
    {
        if (!product_config.environments.is_sandbox_defined(sandbox_id_override))
        {
            std::cerr << "Sandbox Id \"" << sandbox_id_override << "\" was provided on the command line, but is not found in the product config. Attempting to use it regardless." << std::endl;
        }
        platform_config.deployment.sandbox.id = sandbox_id_override;
    }

    std::string deployment_id_override;
    if (try_get_command_line_argument(argument_strings, deployment_id_override, "eosdeploymentid", "epicdeploymentid"))
    {
        if (!product_config.environments.is_deployment_defined(deployment_id_override))
        {
            std::cerr << "Deployment Id \"" << deployment_id_override << "\" was provided on the command line, but is not found in the product config. Attempting to use it regardless." << std::endl;
        }
        platform_config.deployment.id = deployment_id_override;
    }
}


// Called by unity on load. It kicks off the work to load the DLL for Overlay
#if PLATFORM_32BITS
#pragma comment(linker, "/export:UnityPluginLoad=_UnityPluginLoad@4")
#endif
DLL_EXPORT(void) UnityPluginLoad(void*)
{
#if _DEBUG
    logging::show_log_as_dialog("You may attach a debugger to the DLL");
#endif

    const config::ProductConfig product_config = config::Config::get<config::ProductConfig>();
    config::WindowsConfig windows_config = config::Config::get<config::WindowsConfig>();

    apply_cli_arguments(windows_config, product_config);

#if _DEBUG
    logging::global_log_open("gfx_log.txt");
#endif

    std::filesystem::path DllPath;
    logging::log_inform("On UnityPluginLoad");

    

    if (s_eos_sdk_lib_handle)
    {
        FetchEOSFunctionPointers();

        logging::log_inform("start eos init");

        load_eos(windows_config, product_config);

        eos_set_loglevel_via_config();

        s_eos_sdk_lib_handle = nullptr;
        EOS_Shutdown_ptr = nullptr;
        //EOS_Platform_Create_ptr = nullptr;
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
DLL_EXPORT(void) UnityPluginUnload()
{
    if (FuncApplicationWillShutdown != nullptr)
    {
        FuncApplicationWillShutdown();
    }
    unload_library(s_eos_sdk_overlay_lib_handle);
    s_eos_sdk_overlay_lib_handle = nullptr;

    logging::global_log_close();
}
