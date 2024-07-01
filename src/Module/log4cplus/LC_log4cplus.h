#ifndef LC_LOG4_H
#define LC_LOG4_H


#include "LCLoggerBase.hpp"

namespace lclog {

class DLL_PUBLIC  log4cplusLogger : public LCLoggerBase<log4cplusLogger> {
    friend LCLoggerBase<log4cplusLogger>;
private:
    log4cplusLogger():_start_init(false){};
    std::atomic<bool> _start_init;
public: 

    log4cplusLogger(const log4cplusLogger&) = delete;
    log4cplusLogger& operator=(const log4cplusLogger&) = delete;
    log4cplusLogger(log4cplusLogger&&) = delete;
    log4cplusLogger& operator=(log4cplusLogger&&) = delete;


protected:
    ~log4cplusLogger() ;
    void HandleLogOutput(LogLevel level, const std::string& message) ;//< Log the message with the specified level    
    bool isEnable() ;//< Check if the logger is enabled 
    bool Configure(LCLog_cfg_st &config);//< Configure the logger
};
}

#endif // LC_LOG4_H











