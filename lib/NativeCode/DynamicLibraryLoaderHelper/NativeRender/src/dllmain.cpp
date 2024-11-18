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
#include <eos_library_helpers.h>
#include <eos_helpers.h>
#include "io_helpers.h"
#include "PlatformConfig.h"
#include "WindowsConfig.h"
#include "ProductConfig.h"
#include "Config/Config.h"

using namespace pew::eos;
using namespace pew::eos::eos_library_helpers;

using FSig_ApplicationWillShutdown = void (__stdcall *)(void);
FSig_ApplicationWillShutdown FuncApplicationWillShutdown = nullptr;

extern "C"
{
    void __declspec(dllexport) __stdcall UnityPluginLoad(void* unityInterfaces);
    void __declspec(dllexport) __stdcall UnityPluginUnload();
}

void get_cli_arguments(config::PlatformConfig& platform_config, const config::ProductConfig& product_config)
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
                platform_config.deployment.sandbox.id = sandboxArg;
            }
        }
    }

    ////check if a deployment id override exists for sandbox id
    //for (unsigned i = 0; i < product_config.environments.sandboxDeploymentOverrides.size(); ++i)
    //{
    //    if (platform_config.sandboxID == platform_config.sandboxDeploymentOverrides[i].sandboxID)
    //    {
    //        logging::log_inform(("Sandbox Deployment ID override specified: " + platform_config.sandboxDeploymentOverrides[i].deploymentID).c_str());
    //        platform_config.deploymentID = platform_config.sandboxDeploymentOverrides[i].deploymentID;
    //    }
    //}

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
                platform_config.deployment.id = deploymentArg;
            }
        }
    }
}

template <typename... Args>
void test(std::vector<std::string>>& arguments_list, const Args&... args)
{
    
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

    get_cli_arguments(windows_config, product_config);

#if _DEBUG
    logging::global_log_open("gfx_log.txt");
#endif

    std::filesystem::path DllPath;
    logging::log_inform("On UnityPluginLoad");

    s_eos_sdk_lib_handle = load_library_at_path(io_helpers::get_path_relative_to_current_module(SDK_DLL_NAME));

    if (s_eos_sdk_lib_handle)
    {
        FetchEOSFunctionPointers();

        if (EOS_Initialize_ptr)
        {
            logging::log_inform("start eos init");

            eos_init(windows_config, product_config);

            eos_set_loglevel_via_config();

            eos_create(windows_config, product_config);

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
