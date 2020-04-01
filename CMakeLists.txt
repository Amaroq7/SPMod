cmake_minimum_required(VERSION 3.15)

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
option(BUILD_SPADAPTER "Build SourcePawn adapter" ON)
option(BUILD_EXAMPLE "Build example adapter" OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CLANG_MINIMUM_VERSION "10.0.0")

    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS CLANG_MINIMUM_VERSION)
        message(WARNING "Building LLVM for clang older than ${CLANG_MINIMUM_VERSION} is not supported. Using GCC backend instead...")
        set(LLVM_BUILD OFF)
    else()
        include(cmake/BuildLLVM.cmake)
    endif()
endif()

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -pedantic-errors")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0")
else()
    if (NOT BUILD_DYNAMIC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/third_party)
if (UNIX)
    if (LLVM_BUILD)
        include(cmake/BuildYAMLClang.cmake)
    else()
        include(cmake/BuildYAMLGCC.cmake)
    endif()
else()
    include(cmake/BuildYAMLMSVC.cmake)
endif()

add_subdirectory(src core)
add_subdirectory(subprojects)

# install scripts
install(
    DIRECTORY scripting
    DESTINATION .
)

# install libyaml
if (BUILD_DYNAMIC)
    ExternalProject_Get_Property(yaml-cpp-project INSTALL_DIR)
    if (UNIX)
        file(GLOB YAML_LIBRARY_FILES
             LIST_DIRECTORIES false
             "${INSTALL_DIR}/lib/*.so*"
        )
        install(FILES ${YAML_LIBRARY_FILES}
                DESTINATION libs
        )
    else()
        file(GLOB YAML_LIBRARY_FILES
                LIST_DIRECTORIES false
                "${INSTALL_DIR}/lib/*.dll"
        )
        install(FILES ${YAML_LIBRARY_FILES}
                DESTINATION libs
        )
    endif()
endif()

# install libc++abi & libc++ libs
if (LLVM_BUILD AND BUILD_DYNAMIC)
    ExternalProject_Get_Property(llvm-project INSTALL_DIR)
    file(GLOB LLVM_LIBRARY_FILES
            LIST_DIRECTORIES false
            "${LLVM_BINARY_DIR}/lib/*.so*"
    )
    install(FILES ${LLVM_LIBRARY_FILES}
            DESTINATION libs
    )
endif()
