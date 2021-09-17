include_guard(DIRECTORY)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
if (UNIX)
    find_program(BASH_EXEC bash REQUIRED)
endif()

if (UNIX)
    set(ENV{PATH} "$ENV{PATH}:$ENV{HOME}/.local/bin")
endif()

get_target_property(PYTHON_BIN Python3::Interpreter LOCATION)
execute_process(COMMAND ${PYTHON_BIN} -m pip install --user --upgrade pip setuptools wheel)
execute_process(COMMAND pip install git+https://github.com/alliedmodders/ambuild --user)

set(AMBUILD_EXEC "ambuild")

message(STATUS "Building SPVM")
set(SPVM_SOURCE_DIR "${CMAKE_BINARY_DIR}/spvm-project-src")
set(SPVM_BINARY_DIR "${CMAKE_BINARY_DIR}/spvm-project-build")
include(ExternalProject)

ExternalProject_Add(spvm-project
    GIT_REPOSITORY    https://github.com/alliedmodders/sourcepawn.git
    GIT_TAG           d7b2e0a2e814469b8b2ff2a24e9e3c86fc43e852
    GIT_PROGRESS      ON
    SOURCE_DIR        ${SPVM_SOURCE_DIR}
    BINARY_DIR        ${SPVM_BINARY_DIR}
    CONFIGURE_COMMAND ${Python3_EXECUTABLE} ${SPVM_SOURCE_DIR}/configure.py --enable-optimize --build=core --targets=x86 --symbol-files
    BUILD_COMMAND     PATH=$ENV{PATH} ${AMBUILD_EXEC}
    INSTALL_COMMAND   ""
)