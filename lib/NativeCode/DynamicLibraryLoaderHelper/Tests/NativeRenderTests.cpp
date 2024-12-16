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
#include "pch.h"

#include "eos_helpers.h"

namespace pew::eos::tests
{
    TEST(NativeRender, NativeRenderGetPlatformInterfaceTest)
    {
        // Call the plugin load function.
        UnityPluginLoad(nullptr);

        // Try to get a handle to the EOS SDK Platform Interface.
        const auto eos_platform_interface = EOS_GetPlatformInterface();

        // Check to make certain that a non-null pointer was acquired.
        EXPECT_TRUE(eos_platform_interface != nullptr) << "Could not get platform interface for EOS SDK.";

        // Unload the plugin.
        UnityPluginUnload();
    }
}
