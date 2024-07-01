#ifndef LC_DLT_H
#define LC_DLT_H    

#include "LCLoggerBase.hpp" 


namespace lclog {

class DLL_PUBLIC  dltLogger : public LCLoggerBase<dltLogger> {
    friend LCLoggerBase<dltLogger>;
private:

    dltLogger():_start_init(false){};
    std::atomic<bool> _start_init;

    class DltImpl; // 前向声明实现类
    dltLogger::DltImpl* _pImpl; // Pimpl指针

public: 

    ~dltLogger(); // 仅声明析构函数
    dltLogger(const dltLogger&) = delete;
    dltLogger& operator=(const dltLogger&) = delete;
    dltLogger(dltLogger&&) = delete;
    dltLogger& operator=(dltLogger&&) = delete;


protected:

    void HandleLogOutput(LogLevel level, const std::string& message) ;//< Log the message with the specified level    
    bool isEnable() ;//< Check if the logger is enabled 
    bool Configure(LCLog_cfg_st &config);//< Configure the logger
};

}

#endif // LC_DLT_H






