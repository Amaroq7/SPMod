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

if(UNIX AND LLVM_BUILD)
    ExternalProject_Add(spvm-project
        GIT_REPOSITORY    https://github.com/alliedmodders/sourcepawn.git
        GIT_TAG           d7d94319a58e7d800a5e095000efe8ba64bd6c86
        GIT_PROGRESS      ON
        SOURCE_DIR        ${SPVM_SOURCE_DIR}
        BINARY_DIR        ${SPVM_BINARY_DIR}
        DEPENDS           ambuild-project
        DEPENDS           ${LLVM_LIBS}
        PATCH_COMMAND     ${BASH_EXEC} ${CMAKE_SOURCE_DIR}/patches/spvm_patch.sh ${PATCH_EXEC} ${SPVM_SOURCE_DIR} ${CMAKE_SOURCE_DIR}
        COMMAND           sed -i "s|<LLD>|${LLD}|g" ${SPVM_SOURCE_DIR}/AMBuildScript
        COMMAND           sed -i "s|<LLVM_LIBS_DIR>|${LLVM_LIBS_DIR}|g" ${SPVM_SOURCE_DIR}/AMBuildScript
        CONFIGURE_COMMAND CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ${Python3_EXECUTABLE} ${SPVM_SOURCE_DIR}/configure.py --enable-optimize --build=core --targets=x86 ${SPVM_DYNAMIC_LINK}
        BUILD_COMMAND     ${AMBUILD_EXEC}
        INSTALL_COMMAND   ""
    )
elseif(UNIX)
    ExternalProject_Add(spvm-project
            GIT_REPOSITORY    https://github.com/alliedmodders/sourcepawn.git
            GIT_TAG           d7d94319a58e7d800a5e095000efe8ba64bd6c86
            GIT_PROGRESS      ON
            SOURCE_DIR        ${SPVM_SOURCE_DIR}
            BINARY_DIR        ${SPVM_BINARY_DIR}
            DEPENDS           ambuild-project
            PATCH_COMMAND     ${BASH_EXEC} ${CMAKE_SOURCE_DIR}/patches/spvm_patch.sh ${PATCH_EXEC} ${SPVM_SOURCE_DIR} ${CMAKE_SOURCE_DIR}
            COMMAND           sed -i "s|<LLD>|${LLD}|g" ${SPVM_SOURCE_DIR}/AMBuildScript
            COMMAND           sed -i "s|<LLVM_LIBS_DIR>|${LLVM_LIBS_DIR}|g" ${SPVM_SOURCE_DIR}/AMBuildScript
            CONFIGURE_COMMAND CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ${Python3_EXECUTABLE} ${SPVM_SOURCE_DIR}/configure.py --enable-optimize --build=core --targets=x86 ${SPVM_DYNAMIC_LINK}
            BUILD_COMMAND     ${AMBUILD_EXEC}
            INSTALL_COMMAND   ""
    )
else()
    set(WIN_THIRD_PARTY ${CMAKE_BINARY_DIR}/third_party_win_deps)
    set(SHA256_ZIP b9e07417ca08becada20f08e923021a80a9fb09f86d2f4ae48a4f8d275d578d3)
    if (NOT EXISTS ${WIN_THIRD_PARTY})
        file(MAKE_DIRECTORY ${WIN_THIRD_PARTY})
    endif()
    find_program(PWSH_EXEC
                 pwsh
                 REQUIRED)
    function(downloadPatchZip)
        execute_process(COMMAND
                        ${PWSH_EXEC} -command "Invoke-WebRequest -Method Get -Uri 'https://spmod.eu/build-deps/patch-2.7.6.zip' -OutFile 'patch.zip'"
                        WORKING_DIRECTORY ${WIN_THIRD_PARTY})
        file(SHA256 ${WIN_THIRD_PARTY}/patch.zip SHA256_ZIP_DOWNLOADED)
        if (NOT ${SHA256_ZIP} EQUAL ${SHA256_ZIP_DOWNLOADED} AND DEFINED ALREADY_CONFIGURED_SPVM)
            message(FATAL_ERROR "Checksums are not equal (patch utility for windows)")
        endif()
    endfunction()

    if (NOT EXISTS ${WIN_THIRD_PARTY}/patch)
        if (NOT EXISTS ${WIN_THIRD_PARTY}/patch.zip)
            message(VERBOSE "Patch archive not found. Downloading...")
            downloadPatchZip()
        endif()
        execute_process(COMMAND
                        ${PWSH_EXEC} -command "Expand-Archive -Force 'patch.zip' '.'"
                        WORKING_DIRECTORY ${WIN_THIRD_PARTY})
    endif()
    find_program(PATCH_EXEC
                 patch
                 PATHS ${WIN_THIRD_PARTY}
                 REQUIRED
    )
    ExternalProject_Add(spvm-project
            GIT_REPOSITORY    https://github.com/alliedmodders/sourcepawn.git
            GIT_TAG           d7d94319a58e7d800a5e095000efe8ba64bd6c86
            GIT_PROGRESS      ON
            SOURCE_DIR        ${SPVM_SOURCE_DIR}
            BINARY_DIR        ${SPVM_BINARY_DIR}
            DEPENDS           ambuild-project
            PATCH_COMMAND     ${PATCH_EXEC} --verbose ${SPVM_SOURCE_DIR}/AMBuildScript ${CMAKE_SOURCE_DIR}/patches/AMBuildScript.patch
            COMMAND           ${PATCH_EXEC} --verbose ${SPVM_SOURCE_DIR}/configure.py ${CMAKE_SOURCE_DIR}/patches/configurepy.patch
            CONFIGURE_COMMAND ${Python3_EXECUTABLE} ${SPVM_SOURCE_DIR}/configure.py --enable-optimize --build=core --targets=x86 ${SPVM_DYNAMIC_LINK}
            BUILD_COMMAND     ${AMBUILD_EXEC}
            INSTALL_COMMAND   ""
    )
    set(ALREADY_CONFIGURED_SPVM ON)
endif()