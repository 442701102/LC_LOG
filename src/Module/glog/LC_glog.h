#ifndef LC_GLOG_H
#define LC_GLOG_H

#include "LCLoggerBase.hpp"

namespace lclog {
class glogLogger : public LCLoggerBase<glogLogger> {
    friend LCLoggerBase<glogLogger>; 
private:
    glogLogger():_start_init(false){};
    bool  _glogConfig(LC_LOG_SETTING &config);
    
    std::atomic<bool> _start_init;
    std::thread _Delay_Thread;

public:
    typedef struct logsetting_st {
        std::string info_file;
        std::string log_level;
        int file_size;
        int verbose_level;
        bool log_to_file;
    } logsetting;
    logsetting _log_setting{".", "info", 2, 0, false};

    ~glogLogger() override;
    glogLogger(const glogLogger&) = delete;
    glogLogger& operator=(const glogLogger&) = delete;


    static glogLogger& GetInstance() {
        static glogLogger instance;
        return instance;
    }

protected:  
    void HandleLogOutput(LogLevel level, const std::string& message) override;//< Log the message with the specified level    
    bool isEnable() override;//< Check if the logger is enabled
    bool Configure(LC_LOG_SETTING &config)override;//< Configure the logger
};
}






#endif // LC_GLOG_H















