#ifndef LC_GLOG_H
#define LC_GLOG_H

#include "LCLogger.hpp"
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <glog/stl_logging.h>
#include <glog/vlog_is_on.h>
#include <glog/log_severity.h>


class Log_glog : public LCLogger<Log_glog> {
private:
    Log_glog() = default;
public:
    Log_glog(const Log_glog&) = delete;
    Log_glog& operator=(const Log_glog&) = delete;
    virtual ~Log_glog() = default;

    static Log_glog& GetInstance() {
        static Log_glog instance;
        return instance;
    }

    bool Configure(const std::string &config = "");
    void setWatchCycle(uint32_t Seconds);

protected:  
    void LogImpl(LCLogLevel level, const std::string& message) override;

};










#endif // LC_GLOG_H















