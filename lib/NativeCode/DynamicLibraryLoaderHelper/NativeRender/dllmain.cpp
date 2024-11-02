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

#include "config.h"
#include "logging.h"
#include <json_helpers.h>
#include <sstream>

#include "eos_library_helpers.h"
#include <eos_helpers.h>

#include "io_helpers.h"


typedef HKEY__* HKEY;

using FSig_ApplicationWillShutdown = void (__stdcall *)(void);
FSig_ApplicationWillShutdown FuncApplicationWillShutdown = nullptr;

extern "C"
{
    void __declspec(dllexport) __stdcall UnityPluginLoad(void* unityInterfaces);
    void __declspec(dllexport) __stdcall UnityPluginUnload();
}

//-------------------------------------------------------------------------
// Called by unity on load. It kicks off the work to load the DLL for Overlay
#if PLATFORM_32BITS
#pragma comment(linker, "/export:UnityPluginLoad=_UnityPluginLoad@4")
#endif
DLL_EXPORT(void) UnityPluginLoad(void*)
{
#if _DEBUG
    pew::eos::logging::show_log_as_dialog("You may attach a debugger to the DLL");
#endif

    auto path_to_config_json = pew::eos::config::get_path_for_eos_service_config(EOS_SERVICE_CONFIG_FILENAME);
    json_value_s* eos_config_as_json = nullptr;

    eos_config_as_json = pew::eos::config::read_config_json_from_dll();

    if (!eos_config_as_json && std::filesystem::exists(path_to_config_json))
    {
        eos_config_as_json = pew::eos::json_helpers::read_config_json_as_json_from_path(path_to_config_json);
    }

    if (!eos_config_as_json)
    {
        pew::eos::logging::log_warn("Failed to load a valid json config for EOS");
        return;
    }

    pew::eos::config::EOSConfig eos_config = pew::eos::config::eos_config_from_json_value(eos_config_as_json);
    free(eos_config_as_json);

#if PLATFORM_WINDOWS
    //support sandbox and deployment id override via command line arguments
    std::stringstream argStream = std::stringstream(GetCommandLineA());
    std::istream_iterator<std::string> argsBegin(argStream);
    std::istream_iterator<std::string> argsEnd;
    std::vector<std::string> argStrings(argsBegin, argsEnd);
    std::string egsArgName = "-epicsandboxid=";
    std::string sandboxArgName = "-eossandboxid=";
    for (unsigned i = 0; i < argStrings.size(); ++i)
    {
        std::string* match = nullptr;
        if (argStrings[i]._Starts_with(sandboxArgName))
        {
            match = &sandboxArgName;
        }
        else if(argStrings[i]._Starts_with(egsArgName))
        {
            match = &egsArgName;
        }
        if (match != nullptr)
        {
            std::string sandboxArg = argStrings[i].substr(match->length());
            if (!sandboxArg.empty())
            {
                pew::eos::logging::log_inform(("Sandbox ID override specified: " + sandboxArg).c_str());
                eos_config.sandboxID = sandboxArg;
            }
        }
    }
#endif

    //check if a deployment id override exists for sandbox id
    for (unsigned i = 0; i < eos_config.sandboxDeploymentOverrides.size(); ++i)
    {
        if (eos_config.sandboxID == eos_config.sandboxDeploymentOverrides[i].sandboxID)
        {
            pew::eos::logging::log_inform(("Sandbox Deployment ID override specified: " + eos_config.sandboxDeploymentOverrides[i].deploymentID).c_str());
            eos_config.deploymentID = eos_config.sandboxDeploymentOverrides[i].deploymentID;
        }
    }

#if PLATFORM_WINDOWS
    std::string deploymentArgName = "-eosdeploymentid=";
    std::string egsDeploymentArgName = "-epicdeploymentid=";
    for (unsigned i = 0; i < argStrings.size(); ++i)
    {
        std::string* match = nullptr;
        if (argStrings[i]._Starts_with(deploymentArgName))
        {
            match = &deploymentArgName;
        }
        else if (argStrings[i]._Starts_with(egsDeploymentArgName))
        {
            match = &egsDeploymentArgName;
        }
        if (match != nullptr)
        {
            std::string deploymentArg = argStrings[i].substr(match->length());
            if (!deploymentArg.empty())
            {
                pew::eos::logging::log_inform(("Deployment ID override specified: " + deploymentArg).c_str());
                eos_config.deploymentID = deploymentArg;
            }
        }
    }
#endif

#if _DEBUG
    pew::eos::logging::global_log_open("gfx_log.txt");
#endif

    std::filesystem::path DllPath;
    pew::eos::logging::log_inform("On UnityPluginLoad");
    //if (!get_overlay_dll_path(&DllPath))
    //{
    //    show_log_as_dialog("Missing Overlay DLL!\n Overlay functionality will not work!");
    //}

    pew::eos::eos_library_helpers::s_eos_sdk_lib_handle = pew::eos::eos_library_helpers::load_library_at_path(pew::eos::io_helpers::get_path_relative_to_current_module(SDK_DLL_NAME));

    //eos_sdk_overlay_lib_handle = load_library_at_path(DllPath);
    //if (eos_sdk_overlay_lib_handle)
    //{
    //    log_warn("loaded eos overlay sdk");
    //    FuncApplicationWillShutdown = load_function_with_name<FSig_ApplicationWillShutdown>(eos_sdk_overlay_lib_handle, "EOS_Overlay_Initilize");
    //    if(FuncApplicationWillShutdown == nullptr)
    //    {
    //        log_warn("Unable to find overlay function");
    //    }
    //}

    if (pew::eos::eos_library_helpers::s_eos_sdk_lib_handle)
    {
        pew::eos::eos_library_helpers::FetchEOSFunctionPointers();

        if (pew::eos::eos_library_helpers::EOS_Initialize_ptr)
        {
            pew::eos::logging::log_inform("start eos init");

            pew::eos::eos_init(eos_config);

            pew::eos::eos_set_loglevel_via_config();
            //log_warn("start eos create");
            pew::eos::eos_create(eos_config);

            // This code is commented out because the handle is now handed off to the C# code
            //EOS_Platform_Release(eos_platform_handle);
            //eos_platform_handle = NULL;
            //log_warn("start eos shutdown");
            //EOS_Shutdown();
            //log_warn("unload eos sdk");
            //unload_library(s_eos_sdk_lib_handle);

            pew::eos::eos_library_helpers::s_eos_sdk_lib_handle = NULL;
            pew::eos::eos_library_helpers::EOS_Initialize_ptr = NULL;
            pew::eos::eos_library_helpers::EOS_Shutdown_ptr = NULL;
            pew::eos::eos_library_helpers::EOS_Platform_Create_ptr = NULL;
        }
        else
        {
            pew::eos::logging::log_warn("unable to find EOS_Initialize");
        }

    }
    else
    {
        pew::eos::logging::log_warn("Couldn't find dll "  SDK_DLL_NAME);
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
    pew::eos::eos_library_helpers::unload_library(pew::eos::eos_library_helpers::s_eos_sdk_overlay_lib_handle);
    pew::eos::eos_library_helpers::s_eos_sdk_overlay_lib_handle = nullptr;

    pew::eos::logging::global_log_close();
}
