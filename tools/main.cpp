#include <iostream>
#include <string>
#include <mutex>
#include <atomic>

#include <thread>
#include "lclog.hpp"
int main() {
    std::cout << "Hello, World!" << std::endl;
    lclog::init();
    lclog::printf(lclog::LogLevel::Info, "Hello, World!");
    return 0;
}