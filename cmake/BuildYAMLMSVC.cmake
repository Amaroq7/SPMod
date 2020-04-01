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
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DCMAKE_CXX_EXTENSIONS=OFF
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DYAML_BUILD_SHARED_LIBS=${BUILD_DYNAMIC}
        -DYAML_MSVC_SHARED_RT=${BUILD_DYNAMIC}
        -DYAML_CPP_BUILD_CONTRIB=OFF
        -DYAML_CPP_BUILD_TESTS=OFF
        -DYAML_CPP_BUILD_TOOLS=OFF
        )

ExternalProject_Get_Property(yaml-cpp-project INSTALL_DIR)

if (BUILD_DYNAMIC)
    add_library(yaml-cpp SHARED IMPORTED GLOBAL)
    set_target_properties(yaml-cpp
                          PROPERTIES
                          IMPORTED_LOCATION ${INSTALL_DIR}/bin/libyaml-cpp.dll
                          IMPORTED_IMPLIB ${INSTALL_DIR}/lib/yaml-cpp.lib
    )
else()
    add_library(yaml-cpp STATIC IMPORTED GLOBAL)
    set_target_properties(yaml-cpp PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/libyaml-cppmt.lib)
endif()

set(YAML_CPP_INCLUDE_DIR "${INSTALL_DIR}/include")
set(YAML_CPP_LIBRARIES "yaml-cpp")
