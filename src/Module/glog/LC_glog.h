#ifndef LC_GLOG_H
#define LC_GLOG_H

#include "LCLoggerBase.hpp"
#include <thread>
#include <atomic>

namespace lclog {
class Log_glog : public LCLoggerBase<Log_glog> {
    friend LCLoggerBase<Log_glog>; 
private:
    constexpr int mapLogLevel(LogLevel level) {
        switch (level) {
            case Fatal: return 0;
            case Error: return 1;
            case Warn:  return 2;
            case Info:  return 3;
            case Debug: return 4;
            default:    return -1; 
        }
    }
    Log_glog():_start_init(false){};
    bool  _glogInit(LC_LOG_SETTING &config);
    
    std::atomic<bool> _start_init;
    std::thread _Delay_Thread;
    std::mutex _wait_mutex;

public:
    typedef struct logsetting_st {
        std::string info_file;
        std::string log_level;
        int file_size;
        int verbose_level;
        bool log_to_file;
    } logsetting;
    logsetting _log_setting{".", "info", 2, 0, false};

    ~Log_glog() override;
    Log_glog(const Log_glog&) = delete;
    Log_glog& operator=(const Log_glog&) = delete;


    static Log_glog& GetInstance() {
        static Log_glog instance;
        return instance;
    }

    bool Configure(LC_LOG_SETTING &config);//< Configure the logger
    void setWatchCycle(uint32_t Seconds);

protected:  
    void HandleLogOutput(LogLevel level, const std::string& message) override;//< Log the message with the specified level    

};
}






#endif // LC_GLOG_H















