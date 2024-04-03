#ifndef LC_GLOG_H
#define LC_GLOG_H

#include "LCLoggerBase.hpp"
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <glog/stl_logging.h>
#include <glog/vlog_is_on.h>
#include <glog/log_severity.h>

namespace lclog {
class Log_glog : public LCLoggerBase<Log_glog> {
private:
    Log_glog() = default;
public:
    ~Log_glog() = default;
    Log_glog(const Log_glog&) = delete;
    Log_glog& operator=(const Log_glog&) = delete;


    static Log_glog& GetInstance() {
        static Log_glog instance;
        return instance;
    }

    bool Configure(const std::string &config = "");//< Configure the logger
    void setWatchCycle(uint32_t Seconds);

protected:  
    void LogImpl(LogLevel level, const std::string& message) override;//< Log the message with the specified level    

};
}






#endif // LC_GLOG_H















