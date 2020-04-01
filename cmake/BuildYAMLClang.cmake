include_guard(GLOBAL)

# Building YAML
include(ExternalProject)
ExternalProject_Add(yaml-cpp-project
        GIT_REPOSITORY    https://github.com/jbeder/yaml-cpp.git
        GIT_TAG           yaml-cpp-0.6.3
        GIT_SHALLOW       ON
        GIT_PROGRESS      ON
        SOURCE_DIR        "${CMAKE_BINARY_DIR}/yaml-cpp-src"
        BINARY_DIR        "${CMAKE_BINARY_DIR}/yaml-cpp-build"
        INSTALL_DIR       "${CMAKE_BINARY_DIR}/install"
        DEPENDS           "llvm-project"
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_CXX_EXTENSIONS=OFF
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY
        -DCMAKE_CXX_FLAGS=-m32\ -stdlib=libc++\ -fPIC
        -DCMAKE_SHARED_LINKER_FLAGS=-m32\ -L${LLVM_BINARY_DIR}/lib\ -fuse-ld=lld\ --rtlib=compiler-rt\ -lc++\ -lc++abi\ -lunwind
        -DCMAKE_STATIC_LINKER_FLAGS=${LLVM_BINARY_DIR}/lib/libc++.a\ ${LLVM_BINARY_DIR}/lib/libc++abi.a\ ${LLVM_BINARY_DIR}/lib/libunwind.a
        -DYAML_BUILD_SHARED_LIBS=${BUILD_DYNAMIC}
        -DYAML_CPP_BUILD_CONTRIB=OFF
        -DYAML_CPP_BUILD_TESTS=OFF
        -DYAML_CPP_BUILD_TOOLS=OFF
        )

ExternalProject_Get_Property(yaml-cpp-project INSTALL_DIR)

if (BUILD_DYNAMIC)
    add_library(yaml-cpp SHARED IMPORTED GLOBAL)
    set_target_properties(yaml-cpp PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libyaml-cpp.so.0.6.3)
else()
    add_library(yaml-cpp STATIC IMPORTED GLOBAL)
    set_target_properties(yaml-cpp PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libyaml-cpp.a)
endif()

set(YAML_CPP_INCLUDE_DIR "${INSTALL_DIR}/include")
set(YAML_CPP_LIBRARIES "yaml-cpp")
