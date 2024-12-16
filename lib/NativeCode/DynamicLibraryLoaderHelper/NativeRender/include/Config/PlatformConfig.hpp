#ifndef PLATFORM_CONFIG_HPP
#define PLATFORM_CONFIG_HPP

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

#include <unordered_map>
#include "config_legacy.h"
#include "EnumMappings.h"
#include "eos_init.h"
#include "eos_auth_types.h"
#include "eos_ui_types.h"
#include "json_helpers.h"
#include "ProductionEnvironments.hpp"
#include "Config/ClientCredentials.hpp"
#include "Config/Config.hpp"

namespace pew::eos::config
{
    /**
     * \brief Describes the configuration options for a platform that wants to
     * interact with the EOS SDK.
     */
    struct PlatformConfig : Config
    {
    public:
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

    protected:
        explicit PlatformConfig(const char* file_name) : Config(file_name),
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

        void parse_json_element(const std::string& name, json_value_s& value) override
        {
            if (name == "deployment")
            {
                deployment.from_json(value);
            }
            else if (name == "clientCredentials")
            {
                client_credentials.from_json(value);
            }
            else if (name == "isServer")
            {
                is_server = parse_bool(value);
            }
            else if (name == "platformOptionsFlags")
            {
                platform_options_flags = parse_flags<int>(
                    &PLATFORM_CREATION_FLAGS_STRINGS_TO_ENUM, 
                    0, 
                    &value);
            }
            else if (name == "authScopeOptionsFlags")
            {
                auth_scope_flags = parse_flags<EOS_EAuthScopeFlags>(
                    &AUTH_SCOPE_FLAGS_STRINGS_TO_ENUM, 
                    EOS_EAuthScopeFlags::EOS_AS_NoFlags, 
                    &value);
            }
            else if (name == "integratedPlatformManagementFlags")
            {
                integrated_platform_management_flags = 
                    parse_flags<EOS_EIntegratedPlatformManagementFlags>(
                        &config_legacy::INTEGRATED_PLATFORM_MANAGEMENT_FLAGS_STRINGS_TO_ENUM, 
                        EOS_EIntegratedPlatformManagementFlags::EOS_IPMF_Disabled, 
                        &value);
            }
            else if (name == "tickBudgetInMilliseconds")
            {
                tick_budget_in_milliseconds = parse_number<int>(value);
            }
            else if (name == "taskNetworkTimeoutSeconds")
            {
                task_network_timeout_seconds = parse_number<double>(value);
            }
            else if (name == "threadAffinity")
            {
                const auto thread_affinity_json_object = *static_cast<json_object_s*>(value.payload);
                auto thread_affinity_iterator = thread_affinity_json_object.start;

                // TODO: Figure out how to implement this clean approach with
                //       all the properties of any class deriving from
                //       Serializable.
                std::unordered_map<std::string, uint64_t*> affinity_map = {
                    {"NetworkWork", &thread_affinity.NetworkWork},
                    {"StorageIo", &thread_affinity.StorageIo},
                    {"WebSocketIo", &thread_affinity.WebSocketIo},
                    {"P2PIo", &thread_affinity.P2PIo},
                    {"HttpRequestIo", &thread_affinity.HttpRequestIo},
                    {"RTCIo", &thread_affinity.RTCIo},
                    {"EmbeddedOverlayMainThread", &thread_affinity.EmbeddedOverlayMainThread},
                    {"EmbeddedOverlayWorkerThreads", &thread_affinity.EmbeddedOverlayWorkerThreads},
                };

                while(thread_affinity_iterator)
                {
                    const std::string element_name = thread_affinity_iterator->name->string;

                    if (affinity_map.find(element_name) != affinity_map.end() && thread_affinity_iterator->value != nullptr)
                    {
                        auto element = *(thread_affinity_iterator->value);
                        *affinity_map[element_name] = parse_number<uint64_t>(element);
                    }

                    thread_affinity_iterator = thread_affinity_iterator->next;
                }

                // TODO: Confirm that setting this version to the latest every
                //       time won't be a problem for users if their config was
                //       serialized with an older version.
                thread_affinity.ApiVersion = EOS_INITIALIZE_THREADAFFINITY_API_LATEST;
                
            }
            else if (name == "alwaysSendInputToOverlay")
            {
                always_send_input_to_overlay = parse_bool(value);
            }
            else if (name == "initialButtonDelayForOverlay")
            {
                initial_button_delay_for_overlay = parse_number<float>(value);
            }
            else if (name == "repeatButtonDelayForOverlay")
            {
                repeat_button_delay_for_overlay = parse_number<float>(value);
            }
            else if (name == "toggleFriendsButtonCombination")
            {
                // Handle toggleFriendsButtonCombination
                toggle_friends_button_combination = parse_flags<EOS_UI_EInputStateButtonFlags>(
                    &INPUT_STATE_BUTTON_FLAGS_STRINGS_TO_ENUM,
                    EOS_UI_EInputStateButtonFlags::EOS_UISBF_None,
                    &value);
            }
        }

        friend struct Config;
        virtual ~PlatformConfig() = default;
    };
}

#endif
