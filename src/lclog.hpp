#ifndef GLOBAL_LOGGER_H
#define GLOBAL_LOGGER_H
#include <string>
#include "env_def.h"
#include "Module/glog/LC_glog.h"

namespace lclog {
void init(const std::string& config_file="") {
    LC_LOG_SETTING config;  
    config.input_file = "";
    config.output_file = "testglog";
    config.log_id = "glog1127";
    config.log_level = LogLevel::Info;
    config.max_file_size = 1024;
    config.max_log_count = 10;
    config.log_to_file = true;  
    Log_glog::GetInstance().init(config);
}

// 全局日志函数
// template<typename... Args>
// void Log(lclog::LogLevel level, const std::string& format, Args&&... args) {
//     Log_glog::GetInstance().Log(level, format, std::forward<Args>(args)...);
// }

void printf(LogLevel level, const char* format, ...){
    Log_glog::GetInstance().printf(level, format);  
}
}
#endif // GLOBAL_LOGGER_H
