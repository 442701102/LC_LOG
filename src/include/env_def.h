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
        int max_file_count;  
        bool log_to_file;
        bool daily_file;
    };  
}
// 在某个公共头文件中定义
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif


#endif // ENV_DEF_H