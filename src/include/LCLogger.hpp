#ifndef LCLogger_H
#define LCLogger_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdarg> 
#include <concepts>
//如果gcc大于13,且支持C++20 则使用std::format
#if defined(__GNUC__) && __GNUC__ >= 13 && defined(__cpp_lib_format) 
    static_assert(__cplusplus >= 202002L, "C++20 with library support is required.");
    #include <format>
    #define FMT_USE_STD_FORMAT
#else
#include <fmt/format.h>
#define FMT_USE_FMT
#endif

enum LCLogLevel {
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};


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
// CRTP基类
template<typename Derived>
class LCLogger {
public:
    LCLogger() = default; // 默认构造函数
    ~LCLogger() = default; // 默认析构函数

    // 禁止拷贝构造和赋值
    LCLogger(const LCLogger&) = delete;
    LCLogger& operator=(const LCLogger&) = delete;

    // 静态初始化方法
    static bool init(const std::string &config = "") { 
            // 确保派生类提供GetInstance静态方法
        static_assert(std::is_member_function_pointer<decltype(&Derived::GetInstance)>::value,
                  "Derived class must implement static GetInstance method.");  
        // 可以设置全局配置，比如日志级别，输出目的地等
        return Derived::GetInstance()->Configure(config);
    }

    // 格式化日志接口
    template<Formattable... Args>
    static void Log(LCLogLevel level, const std::string& format, Args&&... args) {
#ifdef FMT_USE_STD_FORMAT
        std::string message = std::format(format, std::forward<Args>(args)...);
#else
        std::string message = fmt::format(format, std::forward<Args>(args)...);
#endif
        Derived::GetInstance()->LogImpl(level, message);
    }
    static std::string LogInternal(const char* format, va_list args) {
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

    static void printf(LCLogLevel level, const char* format, ...) {
        va_list args;
        va_start(args, format);
        std::string formattedMessage = LogInternal(format, args);
        va_end(args);
        
        Derived::GetInstance()->LogImpl(level, formattedMessage);
    }

    // 用于收集日志消息的临时对象
    class LogStream {
    public:
        LogStream(LCLogLevel level) : level_(level) {}
        
        ~LogStream() {
            Derived::GetInstance()->LogImpl(level_, stream_.str());
        }

        template<typename T>
        LogStream& operator<<(const T& message) {
            stream_ << message;
            return *this;
        }

    private:
        LCLogLevel level_;
        std::ostringstream stream_;
    };

    // 提供一个接口来开始流式日志记录
    static LogStream Log(LCLogLevel level) {
        return LogStream(level);
    }

protected:
    virtual void LogImpl(LCLogLevel level, const std::string& message) = 0;
    

};

// 日志宏定义
#define LC_LOG(level) LCLogger::Log(level)

#endif // LCLogger_H
