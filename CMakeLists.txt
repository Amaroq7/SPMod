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
option(BUILD_DYNAMIC "Build SPMod as dynamically linked library" ON)
option(LLVM_TOOLCHAIN "Build SPMod using LLVM toolchain" OFF)
option(BUILD_SPADAPTER "Build SourcePawn adapter" ON)
option(BUILD_EXAMPLE "Build example adapter" OFF)

if (WIN32)
    if (NOT BUILD_DYNAMIC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
    include(cmake/BuildYAMLMSVC.cmake)
elseif (UNIX)
    include(cmake/BuildYAMLUnix.cmake)
endif()

add_subdirectory(src spmod)
add_subdirectory(subprojects)

# install scripts
install(
    DIRECTORY scripting
    DESTINATION .
)