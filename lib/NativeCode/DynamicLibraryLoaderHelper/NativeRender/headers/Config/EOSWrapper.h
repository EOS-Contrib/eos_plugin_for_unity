#ifndef EOS_WRAPPER_H
#define EOS_WRAPPER_H

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

#pragma once

#include "headers/Config/DLLWrapper.h"
#include "headers/Config/ConfigBase.h"
#include "eos_init.h"
#include "eos_base.h"

namespace pew::eos::config
{
    class PlatformConfig;
    class ProductConfig;
}

namespace pew::eos
{
    REGISTER_LIBRARY_FUNCTION("EOS_Initialize", EOS_EResult(EOS_CALL* EOS_Initialize_t)(const EOS_InitializeOptions* Options), EOS_Initialize_t);
    REGISTER_LIBRARY_FUNCTION("EOS_Shutdown", EOS_EResult(EOS_CALL* EOS_Shutdown_t)(), EOS_Shutdown_t);
    REGISTER_LIBRARY_FUNCTION("EOS_Platform_Create", EOS_HPlatform(EOS_CALL* EOS_Platform_Create_t)(const EOS_Platform_Options* Options), EOS_Platform_Create_t);
    REGISTER_LIBRARY_FUNCTION("EOS_Logging_SetCallback", EOS_EResult(EOS_CALL* EOS_Logging_SetCallback_t)(EOS_LogMessageFunc Callback), EOS_Logging_SetCallback_t);
    REGISTER_LIBRARY_FUNCTION("EOS_Logging_SetLogLevel", EOS_EResult(EOS_CALL* EOS_Logging_SetLogLevel_t)(EOS_ELogCategory LogCategory, EOS_ELogLevel LogLevel), EOS_Logging_SetLogLevel_t);
    REGISTER_LIBRARY_FUNCTION("EOS_IntegratedPlatformOptionsContainer_Add", EOS_EResult(*EOS_IntegratedPlatformOptionsContainer_Add_t)(EOS_HIntegratedPlatformOptionsContainer Handle, const EOS_IntegratedPlatformOptionsContainer_AddOptions* InOptions), EOS_IntegratedPlatformOptionsContainer_Add_t);
    REGISTER_LIBRARY_FUNCTION("EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer", EOS_EResult(*EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t)(const EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions* Options, EOS_HIntegratedPlatformOptionsContainer* OutIntegratedPlatformOptionsContainerHandle), EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t);
    REGISTER_LIBRARY_FUNCTION("EOS_IntegratedPlatformOptionsContainer_Release", void(*EOS_IntegratedPlatformOptionsContainer_Release_t)(EOS_HIntegratedPlatformOptionsContainer IntegratedPlatformOptionsContainerHandle), EOS_IntegratedPlatformOptionsContainer_Release_t);

    class EOSWrapper : public DLLWrapper
    {
    public:
        EOSWrapper() : DLLWrapper(SDK_DLL_NAME)
        {
        }

        EOS_HPlatform start_eos() const;
    private:
        void init(const config::PlatformConfig& platform_config, const config::ProductConfig& product_config) const;

        EOS_HPlatform create(const config::PlatformConfig& platform_config, const config::ProductConfig& product_config) const;
    };
}

#endif
