include_guard(DIRECTORY)

find_program(PYTHON_PIP pip3 REQUIRED)
if (UNIX)
    find_program(PATCH_EXEC patch REQUIRED)
    find_program(BASH_EXEC bash REQUIRED)
endif()
find_package(Python3 REQUIRED COMPONENTS Interpreter)

message(STATUS "Installing AMBuild")
set(AMBUILD_SOURCE_DIR "${CMAKE_BINARY_DIR}/ambuild-project-src")
include(ExternalProject)
ExternalProject_Add(ambuild-project
        GIT_REPOSITORY    https://github.com/alliedmodders/ambuild.git
        GIT_TAG           2.2
        GIT_SHALLOW       ON
        GIT_PROGRESS      ON
        SOURCE_DIR        ${AMBUILD_SOURCE_DIR}
        CONFIGURE_COMMAND ""
        PATCH_COMMAND     ${BASH_EXEC} ${CMAKE_SOURCE_DIR}/patches/ambuild_patch.sh ${PATCH_EXEC} ${AMBUILD_SOURCE_DIR} ${CMAKE_SOURCE_DIR}
        BUILD_COMMAND     ${Python3_EXECUTABLE} -m pip install ${AMBUILD_SOURCE_DIR} --user --force-reinstall
        INSTALL_COMMAND   ""
        )
set(AMBUILD_EXEC "$ENV{HOME}/.local/bin/ambuild")