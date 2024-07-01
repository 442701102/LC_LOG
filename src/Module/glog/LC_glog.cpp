#include "LC_glog.h"

#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <glog/stl_logging.h>
#include <glog/vlog_is_on.h>
#include <glog/log_severity.h>
namespace lclog{
    static constexpr int mapLogLevel(LogLevel level) {
        switch (level) {
            case Fatal: return 0;
            case Error: return 1;
            case Warn:  return 2;
            case Info:  return 3;
            case Debug: return 4;
            default:    return -1; 
        }
    }

glogLogger::~glogLogger(){
    std::cout << "glogLogger is shutting down" << std::endl;
    google::ShutdownGoogleLogging();    //Close the glog library and free the memory of the glog library
}
void glogLogger::HandleLogOutput(LogLevel level, const std::string& message) {
       VLOG(mapLogLevel(level)) << message; 
}
bool glogLogger::Configure(LCLog_cfg_st &config) {
    if(this->_start_init == true){
      std::cout  <<  "The log system has been initialized" <<std::endl;
      return false;
    }
    //set glog config 
    google::InitGoogleLogging(config.log_id.data());    
    _glogConfig(config);
    try {
    //set thread 
        this->_Delay_Thread = std::thread([this]() {
    //Delay 10s to set this property to avoid resource strain
        std::this_thread::sleep_for(std::chrono::seconds(10));
    //  [Thread-safe]指定级别以上的所有日志消息都立即写入到日志文件中
        google::FlushLogFiles(google::GLOG_INFO);//Vlog is all info level.
        });
        this->_Delay_Thread.detach();
    } catch (const std::system_error& e) {
        std::cerr << "Exception caught during thread creation: " << e.what() << std::endl;
        return false;
    }
    std::cout<<  "OutputLogFile name :"<<config.output_file<<std::endl;
    this->_start_init = true;
    return true;
}
bool glogLogger::_glogConfig(LCLog_cfg_st &config) {
    FLAGS_v = mapLogLevel(config.log_level);
    FLAGS_logtostderr = (config.log_to_file) ? 0:1; //1不输出到文件
    FLAGS_alsologtostderr = true;
    FLAGS_colorlogtostderr = true;  

    FLAGS_max_log_size = static_cast<google::uint32>(config.max_file_size) > MAX_LOG_SIZE_MB ? MAX_LOG_SIZE_MB : this->_log_setting.file_size;//最大日志大小（MB)
    FLAGS_log_prefix = true;
    FLAGS_stop_logging_if_full_disk = true;
    FLAGS_logbufsecs = 0; //实时输出日志

//  [Thread - safe]设置指定级别的日志输出的日志文件，如果basefilename为""则表示该级别日志不输出。
    google::SetLogDestination(google::INFO,config.output_file.data());
//  [Thread - safe]设置置顶级别的日志文件的软连接，symlikbasename为空表示不设置软连接。如果不调用该函数，系统默认连接名称是程序名。   
    google::SetLogSymlink(google::GLOG_INFO,"");
//  [Thread-safe]确定除了输出到日志文件同时还输出到STDERR的日志最小级别
    google::SetStderrLogging(google::GLOG_INFO);
//  [Thread-safe]为所有日志文件添加文件扩展名，特别用于SetLogDestination()设置的日志文件
    google::SetLogFilenameExtension(".log");  
/*Core Dump 配置*/
    // 信号处理函数，处理的主要信号有SIGSEGV / SIGILL / SIGFPE / SIGBRT / SIGBUS/SIGTERM。
//    google::InstallFailureSignalHandler();
    //重定向到log文件中,设置系统崩溃时的输出函数，data数据不一定是以'\0'结尾。
//    google::InstallFailureWriter(&SignalHandle);
    return true;
}
bool glogLogger::isEnable() {
    return this->_start_init;
}

}