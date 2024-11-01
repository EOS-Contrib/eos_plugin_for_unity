#pragma once

struct ProductionEnvironment
{
    std::string DeploymentId;
    std::string SandboxId;
};

struct ProductConfig
{
    std::string ProductName;
    std::string ProductId;
    std::string ProductVersion;

    std::vector<ProductionEnvironment> environments;
};

struct EOSClientCredentials
{
    std::string ClientId;
    std::string ClientSecret;
    std::string EncryptionKey;
};

struct PlatformConfig
{
    ProductionEnvironment deployment;

    EOSClientCredentials clientCredentials;

    std::string overrideCountryCode;
    std::string overrideLocaleCode;

    // this is called platformOptionsFlags in C#
    uint64_t flags = 0;

    uint32_t tickBudgetInMilliseconds = 0;
    double taskNetworkTimeoutSeconds = 0.0;

    EOS_Initialize_ThreadAffinity thread_affinity;

    bool isServer = false;
};
