#include "LC_log4cplus.h"
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/log4cplus.h>
#include <numbers>
namespace lclog {
static constexpr log4cplus::LogLevel mapLogLevel(LogLevel level) {
    switch (level) {
        case LogLevel::Fatal: return ::log4cplus::FATAL_LOG_LEVEL;
        case LogLevel::Error: return ::log4cplus::ERROR_LOG_LEVEL;
        case LogLevel::Warn: return ::log4cplus::WARN_LOG_LEVEL;
        case LogLevel::Info: return ::log4cplus::INFO_LOG_LEVEL;
        case LogLevel::Debug: return ::log4cplus::DEBUG_LOG_LEVEL;
        default: return ::log4cplus::TRACE_LOG_LEVEL;
    }
}
bool log4cplusLogger::Configure(LC_LOG_SETTING &config) {
    if (this->_start_init == true) {
        std::cout << "log4cplusLogger has been initialized" << std::endl;
        return true;
    }
    try{
        ::log4cplus::initialize();
        //log4cplus.properties
        ::log4cplus::tstring tinput_file = LOG4CPLUS_C_STR_TO_TSTRING(config.input_file.c_str());
        ::log4cplus::PropertyConfigurator::doConfigure(tinput_file);
    }
    catch (std::runtime_error& e) {
        std::cout << "Configure log4cplus failed: "  << std::endl;
        return false;
    }
    catch (std::exception& e) {
        std::cout << "Configure log4cplus failed: " << std::endl;
        return false;
    }
    this->_start_init = true;
    return true;
}   
//log4cplusLogger
log4cplusLogger::~log4cplusLogger() {
    std::cout << "log4cplusLogger is shutting down" << std::endl;
    ::log4cplus::Logger::shutdown();
}

void log4cplusLogger::HandleLogOutput(LogLevel level, const std::string& message) {
    //::log4cplus::Logger::getRoot().log(mapLogLevel(level), static_cast<::log4cplus::tstring>(message));
    ::log4cplus::Logger::getRoot().forcedLog(mapLogLevel(level), LOG4CPLUS_STRING_TO_TSTRING(message));

}

bool log4cplusLogger::isEnable() {
    return this->_start_init;
}

}