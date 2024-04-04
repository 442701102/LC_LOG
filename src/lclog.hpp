#ifndef GLOBAL_LOGGER_H
#define GLOBAL_LOGGER_H
#include <string>
#include "env_def.h"
#include "Module/glog/LC_glog.h"
#define Derived Log_glog  
namespace lclog {
inline Derived& GlobalLogInstance() {
    return Derived::GetInstance();
}

void init(const std::string& config_file="") {
    LC_LOG_SETTING config;  
    config.input_file = "";
    config.output_file = "testglog";
    config.log_id = "glog1127";
    config.log_level = LogLevel::Info;
    config.max_file_size = 1024;
    config.max_log_count = 10;
    config.log_to_file = true;  
    GlobalLogInstance().init(config);
}


// 1. 流式日志接口
inline LCLoggerBase<Derived>::LogStream Log(LogLevel level) {
    return GlobalLogInstance().Log(level);
}
// 2. 格式化字符串日志接口
template<Formattable... Args>
void LogFormat(LogLevel level, std::string_view sformat, Args&&... args) {
    GlobalLogInstance().LogFormat(level, sformat, std::forward<Args>(args)...);
}
// 3. 字符串拼接日志接口
template<typename... Args>
void LogConcat(LogLevel level, Args&&... args) {
    GlobalLogInstance().LogConcat(level, std::forward<Args>(args)...);
}
// 4. printf 风格日志接口
inline void printf(LogLevel level, const char* format, ...) {
    va_list args;
    va_start(args, format);
    GlobalLogInstance().printf(level, format, args);
    va_end(args);
}

}
#endif // GLOBAL_LOGGER_H
