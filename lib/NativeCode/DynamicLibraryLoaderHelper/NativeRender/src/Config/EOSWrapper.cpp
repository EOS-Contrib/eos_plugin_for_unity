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

#include <pch.h>
#include "EOSWrapper.h"
#include "ProductConfig.h"
#include "WindowsConfig.h"
#include "config.h"
#include "eos_helpers.h"

namespace pew::eos
{

    EOS_HPlatform EOSWrapper::load_eos_sdk()
    {
        // Get the configuration values
        const auto product_config = config::Config::get<config::ProductConfig>();
        auto windows_config = config::Config::get<config::WindowsConfig>();

        // Apply any command line arguments that there might be
        apply_cli_arguments(windows_config, product_config);

        // Initialize the sdk
        init(windows_config, product_config);

        return load_eos
    }

    void EOSWrapper::init(const config::PlatformConfig& platform_config, const config::ProductConfig& product_config)
    {
        EOS_InitializeOptions sdk_options{};
        sdk_options.ApiVersion = EOS_INITIALIZE_API_LATEST;
        sdk_options.AllocateMemoryFunction = nullptr;
        sdk_options.ReallocateMemoryFunction = nullptr;
        sdk_options.ReleaseMemoryFunction = nullptr;
        sdk_options.ProductName = product_config.product_name.c_str();
        sdk_options.ProductVersion = product_config.product_version.c_str();
        sdk_options.Reserved = new int[] {1, 1};
        sdk_options.SystemInitializeOptions = nullptr;

        // Because the parameter passed is const, the value needs to be copied.
        EOS_Initialize_ThreadAffinity affinity = platform_config.thread_affinity;
        sdk_options.OverrideThreadAffinity = &affinity;

        const auto init_result = call_library_function<EOS_Initialize_t>(&sdk_options);
        if (init_result != EOS_EResult::EOS_Success)
        {
            logging::log_error("Unable to do eos init");
        }

        // Set the log level and the log callback
        call_library_function<EOS_Logging_SetLogLevel_t>(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, EOS_ELogLevel::EOS_LOG_VeryVerbose);
        call_library_function<EOS_Logging_SetCallback_t>(&logging::eos_log_callback);
    }
}
