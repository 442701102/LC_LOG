#include "LC_glog.h"




void Log_glog::LogImpl(LCLogLevel level, const std::string& message) {
    switch (level) {
        case LCLogLevel::Debug:
            LOG(INFO) << message;
            break;
        case LCLogLevel::Info:
            LOG(INFO) << message;
            break;
        case LCLogLevel::Warn:
            LOG(WARNING) << message;
            break;
        case LCLogLevel::Error:
            LOG(ERROR) << message;
            break;
        case LCLogLevel::Fatal:
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

