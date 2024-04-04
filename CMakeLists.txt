cmake_minimum_required(VERSION 3.15)
project(LC_LOG_PRO CXX)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Weclome to the LC_LOG_PRO !!!!")

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/third-party")

# Suppose we have an option for choosing the logger implementation
set(LOGGER_CRTP "LOG4CPLUS" CACHE STRING "Select which logging implementation to use")
# Configure based on the selected logger
if(LOGGER_CRTP STREQUAL "STD_COUT")
    add_definitions(-DUSE_STD_COUT)
    set(USE_STDOUT_LOGGER  1)
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
    message(FATAL_ERROR "Invalid logger implementation selected: ${LOGGER_IMPL}")
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

