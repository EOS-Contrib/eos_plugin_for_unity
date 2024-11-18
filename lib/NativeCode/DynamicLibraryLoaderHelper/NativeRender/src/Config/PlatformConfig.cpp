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

namespace pew::eos::config
{
    void PlatformConfig::migrate()
    {
        // Migrate the platform configuration if needed
    }

    void PlatformConfig::from_json(const nlohmann::json& json)
    {
        auto json_str = json.dump();

        json["deployment"].get_to(deployment);
        
        json["clientCredentials"].get_to(client_credentials);
        json["isServer"].get_to(is_server);
        
        json["authScopeOptionsFlags"].get_to(auth_scope_flags);
        
        // TODO: Make C# use this string instead of the generic "flags".
        //       Also make the value output by the C# to be a comma-delimited 
        //       list of values.
        //json["integratedPlatformManagementFlags"].get_to(integrated_platform_management_flags);

        json["tickBudgetInMilliseconds"].get_to(tick_budget_in_milliseconds);
        json["taskNetworkTimeoutSeconds"].get_to(task_network_timeout_seconds);

        // TODO: Make sure that ApiVersion is being serialized by the C#, because currently it is not
        json["threadAffinity"].get_to(thread_affinity);
        json["alwaysSendInputToOverlay"].get_to(always_send_input_to_overlay);
        json["initialButtonDelayForOverlay"].get_to(initial_button_delay_for_overlay);
        json["repeatButtonDelayForOverlay"].get_to(repeat_button_delay_for_overlay);
        json["toggleFriendsButtonCombination"].get_to(toggle_friends_button_combination);

        std::cout << json_str << std::endl;
    }
}
