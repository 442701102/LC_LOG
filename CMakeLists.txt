cmake_minimum_required(VERSION 3.15)
project(LC_LOG_PRO CXX)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Weclome to the LC_LOG_PRO !!!!")

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/third-party")
# 通过命令行或 GUI 提供一个选项来选择日志实现

# 检查是否定义了log_build_type变量
if(DEFINED log_build_type)
    # 如果log_build_type已定义，则将其值赋给LOGGER_CRTP
    set(LOGGER_CRTP ${log_build_type})
else()
    # 如果log_build_type未定义，使用默认值"CUSTOM"
    set(LOGGER_CRTP "CUSTOM" CACHE STRING "选择要使用的日志实现")
endif()

# 可用选项的文档
set_property(CACHE LOGGER_CRTP PROPERTY STRINGS "CUSTOM" "SPDLOG" "GLOG" "LOG4CPLUS" "DLT")

# 根据选择的日志器配置
if(LOGGER_CRTP STREQUAL "CUSTOM")
    add_definitions(-DUSE_STD_COUT)
    set(USE_CUSTOM_LOGGER  1)
elseif(LOGGER_CRTP STREQUAL "SPDLOG")
    add_definitions(-DUSE_SPDLOG)
    set(USE_SPDLOG_LOGGER  1)
elseif(LOGGER_CRTP STREQUAL "GLOG")
    add_definitions(-DUSE_GLOG)
    set(USE_GLOG_LOGGER  1)
elseif(LOGGER_CRTP STREQUAL "LOG4CPLUS")
    add_definitions(-DUSE_LOG4CPLUS)
    set(USE_LOG4CPLUS_LOGGER  1)
elseif(LOGGER_CRTP STREQUAL "DLT")
    add_definitions(-DUSE_DLT)
    set(USE_DLT_LOGGER  1)
else()
    message(FATAL_ERROR "选择了无效的日志实现：${LOGGER_CRTP}")
endif()

message(STATUS "Selected logger implementation: ${LOGGER_CRTP}")


# Add a configuration header file to handle CMake's settings for the source code
configure_file (
  "${CMAKE_SOURCE_DIR}/config/config.h.in"
  "${CMAKE_SOURCE_DIR}/include/config.h"  
)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)
add_subdirectory(tools)

