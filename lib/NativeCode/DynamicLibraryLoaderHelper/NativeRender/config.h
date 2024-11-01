#pragma once
#include <json.hpp>

static std::filesystem::path get_path_for_eos_service_config(std::string config_filename);
std::string sanatize_guid(const std::string& input);

namespace PlayEveryWare::EpicOnlineServices::Config
{
    struct EOSClientCredentials
    {
        std::string ClientId;
        std::string ClientSecret;
        std::string EncryptionKey;
    };
}

namespace nlohmann {
    template<>
    struct adl_serializer<EOS_Initialize_ThreadAffinity> {
        static void from_json(const json& j, EOS_Initialize_ThreadAffinity& ta) {
            ta.NetworkWork = j.value("NetworkWork", 0ULL);
            ta.StorageIo = j.value("StorageIo", 0ULL);
            ta.WebSocketIo = j.value("WebSocketIo", 0ULL);
            ta.P2PIo = j.value("P2PIo", 0ULL);
            ta.HttpRequestIo = j.value("HttpRequestIo", 0ULL);
            ta.RTCIo = j.value("RTCIo", 0ULL);
            ta.EmbeddedOverlayMainThread = j.value("EmbeddedOverlayMainThread", 0ULL);
            ta.EmbeddedOverlayWorkerThreads = j.value("EmbeddedOverlayWorkerThreads", 0ULL);
        }
    };

    template <>
    struct adl_serializer<PlayEveryWare::EpicOnlineServices::Config::EOSClientCredentials> {
        static void from_json(const json& j, PlayEveryWare::EpicOnlineServices::Config::EOSClientCredentials& creds) {
            creds.ClientId = j.at("ClientId").get<std::string>();
            creds.ClientSecret = j.at("ClientSecret").get<std::string>();
            creds.EncryptionKey = j.at("EncryptionKey").get<std::string>();
        }
    };
}

namespace PlayEveryWare::EpicOnlineServices::Config
{
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

    struct PlatformConfig {
        ProductionEnvironment deployment;
        EOSClientCredentials clientCredentials;
        std::string overrideCountryCode;
        std::string overrideLocaleCode;
        bool isServer = false;
        std::string platformOptionsFlags;
        std::string authScopeOptionsFlags;
        uint64_t flags = 0;
        uint32_t tickBudgetInMilliseconds = 0;
        double taskNetworkTimeoutSeconds = 0.0;
        bool alwaysSendInputToOverlay = false;
        double initialButtonDelayForOverlay = 0.0;
        double repeatButtonDelayForOverlay = 0.0;
        EOS_Initialize_ThreadAffinity thread_affinity;
        std::string toggleFriendsButtonCombination;
        bool _configValuesMigrated = false;
        std::string schemaVersion;
    };

    template <typename T>
    T get_value_or_default(const nlohmann::json& j, const std::string& key, const T& default_value) {
        if (j.contains(key) && !j.at(key).is_null()) {
            try {
                return j.at(key).get<T>();
            }
            catch (const nlohmann::json::exception& e) {
                // Handle type conversion errors if necessary
                std::cerr << "Error parsing key '" << key << "': " << e.what() << std::endl;
                return default_value;
            }
        }
        else {
            // Key does not exist or is null
            return default_value;
        }

    }

    struct DeploymentItem {
        std::string Name;
        ProductionEnvironment Value;
    };

    inline void from_json(const nlohmann::json& j, ProductionEnvironment& env) {
        env.DeploymentId = sanatize_guid(get_value_or_default<std::string>(j, "DeploymentId", ""));
        env.SandboxId = get_value_or_default<std::string>(j.at("SandboxId"), "Value", "");
    }


    inline void from_json(const nlohmann::json& j, DeploymentItem& item) {
        item.Name = get_value_or_default<std::string>(j, "Name", "");
        item.Value = j.at("Value").get<ProductionEnvironment>();
    }

    // Define from_json functions
    inline void from_json(const nlohmann::json& j, ProductConfig& config) {
        config.ProductName = get_value_or_default<std::string>(j, "ProductName", "");
        config.ProductId = sanatize_guid(get_value_or_default<std::string>(j, "ProductId", ""));
        config.ProductVersion = get_value_or_default<std::string>(j, "Version", "");

        const auto& environments = j.at("Environments");
        const auto& deployments = environments.at("Deployments");

        for (const auto& deploymentItemJson : deployments) {
            DeploymentItem deploymentItem = deploymentItemJson.get<DeploymentItem>();
            config.environments.push_back(deploymentItem.Value);
        }
    }

    inline void from_json(const nlohmann::json& j, EOS_Initialize_ThreadAffinity& ta)
    {
        ta.ApiVersion = EOS_INITIALIZE_THREADAFFINITY_API_LATEST;

        ta.StorageIo = get_value_or_default<uint64_t>(j, "StorageIo", 0ULL);
        ta.WebSocketIo = get_value_or_default<uint64_t>(j, "WebSocketIo", 0ULL);
        ta.P2PIo = get_value_or_default<uint64_t>(j, "P2PIo", 0ULL);
        ta.HttpRequestIo = get_value_or_default<uint64_t>(j, "HttpRequestIo", 0ULL);
        ta.RTCIo = get_value_or_default<uint64_t>(j, "RTCIo", 0ULL);
        ta.EmbeddedOverlayMainThread = get_value_or_default<uint64_t>(j, "EmbeddedOverlayMainThread", 0ULL);
        ta.EmbeddedOverlayWorkerThreads = get_value_or_default<uint64_t>(j, "EmbeddedOverlayWorkerThreads", 0ULL);
    }

    inline void from_json(const nlohmann::json& j, PlatformConfig& config) {
        // Using get_value_or_default for primitive types
        config.isServer = get_value_or_default<bool>(j, "isServer", false);
        config.platformOptionsFlags = get_value_or_default<std::string>(j, "platformOptionsFlags", "");
        config.authScopeOptionsFlags = get_value_or_default<std::string>(j, "authScopeOptionsFlags", "");
        config.flags = get_value_or_default<uint64_t>(j, "flags", 0);
        config.tickBudgetInMilliseconds = get_value_or_default<uint32_t>(j, "tickBudgetInMilliseconds", 0);
        config.taskNetworkTimeoutSeconds = get_value_or_default<double>(j, "taskNetworkTimeoutSeconds", 0.0);
        config.alwaysSendInputToOverlay = get_value_or_default<bool>(j, "alwaysSendInputToOverlay", false);
        config.initialButtonDelayForOverlay = get_value_or_default<double>(j, "initialButtonDelayForOverlay", 0.0);
        config.repeatButtonDelayForOverlay = get_value_or_default<double>(j, "repeatButtonDelayForOverlay", 0.0);
        config.toggleFriendsButtonCombination = get_value_or_default<std::string>(j, "toggleFriendsButtonCombination", "");
        config._configValuesMigrated = get_value_or_default<bool>(j, "_configValuesMigrated", false);
        config.schemaVersion = get_value_or_default<std::string>(j, "schemaVersion", "");

        // For nested objects, check existence before parsing
        if (auto it = j.find("deployment"); it != j.end() && !it->is_null()) {
            config.deployment = it->get<ProductionEnvironment>();
        }

        if (auto it = j.find("clientCredentials"); it != j.end() && !it->is_null()) {
            config.clientCredentials = it->get<EOSClientCredentials>();
        }

        if (auto it = j.find("threadAffinity"); it != j.end() && !it->is_null()) {
            config.thread_affinity = it->get<EOS_Initialize_ThreadAffinity>();
        }
    }
}



