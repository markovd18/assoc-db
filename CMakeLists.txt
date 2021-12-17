#
# Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 7.11.21.
#

cmake_minimum_required (VERSION 3.1)
project ("kiv-cpp-sp-02")

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

set(LINK_STDCPP_FS FALSE)
IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    ADD_DEFINITIONS(-Wall -Wextra -Wno-ignored-attributes -Wno-deprecated-declarations -Wno-unknown-pragmas -Wno-unused-parameter -fPIC)
    set(LINK_STDCPP_FS TRUE)
    ADD_DEFINITIONS(-fconcepts)

    # gcc reports a specific use case of copy semantics as deprecated, unlike Clang
    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        ADD_DEFINITIONS(-Wno-deprecated-copy -Wimplicit-fallthrough=0 -Wno-class-memaccess)
    ENDIF()
    # debug build = no optimization, add debug symbols
    SET(CMAKE_CXX_FLAGS_DEBUG "-g")
ENDIF()

# collecting all source files for linking to the executable
file(GLOB_RECURSE INCLUDE include/*.h include/*.cpp)
file(GLOB_RECURSE SRC src/*.h src/*.cpp)

# including all header files from 'include' directory for convenient global inlcuding
SET(INCLUDES "include" CACHE PATH "Public 'include' directory location")
INCLUDE_DIRECTORIES("${INCLUDES}")

set(SOURCES ${INCLUDE} ${SRC})

# Add source to this project's executable.
add_executable (kiv-cpp-sp-02 ${SOURCES})
#setting output directory for generated executable to the project root
set_target_properties(kiv-cpp-sp-02 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
if (LINK_STDCPP_FS)
    target_link_libraries(kiv-cpp-sp-02 stdc++fs)
endif ()