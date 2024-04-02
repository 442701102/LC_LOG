#ifndef LIB_MAIN_HPP
#define LIB_MAIN_HPP

#include <iostream>
//spd
#include <spdlog/spdlog.h>  
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>


int spdlog_test() {
    // Create a logger with a stdout color sink
    auto console = spdlog::stdout_color_mt("console");
    console->info("Welcome to spdlog!");
    console->error("Some error message with arg: {}", 1);

    // Create basic file logger (not rotated)
    auto my_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
    my_logger->info("Some log message");

    // Create a daily logger - a new file is created every day on 2:30am
    auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30);
    daily_logger->info("Some log message");

    return 0;
}   




int hello() {
    return 0;
}

#endif // LIB_MAIN_HPP