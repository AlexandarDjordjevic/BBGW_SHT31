cmake_minimum_required(VERSION 2.8)

project(BBGW_SHT31)
set (CMAKE_CXX_FLAGS "-static-libstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(${PROJECT_NAME} main.cpp sht3x.cpp sht3x.h i2c.cpp i2c.h)
