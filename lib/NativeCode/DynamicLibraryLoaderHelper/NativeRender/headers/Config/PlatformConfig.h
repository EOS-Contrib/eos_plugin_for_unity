#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

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

#include "eos_init.h"
#include "ProductionEnvironments.hpp"
#include "headers/Config/ClientCredentials.hpp"
#include "headers/Config/ConfigBase.h"

namespace pew::eos::config
{
    /**
     * \brief Describes the configuration options for a platform that wants to
     * interact with the EOS SDK.
     */
    class PlatformConfig : public ConfigBase
    {
    protected:
        explicit PlatformConfig(const char* file_name) : ConfigBase(PlatformConfig::get_config_path(file_name)),
             is_server(false),
             platform_options_flags(0),
             tick_budget_in_milliseconds(0),
             task_network_timeout_seconds(0),
             thread_affinity(),
             always_send_input_to_overlay(false),
             initial_button_delay_for_overlay(0),
             repeat_button_delay_for_overlay(0)
        {
        }

        std::filesystem::path get_config_path(const char* file_name) override;

        void from_json(const nlohmann::json& json) override;

        friend class ConfigBase;
    public:
        virtual ~PlatformConfig() = default;
        /**
         * \brief The deployment for the platform.
         */
        ProductionEnvironments::Deployment deployment;

        /**
         * \brief The client credentials to use for the platform.
         */
        ClientCredentials client_credentials;

        /**
         * \brief Whether or not the game is a dedicated game server.
         */
        bool is_server;

        /**
         * \brief Result of a bit-wise or operation combining all platform
         * flags.
         */
        int platform_options_flags;

        /**
         * \brief The AuthScopeFlags for the platform.
         */
        EOS_EAuthScopeFlags auth_scope_flags = EOS_EAuthScopeFlags::EOS_AS_NoFlags;

        /**
         * \brief Integrated platform management flags for the platform.
         */
        EOS_EIntegratedPlatformManagementFlags integrated_platform_management_flags = EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_Disabled;

        /**
         * \brief The maximum amount of execution time the EOS SDK can use each
         * frame.
         */
        int tick_budget_in_milliseconds;

        /**
         * \brief Indicates the maximum number of seconds that (before first
         * coming only) the EOS SDK will allow network calls to run before
         * failing with EOS_TimedOut. This value does not apply after the EOS
         * SDK has been initialized.
         */
        double task_network_timeout_seconds;

        /**
         * \brief Defines the thread affinity for threads started by the EOS
         * SDK. Leave values at zero to use default platform settings.
         */
        EOS_Initialize_ThreadAffinity thread_affinity;

        /**
         * \brief If true, the plugin will always send input to the overlay from
         * the C# side to native side, and handle showing the overlay. This
         * doesn't always mean input makes it to the EOS SDK.
         */
        bool always_send_input_to_overlay;

        /**
         * \brief Initial button delay (if not set, whatever the default is will
         * be used).
         */
        float initial_button_delay_for_overlay;

        /**
         * \brief Repeat button delay for the overlay. If not set, whatever the
         * default is will be used.
         */
        float repeat_button_delay_for_overlay;

        /**
         * \brief Users can press the button(s) associated with this value to
         * activate the Epic Social Overlay. Not all combinations are valid; the
         * SDK will log an error at the start of runtime if an invalid
         * combination is selected.
         */
        EOS_UI_EInputStateButtonFlags toggle_friends_button_combination = EOS_UI_EInputStateButtonFlags::EOS_UISBF_None;

        /**
         * \brief The country code to use when creating the eos platform. Note
         * that the Unity project does not currently expose this property for
         * the user to edit. TODO: Expose property in Unity project.
         */
        std::string overrideCountryCode;

        /**
         * \brief The locale code to use when creating the eos platform. Note
         * that the Unity project does not currently expose this property for
         * the user to edit. TODO: Expose property in Unity project.
         */
        std::string overrideLocaleCode;
    };

}

#endif
