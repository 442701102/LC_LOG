#include <iostream>
#include <string>
#include <mutex>
#include <atomic>

#include <thread>
#include "global_logger.hpp"
int main() {
    std::cout << "Hello, World!" << std::endl;
    lclog::init();
    // 1. 测试基本的流式日志输出
    std::cout << "Testing basic stream logging:" << std::endl;
    lclog::Log(lclog::LogLevel::Info) << "1. This is a basic info message";

    // 2. 测试流式日志记录，包含多种数据类型
    std::cout << "Testing stream logging with multiple data types:" << std::endl;
    lclog::Log(lclog::LogLevel::Info) << "2. Values: " << 42 << ", " << 3.14 << ", " << "string literal";

    // 3. 测试格式化字符串日志记录
    std::cout << "Testing formatted string logging with placeholders:" << std::endl;
    lclog::LogFormat(lclog::LogLevel::Warn, "3. Formatted message with int: {}, double: {}, string: {}", 44, 52.27, "string literal");

    // 4. 测试printf风格日志记录
    std::cout << "Testing printf style logging with various format specifiers:" << std::endl;
    lclog::printf(lclog::LogLevel::Error, "4. Printf style message with int: %d, double: %.2f, string: %s", 99, 548431.66, "string literal");

    // 5. 测试高级格式化字符串日志记录
    std::cout << "Testing advanced formatted string logging:" << std::endl;
    lclog::LogFormat(lclog::LogLevel::Info, "5. Advanced formatting with width and precision: int: {:>10}, double: {:.5f}", 55, 3.14159);

    // 6. 测试字符串拼接日志记录
    std::cout << "Testing string concatenation logging:" << std::endl;  
    lclog::LogConcat(lclog::LogLevel::Error, "6. Concatenated message with multiple strings: ", "Hello, ", "World!");  
    return 0;
}