#ifndef GLOBAL_LOGGER_H
#define GLOBAL_LOGGER_H
#include <string>
#include "env_def.h"



#include "config.h" // Include configuration generated by CMake


#if defined(USE_STDOUT_LOGGER)
#include "StdoutLogger.h"
#define Derived  StdoutLogger
#elif defined(USE_SPDLOG_LOGGER)
#include "Module/spdlog/LC_spdlog.h"
#define Derived spdlogLogger  
#elif defined(USE_GLOG_LOGGER)
#include "Module/glog/LC_glog.h"
#define Derived glogLogger  
#elif defined(USE_LOG4CPLUS_LOGGER)
#include "Module/log4cplus/LC_log4cplus.h"
#define Derived log4cplusLogger
#elif defined(USE_DLT_LOGGER)
#include "Module/dlt/LC_dlt.h"  
#define Derived dltLogger
#else
#error "No logger type defined!"
#endif

// 模板函数来检查GetInstance方法是否存在并返回正确的类型
template<typename T>
auto TestGetInstance() -> decltype(T::GetInstance(), std::true_type{}) {
    static_assert(std::is_same<decltype(T::GetInstance()), T&>::value,
                  "GetInstance must return a reference to T");
    return std::true_type{};
}

// 用来在编译时强制检查的宏
#define CHECK_CLASS_TYPE(T) static_cast<void>(TestGetInstance<T>())

namespace lclog {
inline Derived& GlobalLogInstance() {
    return Derived::GetInstance();
}

void init(const std::string& config_file="") {
    // 检查glogLogger类是否满足要求
    CHECK_CLASS_TYPE(Derived);
    LC_LOG_SETTING config;  
    config.input_file = "/tmp/log4cplus.properties";
    config.output_file = "testlog";
    config.log_id = "glog1127";
    config.log_level = LogLevel::Info;
    config.max_file_size = 1024;
    config.max_file_count = 10;
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
