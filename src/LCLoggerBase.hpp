#ifndef LCLoggerBase_H
#define LCLoggerBase_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdarg> 
#include <concepts>
#include "env_def.h"

//如果gcc大于13,且支持C++20 则使用std::format
#if defined(__GNUC__) && __GNUC__ >= 13 && defined(__cpp_lib_format) 
    static_assert(__cplusplus >= 202002L, "C++20 with library support is required.");
    #include <format>
    #define FMT_USE_STD_FORMAT
#else
#include <fmt/format.h>
#define FMT_USE_FMT
#endif


#ifdef FMT_USE_STD_FORMAT
    template<typename Arg>
    concept FormattableSingle = requires(Arg arg) {
        { std::format("{}", arg) } -> std::convertible_to<std::string>;
    };
#else
    template<typename Arg>
    concept FormattableSingle = requires(Arg arg) {
        { fmt::format("{}", arg) } -> std::convertible_to<std::string>;
    };
#endif

template<typename... Args>
concept Formattable = (... && FormattableSingle<Args>);

namespace lclog {
// CRTP基类
template<typename Derived>
class LCLoggerBase  {
public:
    LCLoggerBase () = default; //< 默认构造函数
    virtual ~LCLoggerBase() = default; //< 虚析构函数

    // 禁止拷贝构造和赋值
    LCLoggerBase (const LCLoggerBase &) = delete;
    LCLoggerBase & operator=(const LCLoggerBase &) = delete;

    // 静态初始化方法
    bool init(LC_LOG_SETTING &config, LogLevel level = LogLevel::Info) {
        // 确保派生类提供GetInstance静态方法
        // static_assert(std::is_member_function_pointer<decltype(&Derived::GetInstance)>::value,
        //           "Derived class must implement static GetInstance method.");
        // 设置当前日志级别
        m_currentLevel = level;  
        return Derived::GetInstance().Configure(config);
    }

    // 格式化日志接口
    template<typename FormatStr, typename... Args>
    void Log(LogLevel level, FormatStr&& format, Args&&... args) {
        if(level < m_currentLevel) return;
        #ifdef FMT_USE_STD_FORMAT
            std::string message = std::format(std::forward<FormatStr>(format), std::forward<Args>(args)...);
        #else
            std::string message = fmt::format(std::forward<FormatStr>(format), std::forward<Args>(args)...);
        #endif
            Derived::GetInstance().LogImpl(level, message);
    }
    std::string LogInternal(const char* format, va_list args) {
        std::vector<char> buffer(4096);
        va_list args_copy;
        va_copy(args_copy, args);
        int needed = vsnprintf(buffer.data(), buffer.size(), format, args_copy);
        va_end(args_copy);

        if (needed >= static_cast<int>(buffer.size())) {
            buffer.resize(needed + 1);
            va_copy(args_copy, args);
            vsnprintf(buffer.data(), buffer.size(), format, args_copy);
            va_end(args_copy);
        }
        return std::string(buffer.data());
    }

    void printf(LogLevel level, const char* format, ...) {
        if(level < m_currentLevel) return;
        va_list args;
        va_start(args, format);
        std::string formattedMessage = LogInternal(format, args);
        va_end(args);
        
        // 确保这里调用的是移动构造函数
        Derived::GetInstance().LogImpl(level, formattedMessage);
    }

    // 用于收集日志消息的临时对象
    class LogStream {
    public:
        LogStream(LogLevel level) : level_(level) {}
        
        ~LogStream() {
            Derived::GetInstance().LogImpl(level_, stream_.str());
        }

        template<typename T>
        LogStream& operator<<(const T& message) {
            stream_ << message;
            return *this;
        }

    private:
        LogLevel level_;
        std::ostringstream stream_;
    };

    // 提供一个接口来开始流式日志记录
    LogStream Log(LogLevel level) {
        return LogStream(level);
    }

protected:
    virtual void LogImpl(LogLevel level, const std::string& message) = 0;
    //current log level
    LogLevel m_currentLevel = LogLevel::Info;

    

};
}
// 日志宏定义
#define LC_LOG(level) LCLoggerBase ::Log(level)

#endif // LCLoggerBase_H
