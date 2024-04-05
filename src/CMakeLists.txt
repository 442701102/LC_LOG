cmake_minimum_required(VERSION 3.5)


project(LCLOG LANGUAGES CXX)

#set(PROJECT_NAME LCLOG)

file(GLOB_RECURSE HEADER_FILES *.h)
# Include source files based on the selected logger implementation
if(LOGGER_CRTP STREQUAL "STD_COUT")
    file(GLOB_RECURSE STD_SRC_FILES Module/std/*.cc Module/std/*.cpp Module/std/*.hpp)
    set(LOGGER_SRC_FILES ${STD_SRC_FILES})
    message(STATUS "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@STD_COUT@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
elseif(LOGGER_CRTP STREQUAL "SPDLOG")
    file(GLOB_RECURSE SPDLOG_SRC_FILES Module/spdlog/*.cc Module/spdlog/*.cpp Module/spdlog/*.hpp)
    set(LOGGER_SRC_FILES ${SPDLOG_SRC_FILES})
    message(STATUS "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@SPDLOG@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
elseif(LOGGER_CRTP STREQUAL "GLOG")
    file(GLOB_RECURSE GLOG_SRC_FILES Module/glog/*.cc Module/glog/*.cpp Module/glog/*.hpp)
    set(LOGGER_SRC_FILES ${GLOG_SRC_FILES})
    message(STATUS "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@GLOG@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
elseif(LOGGER_CRTP STREQUAL "LOG4CPLUS")
    file(GLOB_RECURSE LOG4CPLUS_SRC_FILES Module/log4cplus/*.cc Module/log4cplus/*.cpp Module/log4cplus/*.hpp)
    set(LOGGER_SRC_FILES ${LOG4CPLUS_SRC_FILES})
    message(STATUS "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@LOG4CPLUS@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
elseif(LOGGER_CRTP STREQUAL "DLT")
    file(GLOB_RECURSE DLT_SRC_FILES Module/dlt/*.cc Module/dlt/*.cpp Module/dlt/*.hpp)
    set(LOGGER_SRC_FILES ${DLT_SRC_FILES})
    message(STATUS "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@DLT@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
else()
    message(FATAL_ERROR "Invalid logger implementation selected")
endif()

# Add the source files to the target    

find_package(fmt REQUIRED)
find_package(glog REQUIRED)
find_package(log4cplus REQUIRED)
find_package(spdlog REQUIRED)
find_package(dlt-daemon REQUIRED)

message(STATUS "LOGGER_SRC_FILES: ${LOGGER_SRC_FILES}")
# Add the source files to the target
add_library(${PROJECT_NAME} ${LOGGER_SRC_FILES} LCLoggerBase.hpp LCLoggerBase.cpp ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC  log4cplus::log4cplus fmt::fmt glog::glog spdlog::spdlog dlt-daemon::dlt-daemon)
#设置运行时依赖在cmake主目录的 third-party

#target_precompile_headers(${PROJECT_NAME} PUBLIC  )



install(TARGETS ${PROJECT_NAME} DESTINATION lib)