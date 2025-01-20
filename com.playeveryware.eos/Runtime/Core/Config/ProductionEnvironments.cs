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

namespace PlayEveryWare.EpicOnlineServices
{
    using Common;

    /// <summary>
    /// This class contains information about the set of deployments and
    /// sandboxes that a single EOS plugin project can be configured to
    /// deploy to.
    /// </summary>
    public class ProductionEnvironments
    {
        /// <summary>
        /// Deployments are different environments within defined Sandboxes.
        /// One sandbox can have more than one Deployment.
        /// </summary>
        public SetOfNamed<Deployment> Deployments { get; } = new("Deployment");

        /// <summary>
        /// Sandboxes are different siloed categories of production environment.
        /// One sandbox can have more than one deployment.
        /// </summary>
        public SetOfNamed<SandboxId> Sandboxes { get; } = new("Sandbox");

        public ProductionEnvironments()
        {
            // Set the predicate for removing a sandbox so a sandbox that is
            // associated with a deployment does not get removed.
            Sandboxes.SetRemovePredicate(CanSandboxBeRemoved);
        }

        /// <summary>
        /// Tries to retrieve the first defined named deployment.
        /// </summary>
        /// <param name="deployment">
        /// The first named deployment that has been determined to be defined.
        /// </param>
        /// <returns>
        /// True if there was a defined named deployment found, false otherwise.
        /// </returns>
        public bool TryGetFirstDefinedNamedDeployment(out Named<Deployment> deployment)
        {
            deployment = null;

            // Go through the deployments
            foreach (var dep in Deployments)
            {
                // If the deployment is complete then stop
                if (dep.Value.IsComplete)
                {
                    deployment = dep;
                    break;
                }
            }

            return (deployment != null);
        }

        /// <summary>
        /// Removes a Sandbox from the Production Environment.
        /// </summary>
        /// <param name="sandbox">
        /// The Sandbox to remove from the production environment.
        /// </param>
        /// <returns>
        /// True if the Sandbox was removed, false otherwise. If there is a
        /// defined deployment that references the Sandbox, then removing it is
        /// disallowed.
        /// </returns>
        private bool CanSandboxBeRemoved(SandboxId sandbox)
        {
            foreach (Named<Deployment> deployment in Deployments)
            {
                if (deployment.Value.SandboxId.Equals(sandbox))
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Adds a Deployment to the Production Environment, adding the sandbox
        /// if it does not already exist in the set of sandboxes.
        /// </summary>
        /// <param name="deployment">
        /// The Deployment to add.
        /// </param>
        /// <returns>
        /// True if the deployment was added, false otherwise.
        /// </returns>
        public bool AddDeployment(Deployment deployment)
        {
            // Add the sandbox (will do nothing if the sandbox already exists).
            Sandboxes.Add(deployment.SandboxId);

            // Add the deployment to the list of deployments
            return Deployments.Add(deployment);
        }
    }
}