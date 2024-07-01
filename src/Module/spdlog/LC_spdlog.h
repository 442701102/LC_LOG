#ifndef LC_SPDLOG_H 
#define LC_SPDLOG_H

#include "LCLoggerBase.hpp"


namespace lclog {
class DLL_PUBLIC  spdlogLogger : public LCLoggerBase<spdlogLogger> {
    // 类成员和方法
    friend LCLoggerBase<spdlogLogger>;
private:
    spdlogLogger():_start_init(false){};

    std::atomic<bool> _start_init;
    class spdImpl; // 前向声明实现类
    spdlogLogger::spdImpl* _pImpl; // Pimpl指针
public:

    spdlogLogger(const spdlogLogger&) = delete;
    spdlogLogger& operator=(const spdlogLogger&) = delete;
    spdlogLogger(spdlogLogger&&) = delete;
    spdlogLogger& operator=(spdlogLogger&&) = delete;


protected:
    ~spdlogLogger(); // 仅声明析构函数
    void HandleLogOutput(LogLevel level, const std::string& message) ;//< Log the message with the specified level    
    bool isEnable() ;//< Check if the logger is enabled 
    bool Configure(LCLog_cfg_st &config);//< Configure the logger
};
}


















#endif // LC_SPDLOG_H










