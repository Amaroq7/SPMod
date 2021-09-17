cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0092 NEW)

project(SPMod
        VERSION 0.3.0
        DESCRIPTION "Scripting Engine for HL1"
        HOMEPAGE_URL "https://github.com/Amaroq7/SPMod"
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(BUILD_SPADAPTER "Build SourcePawn adapter" ON)
option(BUILD_EXAMPLE "Build example adapter" OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(IS_CLANG_COMPILER ON)

    string(FIND ${CMAKE_CXX_COMPILER_VERSION} "." FIRST_DOT)
    string(SUBSTRING ${CMAKE_CXX_COMPILER_VERSION} 0 ${FIRST_DOT} CLANG_MAJOR_VER)
    find_program(LLD NAMES lld-${CLANG_MAJOR_VER} lld REQUIRED)
    get_filename_component(LLD ${LLD} NAME)
endif()

if (WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party)

include(cmake/DownloadAnubisSDK.cmake)
include(cmake/BuildFMT.cmake)

add_subdirectory(src spmod)
add_subdirectory(subprojects)

# install scripts
install(
    DIRECTORY scripting
    DESTINATION .
)