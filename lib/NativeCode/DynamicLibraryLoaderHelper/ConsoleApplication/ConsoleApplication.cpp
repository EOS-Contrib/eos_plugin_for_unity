/*
 * Copyright (c) 2024 PlayEveryWare
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

#include <iostream>
#include <config.h>
#include <windows.h>

typedef const char* (__cdecl *GetWindowsConfigFunction)();
typedef const char* (__cdecl *GetDeploymentId)();
typedef const char* (__cdecl *GetSandboxId)();
typedef bool(__cdecl *GetIsServer)();
typedef int (__cdecl *GetAuthScopeOptionsFlags)();
typedef int (__cdecl *GetIntegratedPlatformManagementFlags)();
typedef int (__cdecl *GetTickBudgetInMilliseconds)();
typedef double(__cdecl *GetTaskNetworkTimeoutSeconds)();
typedef int (__cdecl *GetThreadAffinity)();
typedef bool(__cdecl *GetAlwaysSendInputToOverlay)();
typedef float(__cdecl *GetInitialButtonDelayForOverlay)();
typedef float(__cdecl *GetRepeatButtonDelayForOverlay)();
typedef int (__cdecl *GetToggleFriendsButtonCombination)();

const char* call_native_aot()
{
    HMODULE hModule = LoadLibraryW(L"EOSPluginNativeAOT.dll");
    if (!hModule)
    {
        std::cerr << "Failed to load library." << std::endl;
        return nullptr;
    }

    // Function pointers for each of the typedefs
    // Function pointers for each of the typedefs using reinterpret_cast
    auto GetWindowsConfig = reinterpret_cast<GetWindowsConfigFunction>(GetProcAddress(hModule, "GetWindowsConfig"));
    auto GetDeploymentIdPtr = reinterpret_cast<GetDeploymentId>(GetProcAddress(hModule, "GetDeploymentId"));
    auto GetSandboxIdPtr = reinterpret_cast<GetSandboxId>(GetProcAddress(hModule, "GetSandboxId"));
    auto GetIsServerPtr = reinterpret_cast<GetIsServer>(GetProcAddress(hModule, "GetIsServer"));
    auto GetAuthScopeOptionsFlagsPtr = reinterpret_cast<GetAuthScopeOptionsFlags>(GetProcAddress(hModule, "GetAuthScopeOptionsFlags"));
    auto GetIntegratedPlatformManagementFlagsPtr = reinterpret_cast<GetIntegratedPlatformManagementFlags>(GetProcAddress(hModule, "GetIntegratedPlatformManagementFlags"));
    auto GetTickBudgetInMillisecondsPtr = reinterpret_cast<GetTickBudgetInMilliseconds>(GetProcAddress(hModule, "GetTickBudgetInMilliseconds"));
    auto GetTaskNetworkTimeoutSecondsPtr = reinterpret_cast<GetTaskNetworkTimeoutSeconds>(GetProcAddress(hModule, "GetTaskNetworkTimeoutSeconds"));
    auto GetThreadAffinityPtr = reinterpret_cast<GetThreadAffinity>(GetProcAddress(hModule, "GetThreadAffinity"));
    auto GetAlwaysSendInputToOverlayPtr = reinterpret_cast<GetAlwaysSendInputToOverlay>(GetProcAddress(hModule, "GetAlwaysSendInputToOverlay"));
    auto GetInitialButtonDelayForOverlayPtr = reinterpret_cast<GetInitialButtonDelayForOverlay>(GetProcAddress(hModule, "GetInitialButtonDelayForOverlay"));
    auto GetRepeatButtonDelayForOverlayPtr = reinterpret_cast<GetRepeatButtonDelayForOverlay>(GetProcAddress(hModule, "GetRepeatButtonDelayForOverlay"));
    auto GetToggleFriendsButtonCombinationPtr = reinterpret_cast<GetToggleFriendsButtonCombination>(GetProcAddress(hModule, "GetToggleFriendsButtonCombination"));

    if (!GetWindowsConfig ||
        !GetDeploymentIdPtr ||
        !GetSandboxIdPtr ||
        !GetIsServerPtr ||
        !GetAuthScopeOptionsFlagsPtr ||
        !GetIntegratedPlatformManagementFlagsPtr ||
        !GetTickBudgetInMillisecondsPtr ||
        !GetTaskNetworkTimeoutSecondsPtr ||
        !GetThreadAffinityPtr ||
        !GetAlwaysSendInputToOverlayPtr ||
        !GetInitialButtonDelayForOverlayPtr ||
        !GetRepeatButtonDelayForOverlayPtr ||
        !GetToggleFriendsButtonCombinationPtr)
    {
        std::cerr << "Failed to get one or more function pointers." << std::endl;
        FreeLibrary(hModule);
        return 1;
    }

    // Use the function pointers as needed...


    const char* windowsConfigJSON = GetWindowsConfig();
    if (windowsConfigJSON)
    {
        std::cout << "JSON Data: " << windowsConfigJSON << std::endl;
    }

    FreeLibrary(hModule);
    return windowsConfigJSON;
}

int main()
{
    auto windowsConfigJSON = call_native_aot();

    pew::eos::config::EOSConfig eos_config;
    if(try_get_eos_config(eos_config))
    {
        std::cout << "EOSConfig was read successfully.";
    }
    else
    {
        std::cout << "Could not load EOSConfig.";
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
