cmake_minimum_required(VERSION 3.15)
project(LC_LOG_PRO CXX)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Weclome to the LC_LOG_PRO !!!!")

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/third-party")

# Suppose we have an option for choosing the logger implementation
set(LOGGER_IMPL "GLOG" CACHE STRING "Select which logging implementation to use")
# Configure based on the selected logger
if(LOGGER_IMPL STREQUAL "STD_COUT")
    add_definitions(-DUSE_STD_COUT)
elseif(LOGGER_IMPL STREQUAL "SPDLOG")
    add_definitions(-DUSE_SPDLOG)
elseif(LOGGER_IMPL STREQUAL "GLOG")
    add_definitions(-DUSE_GLOG)
elseif(LOGGER_IMPL STREQUAL "LOG4CPLUS")
    add_definitions(-DUSE_LOG4CPLUS)
elseif(LOGGER_IMPL STREQUAL "DLT")
    add_definitions(-DUSE_DLT)
else()
    message(FATAL_ERROR "Invalid logger implementation selected: ${LOGGER_IMPL}")
endif()

message(STATUS "Selected logger implementation: ${LOGGER_IMPL}")
# Add a configuration header file to handle CMake's settings for the source code
configure_file (
  "${CMAKE_SOURCE_DIR}/config/config.h.in"
  "${CMAKE_SOURCE_DIR}/include/config.h"  
)



add_subdirectory(src)

