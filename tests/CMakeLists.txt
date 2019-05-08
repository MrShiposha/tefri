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

if(${CMAKE_SYSTEM_NAME} STREQUAL "linux")
    target_link_libraries(tefri-tests pthread)
endif(${CMAKE_SYSTEM_NAME} STREQUAL "linux")