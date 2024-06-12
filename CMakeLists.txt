cmake_minimum_required(VERSION 3.28)
project(Image_Manipulator)

set(CMAKE_CXX_STANDARD 11)

add_executable(Image_Manipulator src/main.cpp
        src/util.cpp
        src/util.h
        src/image_functions.cpp
        src/image_functions.h
)
include_directories(Image_Manipulator, lib)