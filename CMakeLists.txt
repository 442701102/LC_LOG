cmake_minimum_required(VERSION 3.15)
project(libLC_LOG CXX)

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/third-party")

add_library(${PROJECT_NAME} SHARED)
target_include_directories(${PROJECT_NAME} PUBLIC include)
#find_package(dlt-daemon REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(dlt-daemon REQUIRED)
find_package(log4cplus REQUIRED)
find_package(glog REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC dlt-daemon::dlt-daemon spdlog::spdlog log4cplus::log4cplus glog::glog fmt::fmt)
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                  COMMAND ${CMAKE_COMMAND} -E echo "Linking libraries...")


# 
# Specify the installation directory for the executable 
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

