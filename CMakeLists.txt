cmake_minimum_required(VERSION 3.10)
project(ImageManip)
add_executable(ImageManip src/main.cpp)

add_subdirectory(lib)
target_link_libraries(ImageManip PRIVATE lib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)