#ifndef LCLogger_H
#define LCLogger_H

#include <iostream>
#include <sstream>
#include <string>
#include <format> 
#include <cstdarg> 
#include <concepts>

enum LCLogLevel {
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

template<typename... Args>
concept Formattable = (... && requires(Args arg) { std::format("{}", arg); });

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
        // 可以设置全局配置，比如日志级别，输出目的地等
        return Derived::GetInstance()->(config);
    }

    // 格式化日志接口
    template<Formattable... Args>
    static void Log(LCLogLevel level, const std::string& format, Args&&... args) {
        std::string message = std::format(format, std::forward<Args>(args)...);
        Derived::GetInstance()->LogImpl(level, message);
    }

    // C风格的可变参数日志函数
    static void printf(LCLogLevel level, const char* format, ...) {
        va_list args;
        va_start(args, format);
        std::string formatted = std::vformat(format, std::make_format_args(args));
        va_end(args);
        Derived::GetInstance()->LogImpl(level, formatted);
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
    
    // 确保派生类提供GetInstance静态方法
    static_assert(std::is_member_function_pointer<decltype(&Derived::GetInstance)>::value,
                  "Derived class must implement static GetInstance method.");
};

// 日志宏定义
#define LC_LOG(level) LCLogger::Log(level)

#endif // LCLogger_H
