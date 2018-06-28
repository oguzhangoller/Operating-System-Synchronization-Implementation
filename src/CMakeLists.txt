cmake_minimum_required(VERSION 3.6)
project(322Project2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Process.cpp Process.h Instruction.cpp Instruction.h Semaphore.cpp Semaphore.h)
add_executable(322Project2 ${SOURCE_FILES})