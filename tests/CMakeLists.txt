cmake_minimum_required(VERSION 3.9)
project(tefri-tests)

include_directories(
    ../include
    ../lib
)

file(GLOB TEST_SRC
    "*.h"
    "*.cpp"
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "linux")
    add_definitions(-pthread)
endif(${CMAKE_SYSTEM_NAME} STREQUAL "linux")

add_executable(tefri-tests ${TEST_SRC})