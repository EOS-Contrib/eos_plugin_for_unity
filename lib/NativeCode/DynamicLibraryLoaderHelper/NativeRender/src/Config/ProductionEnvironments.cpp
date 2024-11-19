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
#include "ProductionEnvironments.h"
#include "Sandbox.h"
#include "Deployment.h"

namespace pew::eos::config
{
    bool ProductionEnvironments::is_deployment_defined(const std::string& deployment_id) const
    {
        for(const Deployment& deployment : deployments)
        {
            if (deployment.id == deployment_id)
            {
                return true;
            }
        }

        return false;
    }

    bool ProductionEnvironments::is_sandbox_defined(const std::string& sandbox_id) const
    {
        for(const Sandbox& sandbox : sandboxes)
        {
            if (sandbox.id == sandbox_id)
            {
                return true;
            }
        }

        return false;
    }


}

namespace nlohmann
{
    using namespace pew::eos::config;

    void from_json(const nlohmann::json& json, ProductionEnvironments& environments)
    {
        environments.sandboxes = json["Sandboxes"].get<std::vector<Sandbox>>();
        environments.deployments = json["Deployments"].get < std::vector<Deployment>>();
    }
}

