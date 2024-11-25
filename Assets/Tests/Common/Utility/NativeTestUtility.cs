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

//#if EXTERNAL_TO_UNITY
namespace PlayEveryWare.EpicOnlineServices.Tests.Utility
{
    using Epic.OnlineServices.Platform;
    using Extensions;


    public class NativeTestUtility
    {
        // TODO: Make sure this is literally the same code that EOSManager uses, so
        //       that the testing actually reflects the correct thing.
        public static WindowsOptions GetWindowsOptions()
        {
            WindowsOptions options = new WindowsOptions();

            ProductConfig productConfig = Config.Get<ProductConfig>();
            WindowsConfig windowsConfig = Config.Get<WindowsConfig>();

            options.DeploymentId = windowsConfig.deployment.DeploymentId.ToString("N").ToLowerInvariant();
            options.SandboxId = windowsConfig.deployment.SandboxId.ToString();
            options.ProductId = productConfig.ProductId.ToString("N").ToLowerInvariant();
            options.ClientCredentials = new ClientCredentials()
            {
                ClientId = windowsConfig.clientCredentials.ClientId,
                ClientSecret = windowsConfig.clientCredentials.ClientSecret
            };
            options.EncryptionKey = windowsConfig.clientCredentials.EncryptionKey;
            
            options.Flags = windowsConfig.platformOptionsFlags.Unwrap();
            options.IsServer = windowsConfig.isServer;
            options.TickBudgetInMilliseconds = windowsConfig.tickBudgetInMilliseconds;
            options.TaskNetworkTimeoutSeconds = windowsConfig.taskNetworkTimeoutSeconds;

            return options;
        }

        public static InitializeOptions GetInitializeOptions()
        {
            InitializeOptions options = new InitializeOptions();

            return options;
        }
    }
}
//#endif