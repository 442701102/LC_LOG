#include "LC_glog.h"


namespace lclog{

void Log_glog::LogImpl(LogLevel level, const std::string& message) {
    switch (level) {
        case LogLevel::Debug:
            LOG(INFO) << message;
            break;
        case LogLevel::Info:
            LOG(INFO) << message;
            break;
        case LogLevel::Warn:
            LOG(WARNING) << message;
            break;
        case LogLevel::Error:
            LOG(ERROR) << message;
            break;
        case LogLevel::Fatal:
            LOG(FATAL) << message;
            break;
        default:
            LOG(INFO) << message;
            break;
    }
}
bool Log_glog::Configure(const std::string &config) {
    google::InitGoogleLogging("LC_LOG");
    google::SetLogDestination(google::GLOG_INFO, "log/");
    google::SetLogDestination(google::GLOG_WARNING, "log/");
    google::SetLogDestination(google::GLOG_ERROR, "log/");
    google::SetLogDestination(google::GLOG_FATAL, "log/");
    google::SetLogSymlink(google::GLOG_INFO, "log/");
    google::SetLogSymlink(google::GLOG_WARNING, "log/");
    google::SetLogSymlink(google::GLOG_ERROR, "log/");
    google::SetLogSymlink(google::GLOG_FATAL, "log/");
    google::SetStderrLogging(google::GLOG_INFO);
    google::SetStderrLogging(google::GLOG_WARNING);

    return true;
}

}