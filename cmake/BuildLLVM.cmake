include_guard(GLOBAL)

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
        INSTALL_DIR       "${CMAKE_BINARY_DIR}/install"
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_CXX_EXTENSIONS=OFF
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_EXE_LINKER_FLAGS=-L${LLVM_BINARY_DIR}/lib
        -DCMAKE_SHARED_LINKER_FLAGS=-L${LLVM_BINARY_DIR}/lib\ -l:libclang_rt.builtins-i386.a
        -DLLVM_ENABLE_PROJECTS=libcxx$<SEMICOLON>libcxxabi$<SEMICOLON>libunwind
        -DLLVM_TARGETS_TO_BUILD=X86
        -DLLVM_INCLUDE_EXAMPLES=OFF
        -DLLVM_INCLUDE_TESTS=OFF
        -DLLVM_BUILD_RUNTIMES=OFF
        -DLLVM_ENABLE_Z3_SOLVER=OFF
        -DLLVM_INCLUDE_BENCHMARKS=OFF
        -DLLVM_BUILD_32_BITS=ON
        -DLLVM_USE_LINKER=lld
        -DLLVM_ENABLE_LIBCXX=ON
        -DLLVM_INCLUDE_TOOLS=OFF
        -DLLVM_BUILD_TOOLS=OFF
        -DLLVM_BUILD_UTILS=OFF
        -DLLVM_INCLUDE_RUNTIMES=OFF
        -DLLVM_INCLUDE_UTILS=OFF
        -DLLVM_ENABLE_PLUGINS=OFF
        -DLLVM_BUILD_RUNTIME=ON
        -DLIBUNWIND_USE_COMPILER_RT=ON
        -DLIBUNWIND_ENABLE_ASSERTIONS=OFF
        -DLIBUNWIND_HERMETIC_STATIC_LIBRARY=ON
        -DLIBCXXABI_USE_COMPILER_RT=ON
        -DLIBCXXABI_USE_LLVM_UNWINDER=ON
        -DLIBCXXABI_ENABLE_STATIC_UNWINDER=$<IF:$<BOOL:BUILD_DYNAMIC>,OFF,ON>
        -DLIBCXXABI_HERMETIC_STATIC_LIBRARY=ON
        -DLIBCXXABI_LIBCXX_INCLUDES=${LLVM_SOURCE_DIR}/libcxx/include
        -DLIBCXX_USE_COMPILER_RT=ON
        -DLIBCXX_CXX_ABI=libcxxabi
        -DLIBCXX_HERMETIC_STATIC_LIBRARY=ON
        -DLIBCXX_CXX_ABI_INCLUDE_PATHS=${LLVM_SOURCE_DIR}/libcxxabi/include
        SOURCE_SUBDIR     llvm
        BUILD_COMMAND     ${CMAKE_MAKE_PROGRAM} unwind cxxabi cxx
        INSTALL_COMMAND   ${CMAKE_MAKE_PROGRAM} install unwind cxxabi cxx
        )
if (BUILD_DYNAMIC)
    add_library(unwind SHARED IMPORTED GLOBAL)
    set_target_properties(unwind PROPERTIES IMPORTED_LOCATION ${LLVM_BINARY_DIR}/lib/libunwind.so.1.0)
    add_library(c++abi SHARED IMPORTED GLOBAL)
    set_target_properties(c++abi PROPERTIES IMPORTED_LOCATION ${LLVM_BINARY_DIR}/lib/libc++abi.so.1.0)
    add_library(c++ SHARED IMPORTED GLOBAL)
    set_target_properties(c++ PROPERTIES IMPORTED_LOCATION ${LLVM_BINARY_DIR}/lib/libc++.so.1.0)
else()
    add_library(unwind STATIC IMPORTED GLOBAL)
    set_target_properties(unwind PROPERTIES IMPORTED_LOCATION ${LLVM_BINARY_DIR}/lib/libunwind.a)
    add_library(c++abi STATIC IMPORTED GLOBAL)
    set_target_properties(c++abi PROPERTIES IMPORTED_LOCATION ${LLVM_BINARY_DIR}/lib/libc++abi.a)
    add_library(c++ STATIC IMPORTED GLOBAL)
    set_target_properties(c++ PROPERTIES IMPORTED_LOCATION ${LLVM_BINARY_DIR}/lib/libc++.a)
endif()
set(LLVM_BUILD ON)