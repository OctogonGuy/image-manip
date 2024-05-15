cmake_minimum_required(VERSION 3.10)
project(ImageManip)
add_executable(ImageManip src/main.cpp)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(DIRS ${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/include)

add_library(util STATIC src/util.cpp)
target_include_directories(util PUBLIC ${DIRS})

add_library(image_functions STATIC src/image_functions.cpp)
target_include_directories(image_functions PUBLIC ${DIRS})

target_link_libraries(ImageManip PRIVATE
    image_functions
    util
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)