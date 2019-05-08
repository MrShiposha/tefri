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

add_executable(tefri-tests ${TEST_SRC})