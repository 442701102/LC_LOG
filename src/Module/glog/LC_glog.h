#ifndef LC_GLOG_H
#define LC_GLOG_H

#include "LCLoggerBase.hpp"

namespace lclog {
class DLL_PUBLIC  glogLogger : public LCLoggerBase<glogLogger> {
    friend LCLoggerBase<glogLogger>; 
private:
    glogLogger():_start_init(false){};
    bool  _glogConfig(LCLog_cfg_st &config);
    
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

    glogLogger(const glogLogger&) = delete;
    glogLogger& operator=(const glogLogger&) = delete;

    //删除 glogLogger 的移动构造函数
    glogLogger(glogLogger&&) = delete;
    //删除 glogLogger 的移动赋值运算符
    glogLogger& operator=(glogLogger&&) = delete;

protected:  
    void HandleLogOutput(LogLevel level, const std::string& message) ;//< Log the message with the specified level    
    bool isEnable();//< Check if the logger is enabled
    bool Configure(LCLog_cfg_st &config);//< Configure the logger
    ~glogLogger();
};
}






#endif // LC_GLOG_H















