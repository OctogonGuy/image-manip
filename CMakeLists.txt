cmake_minimum_required(VERSION 3.28)
project(Image_Processor)

set(CMAKE_CXX_STANDARD 11)

add_executable(Image_Processor src/main.cpp
        src/util.cpp
        src/util.h
        src/image_functions.cpp
        src/image_functions.h
)
include_directories(Image_Manipulator, lib)