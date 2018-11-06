CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
PROJECT(tefri-tests)

INCLUDE_DIRECTORIES(../include)

MACRO(REQUIRE_CXX_17)
    IF (MSVC_VERSION GREATER_EQUAL "1900")
        INCLUDE(CheckCXXCompilerFlag)
        CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
        IF (_cpp_latest_flag_supported)
            ADD_COMPILE_OPTIONS("/std:c++latest")
        ELSE()
            MESSAGE(FATAL_ERROR "C++17 is required")
        ENDIF()
    ENDIF()

    SET(CMAKE_CXX_STANDARD 17)
ENDMACRO()

REQUIRE_CXX_17()

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} src/main.cpp)