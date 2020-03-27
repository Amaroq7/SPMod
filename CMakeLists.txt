cmake_minimum_required(VERSION 3.15)

if (WIN32)
    cmake_policy(SET CMP0091 NEW)
    cmake_policy(SET CMP0092 NEW)
endif()

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
        message(WARNING "Building LLVM for clang older than ${CLANG_MINIMUM_VERSION} is not supported. Using gcc backend instead...")
        set(LLVM_BUILD OFF)
    else()
        message(STATUS "Building LLVM")
        set(LLVM_SOURCE_DIR "${CMAKE_BINARY_DIR}/llvm-project-src")
        set(LLVM_BINARY_DIR "${CMAKE_BINARY_DIR}/llvm-project-build")
        string(FIND ${CMAKE_CXX_COMPILER_VERSION} "." FIRST_DOT)
        string(SUBSTRING ${CMAKE_CXX_COMPILER_VERSION} 0 ${FIRST_DOT} CLANG_MAJOR_VER)
        include(ExternalProject)
        ExternalProject_Add(llvm-project
            GIT_REPOSITORY    https://github.com/llvm/llvm-project.git
            GIT_TAG           origin/release/${CLANG_MAJOR_VER}.x
            GIT_SHALLOW       ON
            GIT_PROGRESS      ON
            SOURCE_DIR        ${LLVM_SOURCE_DIR}
            BINARY_DIR        ${LLVM_BINARY_DIR}
            CMAKE_ARGS
                -DCMAKE_BUILD_TYPE=Release
                -DCMAKE_SHARED_LINKER_FLAGS=-L${LLVM_BINARY_DIR}/lib\ -l:libclang_rt.builtins-i386.a
                -DLLVM_ENABLE_PROJECTS=libcxx$<SEMICOLON>libcxxabi$<SEMICOLON>libunwind
                -DLLVM_TARGETS_TO_BUILD=X86
                -DLLVM_INCLUDE_EXAMPLES=OFF
                -DLLVM_INCLUDE_TESTS=OFF
                -DLLVM_INCLUDE_BENCHMARKS=OFF
                -DLLVM_BUILD_32_BITS=ON
                -DLLVM_USE_LINKER=lld
                -DLLVM_ENABLE_LIBCXX=ON
                -DLLVM_INCLUDE_TOOLS=OFF
                -DLLVM_BUILD_RUNTIME=ON
                -DLIBUNWIND_USE_COMPILER_RT=ON
                -DLIBUNWIND_ENABLE_ASSERTIONS=OFF
                -DLIBUNWIND_HERMETIC_STATIC_LIBRARY=ON
                -DLIBCXXABI_USE_COMPILER_RT=ON
                -DLIBCXXABI_USE_LLVM_UNWINDER=ON
                -DLIBCXXABI_HERMETIC_STATIC_LIBRARY=ON
                -DLIBCXXABI_LIBCXX_INCLUDES=${LLVM_SOURCE_DIR}/libcxx/include
                -DLIBCXX_USE_COMPILER_RT=ON
                -DLIBCXX_CXX_ABI=libcxxabi
                -DLIBCXX_HERMETIC_STATIC_LIBRARY=ON
                -DLIBCXX_CXX_ABI_INCLUDE_PATHS=${LLVM_SOURCE_DIR}/libcxxabi/include
            SOURCE_SUBDIR     llvm
            BUILD_COMMAND     make unwind cxxabi cxx
            INSTALL_COMMAND   ""
        )
        add_library(unwind SHARED IMPORTED GLOBAL)
        set_target_properties(unwind PROPERTIES IMPORTED_LOCATION ${LLVM_BINARY_DIR}/lib/libunwind.so.1.0)
        add_library(c++abi SHARED IMPORTED GLOBAL)
        set_target_properties(c++abi PROPERTIES IMPORTED_LOCATION ${LLVM_BINARY_DIR}/lib/libc++abi.so.1.0)
        add_library(c++ SHARED IMPORTED GLOBAL)
        set_target_properties(c++ PROPERTIES IMPORTED_LOCATION ${LLVM_BINARY_DIR}/lib/libc++.so.1.0)
        set(LLVM_BUILD ON)
    endif()
endif()

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -pedantic-errors")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
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

add_subdirectory(src core)
add_subdirectory(subprojects)

# install scripts
install(
    DIRECTORY scripting
    DESTINATION .
)

# install libc++abi & libc++ libs
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND LLVM_BUILD)

    find_library(
        LIBCXXABI c++abi
        PATHS ${LLVM_BINARY_DIR}/lib
        NO_DEFAULT_PATH
    )

    if (LIBCXXABI)
        get_filename_component(LIBCXXABI_DIR ${LIBCXXABI} DIRECTORY)
        get_filename_component(LIBCXXABI ${LIBCXXABI} NAME)

        # resolve any symlinks
        get_filename_component(
            LIBCXXABI ${LIBCXXABI}
            REALPATH
            BASE_DIR ${LIBCXXABI_DIR}
        )

        install(FILES
                ${LIBCXXABI}
                DESTINATION libs
                RENAME libc++abi.so.1
        )
    endif()

    find_library(
        LIBCXX c++
        PATHS ${LLVM_BINARY_DIR}/lib
        NO_DEFAULT_PATH
    )

    if (LIBCXX)
        get_filename_component(LIBCXX_DIR ${LIBCXX} DIRECTORY)
        get_filename_component(LIBCXX ${LIBCXX} NAME)

        # resolve any symlinks
        get_filename_component(
            LIBCXX ${LIBCXX}
            REALPATH
            BASE_DIR ${LIBCXX_DIR}
        )

        install(FILES
                ${LIBCXX}
                DESTINATION libs
                RENAME libc++.so.1
        )
    endif()

    find_library(
        LIBUNWIND unwind
        PATHS ${LLVM_BINARY_DIR}/lib
        NO_DEFAULT_PATH
    )

    if (LIBUNWIND)
        get_filename_component(LIBUNWIND_DIR ${LIBUNWIND} DIRECTORY)
        get_filename_component(LIBUNWIND ${LIBUNWIND} NAME)

        # resolve any symlinks
        get_filename_component(
            LIBUNWIND ${LIBUNWIND}
            REALPATH
            BASE_DIR ${LIBUNWIND_DIR}
        )

        install(FILES
            ${LIBUNWIND}
            DESTINATION libs
            RENAME libunwind.so.1
        )
    endif()
endif()
