#include "LC_spdlog.h"

#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>   
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>    
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/stdout_sinks.h>


namespace lclog {
class spdlogLogger::spdImpl {
public:
    spdImpl() = default;
    ~spdImpl() = default;   
    std::shared_ptr<spdlog::logger> logger;
};

static constexpr spdlog::level::level_enum mapLogLevel(LogLevel level) {
    switch (level) {
        case LogLevel::Fatal: return spdlog::level::level_enum::critical;
        case LogLevel::Error: return spdlog::level::level_enum::err;
        case LogLevel::Warn: return spdlog::level::level_enum::warn;
        case LogLevel::Info: return spdlog::level::level_enum::info;
        case LogLevel::Debug: return spdlog::level::level_enum::debug;
        default: return spdlog::level::level_enum::trace;
    }
}
void spdlogLogger::HandleLogOutput(LogLevel level, const std::string& message) {
    _pImpl->logger->log(static_cast<spdlog::level::level_enum>(mapLogLevel(level)), message);//bug
}
bool spdlogLogger::Configure(LC_LOG_SETTING &config) {
    if (this->_start_init == true) {
        std::cout << "spdlogLogger has been initialized" << std::endl;
        return true;
    }

    try {
        // 创建一个sinks列表
        std::vector<spdlog::sink_ptr> sinks;

        // 添加一个控制台sink，默认总是输出到控制台
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sinks.push_back(console_sink);

        // 根据配置决定是否添加文件sink
        if (config.log_to_file) {
            spdlog::sink_ptr file_sink;
            if (config.daily_file) {
                //daily_file_sink_mt是一个每日创建新日志文件的sink。它按天分割日志文件，每当新的一天开始时，就会创建一个新的日志文件，并且之前的日志文件将保持不变。
                file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(config.output_file, 0, 0);
            } else {
                //rotating_file_sink_mt是一个基于文件大小进行日志文件轮转的sink。当当前日志文件达到指定的大小限制时，它会自动创建一个新的日志文件继续记录，同时保留旧的日志文件直到达到预设的文件数量限制。超出数量限制的最旧文件将被删除。
                file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(config.output_file, config.max_file_size, config.max_file_count);
            }
            sinks.push_back(file_sink);
        }

        _pImpl = new spdlogLogger::spdImpl(); // 创建_pImpl指针   

        // 创建一个具有多个sink的logger
        _pImpl->logger = std::make_shared<spdlog::logger>(config.log_id, begin(sinks), end(sinks));
        
        // 设置日志级别和格式
        _pImpl->logger->set_level(static_cast<spdlog::level::level_enum>(mapLogLevel(config.log_level)));
        _pImpl->logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");

        // 注册_pImpl->logger以便能通过spdlog::get访问
        spdlog::register_logger(_pImpl->logger);

    } catch (const spdlog::spdlog_ex& e) {
        std::cerr << "Exception caught during spdlog configuration: " << e.what() << std::endl;
        return false;
    }

    this->_start_init = true;
    return true;
}
lclog::spdlogLogger::~spdlogLogger() {
    // 正确的析构逻辑
    std::cout << "spdlogLogger is shutting down" << std::endl;
    if (_pImpl && _pImpl->logger) {
        spdlog::drop(_pImpl->logger->name());
    }
    delete _pImpl;  
    spdlog::shutdown(); 
}
bool spdlogLogger::isEnable() {
    return this->_start_init;
}
}   
