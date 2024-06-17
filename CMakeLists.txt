cmake_minimum_required(VERSION 3.28)
project(Image_Manipulator)

set(CMAKE_CXX_STANDARD 11)
FIND_PACKAGE( Boost 1.66.0 COMPONENTS program_options algorithm REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )

add_executable(Image_Manipulator src/main.cpp
        src/util.cpp
        src/util.h
        src/image_functions.cpp
        src/image_functions.h
)
include_directories(Image_Manipulator, lib)

TARGET_LINK_LIBRARIES( Image_Manipulator LINK_PUBLIC ${Boost_LIBRARIES} )