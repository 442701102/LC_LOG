#ifndef ENV_DEF_H
#define ENV_DEF_H   
#include <string>

const constexpr inline int  MAX_LOG_SIZE_MB = 2048;   

namespace lclog {
enum LogLevel {
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

struct LC_LOG_SETTING{
    std::string input_file; 
    std::string output_file;
    std::string log_id;
    LogLevel log_level;
    int max_file_size;
    int max_log_count;  
    bool log_to_file;
};  
}


#endif // ENV_DEF_H