FUNCTION(INITIALIZE_SOURCE_GROUPS)
    FOREACH(_source IN ITEMS ${ARGN})
        IF(IS_ABSOLUTE "${_source}")
            FILE(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        ELSE()
            SET(_source_rel "${_source}")
        ENDIF()
        GET_FILENAME_COMPONENT(_source_path "${_source_rel}" PATH)
        STRING(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        SOURCE_GROUP("${_source_path_msvc}" FILES "${_source}")
    ENDFOREACH()
ENDFUNCTION()

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
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
ENDMACRO()

MACRO(MAKE_HEADER_ONLY)
    FIND_PACKAGE(Python3 COMPONENTS Interpreter)
    IF(${Python3_Interpreter_FOUND})
        ADD_CUSTOM_TARGET(
            make_header_only
            COMMAND ${Python3_EXECUTABLE} "buildheaderonly.py"
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            COMMENT "Creating release header"
        )
    ELSE()
        MESSAGE(FATAL_ERROR "python3 is not found")
    ENDIF()
ENDMACRO()