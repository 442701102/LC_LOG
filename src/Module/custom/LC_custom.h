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
    virtual ~customLogLogger()override;
    customLogLogger(const customLogLogger&) = delete;   
    customLogLogger& operator=(const customLogLogger&) = delete;

    static customLogLogger& GetInstance() {
        static customLogLogger instance;
        return instance;
    }

protected:  
    void HandleLogOutput(LogLevel level, const std::string& message) override;//< Log the message with the specified level    
    bool isEnable() override;//< Check if the logger is enabled
    bool Configure(LC_LOG_SETTING &config)override;//< Configure the logger
};
}






#endif // LC_GLOG_H















