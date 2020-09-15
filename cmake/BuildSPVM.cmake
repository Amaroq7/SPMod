include_guard(DIRECTORY)

include(${CMAKE_SOURCE_DIR}/cmake/InstallAMBuild.cmake)

message(STATUS "Building SPVM")
set(SPVM_SOURCE_DIR "${CMAKE_BINARY_DIR}/spvm-project-src")
set(SPVM_BINARY_DIR "${CMAKE_BINARY_DIR}/spvm-project-build")
include(ExternalProject)

if (BUILD_DYNAMIC)
    set(SPVM_DYNAMIC_LINK --dynamic)
else()
    set(SPVM_DYNAMIC_LINK "")
endif()

if (UNIX)
    ExternalProject_Add(spvm-project
            GIT_REPOSITORY    https://github.com/alliedmodders/sourcepawn.git
            GIT_TAG           d7d94319a58e7d800a5e095000efe8ba64bd6c86
            GIT_PROGRESS      ON
            SOURCE_DIR        ${SPVM_SOURCE_DIR}
            BINARY_DIR        ${SPVM_BINARY_DIR}
            PATCH_COMMAND     ${BASH_EXEC} ${CMAKE_SOURCE_DIR}/patches/spvm_patch.sh ${PATCH_EXEC} ${SPVM_SOURCE_DIR} ${CMAKE_SOURCE_DIR}
            COMMAND           sed -i "s|<LLD>|${LLD}|g" ${SPVM_SOURCE_DIR}/AMBuildScript
            COMMAND           sed -i "s|<LLVM_LIBS_DIR>|${LLVM_LIBS_DIR}|g" ${SPVM_SOURCE_DIR}/AMBuildScript
            CONFIGURE_COMMAND CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ${Python3_EXECUTABLE} ${SPVM_SOURCE_DIR}/configure.py --enable-optimize --build=core --targets=x86 ${SPVM_DYNAMIC_LINK}
            BUILD_COMMAND     ${AMBUILD_EXEC}
            INSTALL_COMMAND   ""
    )
else()
endif()

if (LLVM_BUILD)
    add_dependencies(spvm-project ${LLVM_LIBS})
endif()

add_dependencies(spvm-project ambuild-project)