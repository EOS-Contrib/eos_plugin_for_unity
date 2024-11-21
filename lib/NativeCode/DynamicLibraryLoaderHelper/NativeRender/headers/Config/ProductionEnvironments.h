#ifndef PRODUCTION_ENVIRONMENTS_H
#define PRODUCTION_ENVIRONMENTS_H

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
#include <vector>

#include "headers/Config/Deployment.h"
#include "headers/Config/Sandbox.h"

namespace pew::eos::config
{
    /**
     * \brief Used to describe the production environments to use for the EOS
     * SDK that are available to the product.
     */
    struct ProductionEnvironments
    {
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
            return std::any_of(deployments.begin(), deployments.end(), [deployment_id](const Deployment& d)
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
            return std::any_of(sandboxes.begin(), sandboxes.end(), [sandbox_id](const Sandbox& s)
                {
                    return s.id == sandbox_id;
                });
        }
    };
}

#endif
