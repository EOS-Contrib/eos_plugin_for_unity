#ifndef PRODUCTION_ENVIRONMENTS_HPP
#define PRODUCTION_ENVIRONMENTS_HPP

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
#include <string>
#include <algorithm>
#include <vector>

#include "Serializable.hpp"

namespace pew::eos::config
{
    /**
     * \brief Used to describe the production environments to use for the EOS
     * SDK that are available to the product.
     */
    struct ProductionEnvironments final : Serializable
    {
        /**
         * \brief Used to describe the sandbox component of a deployment.
         */
        struct Sandbox final : Serializable
        {
            std::string id;

        protected:
            void parse_json_element(const std::string& name, json_value_s& value) override
            {
                if (name == "Value")
                {
                    const auto json_string = json_value_as_string(&value);
                    if (nullptr == json_string)
                    {
                        from_json(value);
                    }
                    else
                    {
                        id = json_string->string;
                    }
                }
            }
        };

        /**
         * \brief Used to describe a deployment for initializing the EOS SDK.
         */
        struct Deployment final : Serializable
        {
            std::string id;
            Sandbox sandbox;

        protected:
            void parse_json_element(const std::string& name, json_value_s& value) override
            {
                if (name == "Value")
                {
                    from_json(value);
                }
                else if (name == "DeploymentId")
                {
                    id = json_value_as_string(&value)->string;
                }
                else if (name == "SandboxId")
                {
                    auto sandbox_temp = Sandbox();
                    sandbox_temp.from_json(value);
                    sandbox = sandbox_temp;
                }
            }
        };

        /**
         * \brief The deployments defined for the product.
         */
        std::vector<Deployment> deployments;

        /**
         * \brief The sandboxes defined for the product.
         */
        std::vector<Sandbox> sandboxes;

        /**
         * \brief Determines if a given deployment id is defined in the product
         * config or not.
         * \param deployment_id The deployment to search for.
         * \return True if the deployment is defined, false otherwise.
         */
        bool is_deployment_defined(const std::string& deployment_id) const
        {
            return std::any_of(deployments.begin(), deployments.end(), [&deployment_id](const Deployment& d)
                {
                    return d.id == deployment_id;
                });
        }

        /**
         * \brief Determines if a given sandbox is defined in the product config
         * or not.
         * \param sandbox_id The sandbox id to search for.
         * \return True if the sandbox is defined, false otherwise.
         */
        bool is_sandbox_defined(const std::string& sandbox_id) const
        {
            return std::any_of(sandboxes.begin(), sandboxes.end(), [&sandbox_id](const Sandbox& s)
                {
                    return s.id == sandbox_id;
                });
        }

    protected:
        friend struct ProductConfig;

        void parse_json_element(const std::string& name, json_value_s& value) override
        {
            if (name == "Deployments")
            {
                deployments = parse_json_array<Deployment>(value);
            }
            else if (name == "Sandboxes")
            {
                sandboxes = parse_json_array<Sandbox>(value);
            }
        }
    };
}

#endif
