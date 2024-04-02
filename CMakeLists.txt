cmake_minimum_required(VERSION 3.15)
project(LC_LOG CXX)

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/third-party")
set(CXX_STANDARD 20)

add_library(${PROJECT_NAME} SHARED src/lib_main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)


find_package(fmt REQUIRED)
find_package(glog REQUIRED)
find_package(log4cplus REQUIRED)
find_package(spdlog REQUIRED)
find_package(dlt-daemon REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC  log4cplus::log4cplus fmt::fmt glog::glog spdlog::spdlog dlt-daemon::dlt-daemon)
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                  COMMAND ${CMAKE_COMMAND} -E echo "Linking libraries...")


# 
# Specify the installation directory for the executable 
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

