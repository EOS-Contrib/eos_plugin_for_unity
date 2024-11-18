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
#include <iostream>
#include "../Config/PlatformConfig.h"
#include "string_helpers.h"
#include "../include/json.hpp"
#include "EOSJsonConverterMethods.h"
#include <EOSJsonConverterMethods.cpp>

namespace pew::eos::config
{
    void PlatformConfig::migrate()
    {
        // Migrate the platform configuration if needed. Currently, this
        // scaffolding is in-place for future-proofing.
    }

    void PlatformConfig::from_json(const nlohmann::json& json)
    {
        PARSE_FROM_JSON("deployment",                        deployment);
        PARSE_FROM_JSON("clientCredentials",                 client_credentials);
        PARSE_FROM_JSON("isServer",                          is_server);

        // "platformOptionsFlags" is special, because it's value is a uint64_t,
        // but the C# code translates it to a custom flag enum type and saves it
        // into the json as a comma-delimited list of values. Attempting to
        // parse that directly into an integer fails, so custom logic is
        // required. A "from_json" function cannot be written for this scenario
        // because the template parameter would be uint64_t, and parsing would
        // fail.
        const std::string platform_options_flags_str;
        if(try_get_to(json, "platformOptionsFlags", platform_options_flags_str))
        {
            flags_enum_from_string(platform_options_flags_str, PLATFORM_CREATION_FLAGS_STRING_TO_ENUM, platform_options_flags);
        }


        PARSE_FROM_JSON("authScopeOptionsFlags",             auth_scope_flags);
        PARSE_FROM_JSON("integratedPlatformManagementFlags", integrated_platform_management_flags);
        PARSE_FROM_JSON("tickBudgetInMilliseconds",          tick_budget_in_milliseconds);
        PARSE_FROM_JSON("taskNetworkTimeoutSeconds",         task_network_timeout_seconds);
        PARSE_FROM_JSON("threadAffinity",                    thread_affinity);
        PARSE_FROM_JSON("alwaysSendInputToOverlay",          always_send_input_to_overlay);
        PARSE_FROM_JSON("initialButtonDelayForOverlay",      initial_button_delay_for_overlay);
        PARSE_FROM_JSON("repeatButtonDelayForOverlay",       repeat_button_delay_for_overlay);
        PARSE_FROM_JSON("toggleFriendsButtonCombination",    toggle_friends_button_combination);
    }
}
