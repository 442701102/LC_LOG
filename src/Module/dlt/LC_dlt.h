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

    virtual ~dltLogger()override; // 仅声明析构函数
    dltLogger(const dltLogger&) = delete;
    dltLogger& operator=(const dltLogger&) = delete;

    static dltLogger& GetInstance() {
        static dltLogger instance;
        return instance;
    }

protected:

    void HandleLogOutput(LogLevel level, const std::string& message) override;//< Log the message with the specified level    
    bool isEnable() override;//< Check if the logger is enabled 
    bool Configure(LC_LOG_SETTING &config)override;//< Configure the logger
};

}

#endif // LC_DLT_H






