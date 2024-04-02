#ifndef LIB_MAIN_HPP
#define LIB_MAIN_HPP

#include <iostream>
#include <log4cplus/logger.h>


int log4cplus_logger() {
     log4cplus::Logger c_logger;
    // c_logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
    // log4cplus::BasicConfigurator config;
    // config.configure();
    // LOG4CPLUS_INFO(c_logger, LOG4CPLUS_TEXT("Hello, World!"));
    return 0;
}


int hello() {
    return 0;
}

#endif // LIB_MAIN_HPP