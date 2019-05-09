cmake_minimum_required(VERSION 3.9)
project(tefri-tests)

include_directories(
    ../include
    ../lib
    ../lib/metaxxa
)

file(GLOB TEST_SRC
    "*.h"
    "*.cpp"
)

add_executable(tefri-tests ${TEST_SRC})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

    find_package(Threads)

    target_link_libraries(tefri-tests ${CMAKE_THREAD_LIBS_INIT})
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
