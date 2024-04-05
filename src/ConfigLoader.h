#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include "env_def.h"    
#include <string>
#include <iostream> 
#include <fstream>
#include <filesystem>   
#include <nlohmann/json.hpp>

namespace lclog {
class ConfigLoader {    
public:
    virtual ~ConfigLoader() = default;
    ConfigLoader(const ConfigLoader&) = delete;
    ConfigLoader& operator=(const ConfigLoader&) = delete;

    static ConfigLoader& GetInstance() {
        static ConfigLoader instance;
        return instance;
    }


    inline static constexpr const char* default_config = "/tmp/lclog.json";

    //1. 创建默认配置文件
    void createDefaultConfig(const std::string& config_file = default_config);   

    bool LoadConfig(const std::string& config_file = default_config);

    LCLog_cfg_st getConfig() const { return _config; }
    bool getConfig(LCLog_cfg_st& config) const { config = _config; return true; }
private:
    ConfigLoader():_config(){
        _config.input_file = "/tmp/lclog_input.log";
        _config.output_file = "lclog_output.log";
        _config.log_id = "lclog";
        _config.log_level = LogLevel::Info; 
        _config.max_file_size = 4096;   
        _config.max_file_count = 10;    
        _config.log_to_file = true;
        _config.daily_file = false;
    } 
    LCLog_cfg_st _config;
};      

} // namespace lclog













#endif // CONFIGLOADER_H    