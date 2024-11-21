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
 * THE SOFTWARE IS PROVIDED "AS IS"].get_to( WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <pch.h>
#include "headers/Config/PlatformConfig.h"
#include "headers/Config/common.hpp"
//#include "Config/nlohmann_helpers.hpp"

namespace pew::eos::config
{
    using namespace common;

    std::filesystem::path PlatformConfig::get_config_path(const char* file_name)
    {
        return absolute(get_path_relative_to_current_module(std::filesystem::path(
#ifdef _DEBUG
            "../../../../../../Assets/StreamingAssets/EOS/"
#endif
#ifdef NDEBUG
            "../../StreamingAssets/EOS/"
#endif
        ) / file_name));
    }

    void PlatformConfig::from_json(const json& json)
    {
        //auto json_str = json.dump();
        ////json["deployment"].get_to(                        deployment);
        ////json["clientCredentials"].get_to(                 client_credentials);
        ////json["isServer"].get_to(                          is_server);

        //// "platformOptionsFlags" is special, because it's value is a uint64_t,
        //// but the C# code translates it to a custom flag enum type and saves it
        //// into the json as a comma-delimited list of values. Attempting to
        //// parse that directly into an integer fails, so custom logic is
        //// required. A "from_json" function cannot be written for this scenario
        //// because the template parameter would be uint64_t, and parsing would
        //// fail.
        //std::string platform_options_flags_str;
        //platform_options_flags = 0;
        //json["platformOptionsFlags"].get_to(platform_options_flags_str);
        //flags_enum_from_string(platform_options_flags_str, PLATFORM_CREATION_FLAGS_STRING_TO_ENUM, platform_options_flags);
        //
        //json["authScopeOptionsFlags"].get_to(             auth_scope_flags);
        //json["integratedPlatformManagementFlags"].get_to( integrated_platform_management_flags);
        //json["tickBudgetInMilliseconds"].get_to(          tick_budget_in_milliseconds);
        //json["taskNetworkTimeoutSeconds"].get_to(         task_network_timeout_seconds);
        //json["threadAffinity"].get_to(                    thread_affinity);
        //json["alwaysSendInputToOverlay"].get_to(          always_send_input_to_overlay);
        //json["alwaysSendInputToOverlay"].get_to(          always_send_input_to_overlay);
        //json["initialButtonDelayForOverlay"].get_to(      initial_button_delay_for_overlay);
        //json["repeatButtonDelayForOverlay"].get_to(       repeat_button_delay_for_overlay);
        //json["toggleFriendsButtonCombination"].get_to(    toggle_friends_button_combination);
    }
}
