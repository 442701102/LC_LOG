#ifndef LC_LOG4_H
#define LC_LOG4_H


#include "LCLoggerBase.hpp"

namespace lclog {

class log4cplusLogger : public LCLoggerBase<log4cplusLogger> {
    friend LCLoggerBase<log4cplusLogger>;
private:

    log4cplusLogger():_start_init(false){};
    bool  _log4cplusInit(LC_LOG_SETTING &config);
    
    std::atomic<bool> _start_init;
    std::thread _Delay_Thread;
public: 
    ~log4cplusLogger() override;
    log4cplusLogger(const log4cplusLogger&) = delete;
    log4cplusLogger& operator=(const log4cplusLogger&) = delete;

    static log4cplusLogger& GetInstance() {
        static log4cplusLogger instance;
        return instance;
    }



protected:
    void HandleLogOutput(LogLevel level, const std::string& message) override;//< Log the message with the specified level    
    bool isEnable() override;//< Check if the logger is enabled 
    bool Configure(LC_LOG_SETTING &config)override;//< Configure the logger
};
}

#endif // LC_LOG4_H











