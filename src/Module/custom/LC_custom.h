#ifndef CUSTOM_LOGGER_H 
#define CUSTOM_LOGGER_H 

#include "LCLoggerBase.hpp" 


namespace lclog {   
class customLogLogger : public LCLoggerBase<customLogLogger> {
    friend LCLoggerBase<customLogLogger>;
private:
    std::atomic<bool> _start_init;
    customLogLogger():_start_init(false){}; 
public:

    ~customLogLogger();

    customLogLogger(const customLogLogger&) = delete;   
    customLogLogger& operator=(const customLogLogger&) = delete;
    customLogLogger(customLogLogger&&) = delete;
    customLogLogger& operator=(customLogLogger&&) = delete;

protected:  
    void HandleLogOutput(LogLevel level, const std::string& message) ;//< Log the message with the specified level    
    bool isEnable() ;//< Check if the logger is enabled
    bool Configure(LCLog_cfg_st &config);//< Configure the logger
};
}






#endif // LC_GLOG_H















