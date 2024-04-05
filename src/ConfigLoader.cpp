#include "ConfigLoader.h"
namespace lclog {
void ConfigLoader::createDefaultConfig(const std::string& config_file) {
    // 使用 std::filesystem 检查配置文件是否已经存在
    if (std::filesystem::exists(config_file)) {
        // 如果文件已经存在，我们不需要重新创建它
        return;
    }

    // 使用默认配置值创建一个 JSON 对象
    nlohmann::json configJson = {
        {"input_file", "/tmp/lclog_input.log"},
        {"output_file", "lclog_output.log"},
        {"log_id", "lclog"},
        {"log_level", static_cast<int>(LogLevel::Info)}, // 将 enum 转换为 int 以便序列化
        {"max_file_size", 1024},
        {"max_file_count", 10},
        {"log_to_file", true},
        {"daily_file", false}
    };

    // 尝试打开文件用于写入，这将创建文件
    std::ofstream configFile(config_file);
    if (!configFile.is_open()) {
        // 如果因为某些原因文件未能打开，可能需要处理错误
        throw std::runtime_error("Unable to open config file for writing.");
    }

    // 将 JSON 对象写入文件
    configFile << configJson.dump(4); // 使用 4 个空格的缩进格式化 JSON

    // 关闭文件
    configFile.close();
}
bool ConfigLoader::LoadConfig(const std::string& config_file) {
    std::ifstream ifs(config_file);
    if (!ifs.is_open()) {
        return false;
    }

    nlohmann::json configJson;
    try {
        ifs >> configJson;
    } catch (nlohmann::json::parse_error& e) {
        return false;
    }

    if (configJson.is_object()) {
        if (configJson.find("input_file") != configJson.end() && configJson["input_file"].is_string()) {
            _config.input_file = configJson["input_file"];
        }

        if (configJson.find("output_file") != configJson.end() && configJson["output_file"].is_string()) {
            _config.output_file = configJson["output_file"];
        }

        if (configJson.find("log_id") != configJson.end() && configJson["log_id"].is_string()) {
            _config.log_id = configJson["log_id"];
        }

        if (configJson.find("log_level") != configJson.end() && configJson["log_level"].is_number_integer()) {
            _config.log_level = static_cast<LogLevel>(configJson["log_level"].get<int>());
        }

        if (configJson.find("max_file_size") != configJson.end() && configJson["max_file_size"].is_number_integer()) {
            _config.max_file_size = configJson["max_file_size"];
        }

        if (configJson.find("max_file_count") != configJson.end() && configJson["max_file_count"].is_number_integer()) {
            _config.max_file_count = configJson["max_file_count"];
        }

        if (configJson.find("log_to_file") != configJson.end() && configJson["log_to_file"].is_boolean()) {
            _config.log_to_file = configJson["log_to_file"];
        }

        if (configJson.find("daily_file") != configJson.end() && configJson["daily_file"].is_boolean()) {
            _config.daily_file = configJson["daily_file"];
        }
    }

    return true;
}
} // namespace lclog

