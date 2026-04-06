/*
 * Copyright (c) 2026 Epic Games Inc
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

namespace PlayEveryWare.EpicOnlineServices.Tests.Config
{
    using NUnit.Framework;
    using PlayEveryWare.Common;
    using System;
    using System.Collections.Generic;
    using System.IO;
    using UnityEngine;

    public class ProductConfigTests
    {
        [Test]
        public void SandboxId_IsInvalid_WhenValueIsNull()
        {
            SandboxId sandboxId = new SandboxId();

            Assert.IsFalse(sandboxId.IsValid(), "SandboxId should not be valid when its value is null.");
        }

        [Test]
        public void SandboxId_IsInvalid_WhenValueIsEmptyString()
        {
            SandboxId sandboxId = new SandboxId
            {
                Value = string.Empty
            };

            Assert.IsFalse(sandboxId.IsValid(), "SandboxId should not be valid when its value is empty.");
        }
        
        [Test]
        public void SandboxId_IsInvalid_WhenValueIsWhiteSpace()
        {
            SandboxId sandboxId = new SandboxId
            {
                Value = " "
            };

            Assert.IsFalse(sandboxId.IsValid(), "SandboxId should not be valid when its value is whitespace only.");
        } 

        [Test]
        public void SandboxId_GuidEmptyString_IsEmptyButValid()
        {
            SandboxId sandboxId = new SandboxId
            {
                Value = Guid.Empty.ToString("N")
            };

            Assert.IsTrue(sandboxId.IsValid(), "Guid.Empty is a valid GUID format and should be accepted.");
            Assert.IsTrue(sandboxId.IsEmpty, "Guid.Empty represents an unconfigured SandboxId.");
        }


        [Test]
        public void SandboxId_IsValid_WhenValueIsGuid()
        {
            SandboxId sandboxId = new SandboxId
            {
                Value = Guid.NewGuid().ToString()
            };

            Assert.IsTrue(sandboxId.IsValid(), "Guid.NewGuid.ToStrign() is a valid GUID format and should be accepted.");
        }

        [Test]
        public void SandboxId_IsValid_WhenGuidIsWithoutDashes()
        {
            SandboxId sandboxId = new SandboxId
            {
                Value = Guid.NewGuid().ToString("N")
            };

            Assert.IsTrue(sandboxId.IsValid(), "Guid.NewGuid.ToString(\"N\") is a valid GUID format and should be accepted Without Dashes.");
        }

        [Test]
        public void SandboxId_IsValid_WhenUsingPreProductionFormat()
        {
            string value = "p-" + new string('a', 30);

            SandboxId sandboxId = new SandboxId { Value = value };

            Assert.IsTrue(sandboxId.IsValid(), "'p-' preproduction format should be accepted format.");
        }

        [Test]
        public void SandboxId_IsInvalid_WhenIsTooShort()
        {
            string value = new string('a', 10);

            SandboxId sandboxId = new SandboxId { Value = value };

            Assert.IsFalse(sandboxId.IsValid(), "Short sandbox should not be accepted.");
        }

        [Test]
        public void SandboxId_DoesNotAccept_InvalidString()
        {
            SandboxId sandboxId = new SandboxId();
            sandboxId.Value = "abc";

            Assert.IsTrue(sandboxId.IsEmpty,"Sandbox should not accepted invalid string format.");
            Assert.IsFalse(sandboxId.IsValid(),"Empty Sandbox sould not be accepted as valid.");
        }

        [Test]
        public void SandboxId_KeepsPreviousValue_WhenNewValueIsInvalid()
        {
            string valid = Guid.NewGuid().ToString();

            SandboxId sandboxId = new SandboxId { Value = valid };
            sandboxId.Value = "abc";

            Assert.AreEqual(valid, sandboxId.ToString(), "SandboxID should keeps previous value when new value is invalid.");
        }

        [Test]
        public void SandboxId_DoesNotThrow_WhenValueIsInvalid()
        {
            SandboxId sandboxId = new SandboxId();

            Assert.DoesNotThrow(() =>
            {
                sandboxId.Value = "not-a-sandbox-id";
            });
        }


        [Test]
        public void ProductName_MustNotBeEmpty()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();
            Assert.NotNull(productConfig, "Product Config should not be null.");
            productConfig.ProductName = string.Empty;

            if (!FieldValidator.TryGetFailingValidatorAttributes(productConfig, out List<FieldValidatorFailure> failingAttributes))
            {
                Assert.Fail($"Config should have failing attributes.");
            }

            Assert.IsTrue(failuresIncludeExpectedFailure<NonEmptyStringFieldValidatorAttribute>(nameof(ProductConfig.ProductName),
                failingAttributes,
                NonEmptyStringFieldValidatorAttribute.FieldIsEmptyMessage),
                "There should be a failure of the expected type and message.");
        }

        [Test]
        public void ProductName_PassesValidation_WhenValueIsValid()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();
            Assert.NotNull(productConfig, "Product Config should not be null.");
            productConfig.ProductName = "My Valid Product Name";

            if (!FieldValidator.TryGetFailingValidatorAttributes(productConfig, out List<FieldValidatorFailure> failingAttributes))
            {
                // If there are no errors, then this test is a success
                // The productConfig might be failing in other ways
                return;
            }

            Assert.IsFalse(failuresIncludeExpectedFailure<NonEmptyStringFieldValidatorAttribute>(
                nameof(ProductConfig.ProductName),
                failingAttributes,
                NonEmptyStringFieldValidatorAttribute.FieldIsEmptyMessage),
                "Product Name should not have errors describing it as an empty field.");
        }

        [Test]
        public void ProductVersion_FailsValidation_WhenValueIsEmpty()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();
            Assert.NotNull(productConfig, "Product Config should not be null.");
            productConfig.ProductVersion = string.Empty;

            if (!FieldValidator.TryGetFailingValidatorAttributes(productConfig, out List<FieldValidatorFailure> failingAttributes))
            {
                Assert.Fail($"Config should have failing attributes.");
            }

            Assert.IsTrue(failuresIncludeExpectedFailure<NonEmptyStringFieldValidatorAttribute>(
                nameof(ProductConfig.ProductVersion),
                failingAttributes,
                NonEmptyStringFieldValidatorAttribute.FieldIsEmptyMessage),
                "There should be a failure of the expected type and message.");
        }

        [Test]
        public void ProductVersion_SuccessfulParsing()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();
            Assert.NotNull(productConfig, "Product Config should not be null.");
            productConfig.ProductVersion = "123.456";

            if (!FieldValidator.TryGetFailingValidatorAttributes(productConfig, out List<FieldValidatorFailure> failingAttributes))
            {
                // If there are no errors, then this test is a success
                // The productConfig might be failing in other ways
                return;
            }

            Assert.IsFalse(failuresIncludeExpectedFailure<NonEmptyStringFieldValidatorAttribute>(
                nameof(ProductConfig.ProductVersion),
                failingAttributes,
                NonEmptyStringFieldValidatorAttribute.FieldIsEmptyMessage),
                "Product Version should not have errors describing it as an empty field.");
        }
        [Test]
        public void ProductId_MustBeValidGUID()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();
            Assert.NotNull(productConfig, "Product Config should not be null.");
            productConfig.ProductId = Guid.Empty;

            if (!FieldValidator.TryGetFailingValidatorAttributes(productConfig, out List<FieldValidatorFailure> failingAttributes))
            {
                Assert.Fail($"Config should have failing attributes.");
            }

            Assert.IsTrue(failuresIncludeExpectedFailure<GUIDFieldValidatorAttribute>(
                nameof(ProductConfig.ProductId),
                failingAttributes,
                GUIDFieldValidatorAttribute.EmptyGuidMessage),
                "ProductId should fail when Guid.Empty is used.");
        }


        [Test]
        public void ProductId_SuccessfulValidation()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();
            Assert.NotNull(productConfig);

            productConfig.ProductId = Guid.NewGuid();

            bool hasFailures = FieldValidator.TryGetFailingValidatorAttributes(productConfig, out _);

            Assert.IsFalse(hasFailures, "Valid ProductId should not fail validation.");
        }

        [Test]
        public void ProductionEnvironments_MustHaveAtLeastOneValidDeployment()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();
            Assert.NotNull(productConfig);

            productConfig.Environments = new ProductionEnvironments();

            if (!FieldValidator.TryGetFailingValidatorAttributes(
                productConfig, out List<FieldValidatorFailure> failures))
            {
                Assert.Fail("Config should have failing attributes.");
            }

            Assert.IsTrue(
                failuresIncludeExpectedFailure<ProductionEnvironmentsFieldValidatorAttribute>(
                    nameof(ProductConfig.Environments),
                    failures,
                    ProductionEnvironmentsFieldValidatorAttribute.NoValidDeploymentMessage),
                "There should be a failure when no valid deployments exist.");
        }

        [Test]
        public void ProductionEnvironments_WithIncompleteDeployment_ShouldFail()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();
            productConfig.Environments = new ProductionEnvironments();

            Deployment incompleteDeployment = new Deployment
            {
                DeploymentId = Guid.Empty,
                SandboxId = new SandboxId { Value = "abc" }
            };

            productConfig.Environments.AddDeployment(incompleteDeployment);

            Assert.IsTrue(FieldValidator.TryGetFailingValidatorAttributes(productConfig, out _), "Config should have failing attributes.");
        }

        [Test]
        public void ProductionEnvironments_WithValidDeployment_ShouldPass()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();

            Deployment validDeployment = new Deployment
            {
                DeploymentId = Guid.NewGuid(),
                SandboxId = new SandboxId
                {
                    Value = Guid.NewGuid().ToString()
                }
            };

            productConfig.Environments.AddDeployment(validDeployment);

            Assert.IsFalse(FieldValidator.TryGetFailingValidatorAttributes(productConfig, out _), "Deployment should be complete when both DeploymentId and SandboxId are valid");
        }

        [Test]
        public void ClientCredentials_MustHaveAtLeastOneValidClient()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();
            productConfig.Clients = new SetOfNamed<EOSClientCredentials>();

            if (!FieldValidator.TryGetFailingValidatorAttributes(
                productConfig, out List<FieldValidatorFailure> failures))
            {
                Assert.Fail("Config should have failing attributes.");
            }

            Assert.IsTrue(
                failuresIncludeExpectedFailure<ClientCredentialsFieldValidatorAttribute>(
                    nameof(ProductConfig.Clients),
                    failures,
                    ClientCredentialsFieldValidatorAttribute.NoValidClientCredentialsMessage));
        }


        [Test]
        public void ClientCredentials_WithValidClient_ShouldPass()
        {
            ProductConfig productConfig = ProductConfig.Get<ProductConfig>();

            productConfig.Clients.Add(new EOSClientCredentials(
                "validClientId",
                "validClientSecret",
                null));

            Assert.IsFalse(FieldValidator.TryGetFailingValidatorAttributes(productConfig, out _), "ClientCredentials should be complete when both ClientID and ClientSecret are valid");
        }


        [Test]
        public void Deployment_IsIncomplete_WhenDeploymentIdIsEmpty()
        {
            Deployment deployment = new Deployment
            {
                DeploymentId = Guid.Empty,
                SandboxId = new SandboxId
                {
                    Value = Guid.NewGuid().ToString()
                }
            };

            Assert.IsFalse(deployment.IsComplete, "Deployment should be incomplete when DeploymentId is Guid.Empty.");
        }


        [Test]
        public void Deployment_IsIncomplete_WhenSandboxIdIsEmpty()
        {
            Deployment deployment = new Deployment
            {
                DeploymentId = Guid.NewGuid(),
                SandboxId = new SandboxId()
            };

            Assert.IsFalse(deployment.IsComplete, "Deployment should be incomplete when SandboxId is empty.");
        }


        [Test]
        public void Deployment_IsComplete_WhenDeploymentIdAndSandboxIdAreValid()
        {
            Deployment deployment = new Deployment
            {
                DeploymentId = Guid.NewGuid(),
                SandboxId = new SandboxId
                {
                    Value = Guid.NewGuid().ToString()
                }
            };

            Assert.IsTrue(deployment.IsComplete, "Deployment should be complete when both DeploymentId and SandboxId are valid.");
        }


        /// <summary>
        /// Determines if the provided list of failures contains an expected failure
        /// </summary>
        /// <typeparam name="T">The kind of validator attribute that is expected.</typeparam>
        /// <param name="fieldName">The name of the field that is being checked for.</param>
        /// <param name="failures">A list of all failures from validation.</param>
        /// <param name="message">
        /// A specific error message to check for.
        /// Optional. If null or empty, this is not used.
        /// </param>
        /// <returns>True if the expected failure is within the list.</returns>
        private bool FailuresIncludeExpectedFailure<T>(string fieldName, List<FieldValidatorFailure> failures, string message = "") where T : FieldValidatorAttribute
        {
            foreach (FieldValidatorFailure currentFailure in failures)
            {
                if (currentFailure.FailingAttribute is not T)
                {
                    continue;
                }

                if (currentFailure.FieldInfo.Name != fieldName)
                {
                    continue;
                }

                if (!string.IsNullOrEmpty(message))
                {
                    if (currentFailure.FailingMessage != message)
                    {
                        continue;
                    }
                }

                return true;
            }

            return false;
        }
    }
}
