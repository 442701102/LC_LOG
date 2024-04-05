#include "LC_custom.h"
#include <iostream> 
namespace lclog {


customLogLogger::~customLogLogger() {
    // Do nothing
}   
void customLogLogger::HandleLogOutput(LogLevel level, const std::string& message) {
    // 同时输出到控制台
    std::cout << "custom [ " << level << " ] " << message << std::endl;    
}
bool  customLogLogger::Configure(LCLog_cfg_st &config) {
    this->_start_init = true;
    return true;
}     
bool customLogLogger::isEnable() {
    return true;
}
}   