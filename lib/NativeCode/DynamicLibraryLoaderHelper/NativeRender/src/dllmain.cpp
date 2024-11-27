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

#include "config_legacy.h"
#include "logging.h"
#include <eos_library_helpers.h>
#include <eos_helpers.h>
#include "io_helpers.h"

using namespace pew::eos;
using namespace pew::eos::eos_library_helpers;

using FSig_ApplicationWillShutdown = void (__stdcall *)(void);
FSig_ApplicationWillShutdown FuncApplicationWillShutdown = nullptr;

extern "C"
{
    /**
     * \brief Forward declaration and export for function to be called when
     * Unity is loading the plugin.
     */
    void __declspec(dllexport) __stdcall UnityPluginLoad(void* unityInterfaces);

    /**
     * \brief Forward declaration for function to be called when Unity is
     * unloading the plugin.
     */
    void __declspec(dllexport) __stdcall UnityPluginUnload();
}

void get_cli_arguments(config_legacy::EOSConfig eos_config)
{
    //support sandbox and deployment id override via command line arguments
    std::stringstream argument_stream = std::stringstream(GetCommandLineA());
    std::istream_iterator<std::string> argument_stream_begin(argument_stream);
    std::istream_iterator<std::string> argument_stream_end;
    std::vector<std::string> argument_strings(argument_stream_begin, argument_stream_end);
    std::string egsArgName = "-epicsandboxid=";
    std::string sandboxArgName = "-eossandboxid=";
    for (unsigned i = 0; i < argument_strings.size(); ++i)
    {
        std::string* match = nullptr;
        if (argument_strings[i]._Starts_with(sandboxArgName))
        {
            match = &sandboxArgName;
        }
        else if (argument_strings[i]._Starts_with(egsArgName))
        {
            match = &egsArgName;
        }
        if (match != nullptr)
        {
            std::string sandboxArg = argument_strings[i].substr(match->length());
            if (!sandboxArg.empty())
            {
                logging::log_inform(("Sandbox ID override specified: " + sandboxArg).c_str());
                eos_config.sandboxID = sandboxArg;
            }
        }
    }

    //check if a deployment id override exists for sandbox id
    for (unsigned i = 0; i < eos_config.sandboxDeploymentOverrides.size(); ++i)
    {
        if (eos_config.sandboxID == eos_config.sandboxDeploymentOverrides[i].sandboxID)
        {
            logging::log_inform(("Sandbox Deployment ID override specified: " + eos_config.sandboxDeploymentOverrides[i].deploymentID).c_str());
            eos_config.deploymentID = eos_config.sandboxDeploymentOverrides[i].deploymentID;
        }
    }

    std::string deploymentArgName = "-eosdeploymentid=";
    std::string egsDeploymentArgName = "-epicdeploymentid=";
    for (unsigned i = 0; i < argument_strings.size(); ++i)
    {
        std::string* match = nullptr;
        if (argument_strings[i]._Starts_with(deploymentArgName))
        {
            match = &deploymentArgName;
        }
        else if (argument_strings[i]._Starts_with(egsDeploymentArgName))
        {
            match = &egsDeploymentArgName;
        }
        if (match != nullptr)
        {
            std::string deploymentArg = argument_strings[i].substr(match->length());
            if (!deploymentArg.empty())
            {
                logging::log_inform(("Deployment ID override specified: " + deploymentArg).c_str());
                eos_config.deploymentID = deploymentArg;
            }
        }
    }
}

// Called by unity on load. It kicks off the work to load the DLL for Overlay
#if PLATFORM_32BITS
#pragma comment(linker, "/export:UnityPluginLoad=_UnityPluginLoad@4")
#endif
PEW_EOS_API_FUNC(void) UnityPluginLoad(void*)
{
#if _DEBUG
    logging::show_log_as_dialog("You may attach a debugger to the DLL");
#endif

    config_legacy::EOSConfig eos_config;
    if (!config_legacy::try_get_eos_config(eos_config))
    {
        return;
    }

    get_cli_arguments(eos_config);

#if _DEBUG
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

            eos_init(eos_config);
            eos_set_loglevel_via_config();
            eos_create(eos_config);

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
