#include "LC_dlt.h"
#include <dlt/dlt.h>
#include <iostream> 

namespace lclog {
class dltLogger::DltImpl {  
public:
    DltImpl() = default;
    ~DltImpl() = default;   
    DltContext dlt_context;
};  
static constexpr DltLogLevelType mapLogLevel(LogLevel level) {
    switch (level) {
        case LogLevel::Fatal: return DLT_LOG_FATAL;
        case LogLevel::Error: return DLT_LOG_ERROR;
        case LogLevel::Warn: return DLT_LOG_WARN;
        case LogLevel::Info: return DLT_LOG_INFO;
        case LogLevel::Debug: return DLT_LOG_DEBUG;
        default: return DLT_LOG_VERBOSE;
    }
}
static constexpr const char * mapDltReturnValue(int ret){
    switch (ret) {
        case DLT_RETURN_FILESZERR:
            return "DLT_RETURN_FILESZERR";
        case DLT_RETURN_LOGGING_DISABLED:
            return "DLT_RETURN_LOGGING_DISABLED";
        case DLT_RETURN_USER_BUFFER_FULL:
            return "DLT_RETURN_USER_BUFFER_FULL";
        case DLT_RETURN_WRONG_PARAMETER:
            return "DLT_RETURN_WRONG_PARAMETER";
        case DLT_RETURN_BUFFER_FULL:
            return "DLT_RETURN_BUFFER_FULL";
        case DLT_RETURN_PIPE_FULL:
            return "DLT_RETURN_PIPE_FULL";
        case DLT_RETURN_PIPE_ERROR:
            return "DLT_RETURN_PIPE_ERROR";
        case DLT_RETURN_ERROR:
            return "DLT_RETURN_ERROR";
        case DLT_RETURN_OK:
            return "DLT_RETURN_OK";
        case DLT_RETURN_TRUE:
            return "DLT_RETURN_TRUE";
        default:
            return "Unknown";
    }
}

void dltLogger::HandleLogOutput(LogLevel level, const std::string& message) {
    // 同时输出到控制台
    std::cout << "dlt [ " << level << " ] " << message << std::endl;    
    // 映射日志级别
    DltLogLevelType dlt_level = mapLogLevel(level);

    // 获取日志上下文的引用
    DltContext& dlt_context = _pImpl->dlt_context;

    int ret = DLT_RETURN_OK;

    // 使用DLT_IS_LOG_LEVEL_ENABLED宏先检查当前上下文是否启用了该日志级别
    // 这可以避免在日志级别不足以记录该消息时执行不必要的操作
    if (DLT_IS_LOG_LEVEL_ENABLED(dlt_context, dlt_level)) {
        // 直接使用DLT_LOG宏输出日志
        DltContextData logData;
        if (ret = dlt_user_log_write_start(&dlt_context, &logData, dlt_level) == DLT_RETURN_TRUE) {
            // 添加消息内容到日志中
            ret = dlt_user_log_write_string(&logData, message.c_str());
            if (ret != DLT_RETURN_OK) {
                std::cout << "dlt_user_log_write_string failed,it is " << mapDltReturnValue(ret) << std::endl;
            }  
            ret = dlt_user_log_write_finish(&logData);
            if (ret != DLT_RETURN_OK) {
                std::cout << "dlt_user_log_write_finish failed,it is " << mapDltReturnValue(ret) << std::endl;
            } 
            else {
                std::cout << "dlt_user_log_write_finish success" << std::endl;
            }   
        }
        else{
            std::cout << "dlt_user_log_write_start failed,it is " << mapDltReturnValue(ret) << std::endl;
        }
    }
    else {
        std::cout << "dltLogger is not enabled" << std::endl;
    }
}
bool dltLogger::Configure(LC_LOG_SETTING &config) {
    if (this->_start_init == true) {
        std::cout << "dltLogger has been initialized" << std::endl;
        return true;
    }
    _pImpl == nullptr ? _pImpl = new DltImpl() : _pImpl;    

    DltContext& dlt_context = _pImpl->dlt_context;

    auto ret = dlt_init();
    if(ret != DLT_RETURN_OK) {
        std::cout << "dlt_init failed,it is " << mapDltReturnValue(ret) << std::endl;
        return false;
    }
    ret = dlt_register_app(config.log_id.data(), "LC LOG Application");
    if(ret != DLT_RETURN_OK) {
        std::cout << "dlt_register_app failed,it is " << mapDltReturnValue(ret) << std::endl;
        return false;
    }
    else {
        std::cout << "dlt_register_app success" << std::endl;
    }

    std::string thread_id = std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    // 注册DLT上下文，与DLT_REGISTER_APP一样，这通常在程序启动时执行一次
    DltLogLevelType dlt_level = mapLogLevel(config.log_level); 
    ret = dlt_register_context_ll_ts(&dlt_context, thread_id.c_str(), "LC-LOG-filesystem logger context", dlt_level, DLT_TRACE_STATUS_ON);   
    if(ret != DLT_RETURN_OK) {
        std::cout << "dlt_register_context_ll_ts failed ,it is " << mapDltReturnValue(ret) << std::endl;   
        return false;
    }
    //Set daemon log limit and trace status. Anything above this log level will not be printed.

    DLT_SET_APPLICATION_LL_TS_LIMIT(DLT_LOG_DEBUG, DLT_TRACE_STATUS_ON);

    DLT_LOG(dlt_context, DLT_LOG_INFO, DLT_STRING("LC-LOG-filesystem logger context is registered"));   
   
 //  dlt_with_session_id(0);
   
    ret = dlt_set_log_mode(DltUserLogMode::DLT_USER_MODE_BOTH);
    if(ret != DLT_RETURN_OK) {
        std::cout << "dlt_set_log_mode failed,it is " << mapDltReturnValue(ret) << std::endl;
    }

    this->_start_init = true;
    return true;

}
dltLogger::~dltLogger() {
   // 检查_pImpl是否非空，避免解引用空指针
    if (_pImpl) {
        // 无条件注销DLT上下文和应用程序
        DLT_UNREGISTER_CONTEXT(_pImpl->dlt_context);
        dlt_unregister_app_flush_buffered_logs();

        // 删除_pImpl指针
        delete _pImpl;
    }
    // 最后输出日志器关闭的消息，这不依赖于任何_dlt资源
    std::cout << "dltLogger is shutting down" << std::endl;
}
bool dltLogger::isEnable() {
    return this->_start_init;   
}
// Path: home/cc/code/gittee/LC_LOG/src/Module/log4cplus/LC_log4cplus.h 
} // namespace lclog
