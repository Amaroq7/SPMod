include_guard(GLOBAL)

set(THIRD_PARTY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party)
set(ANUBIS_VERSION v2.0.0)

if (UNIX)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 12.0.0)
            set(ANUBIS_COMPILER clang-12)
        else()
            message(FATAL_ERROR "Clang 12 is required")
        endif()
    else()
        if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 11.0.0)
            set(ANUBIS_COMPILER gcc-11)
        else()
            message(FATAL_ERROR "GCC 11 is required")
        endif()
    endif()
    set(ANUBIS_OS linux)
    set(ANUBIS_ARCHIVE_EXT tar.gz)
else()
    set(ANUBIS_COMPILER msvc2022)
    set(ANUBIS_OS win32)
    set(ANUBIS_ARCHIVE_EXT zip)
endif()

if (NOT EXISTS ${THIRD_PARTY_INCLUDE_DIR}/anubis)
    message(STATUS "Downloading Anubis SDK")

    file(DOWNLOAD https://github.com/Amaroq7/anubis/releases/download/${ANUBIS_VERSION}/anubis-${ANUBIS_OS}-${ANUBIS_VERSION}-${ANUBIS_COMPILER}.${ANUBIS_ARCHIVE_EXT} ${THIRD_PARTY_INCLUDE_DIR}/anubissdk.${ANUBIS_ARCHIVE_EXT})

    file(ARCHIVE_EXTRACT INPUT ${THIRD_PARTY_INCLUDE_DIR}/anubissdk.${ANUBIS_ARCHIVE_EXT} DESTINATION ${THIRD_PARTY_INCLUDE_DIR}/temp_anubis)
    file(COPY ${THIRD_PARTY_INCLUDE_DIR}/temp_anubis/sdk/include/anubis DESTINATION ${THIRD_PARTY_INCLUDE_DIR})

    file(COPY ${THIRD_PARTY_INCLUDE_DIR}/temp_anubis/sdk/include/yaml-cpp DESTINATION ${THIRD_PARTY_INCLUDE_DIR})
    file(COPY ${THIRD_PARTY_INCLUDE_DIR}/temp_anubis/sdk/lib DESTINATION ${THIRD_PARTY_INCLUDE_DIR})

    file(REMOVE_RECURSE ${THIRD_PARTY_INCLUDE_DIR}/temp_anubis)
    file(REMOVE ${THIRD_PARTY_INCLUDE_DIR}/anubissdk.${ANUBIS_ARCHIVE_EXT})

    set(YAML_CPP_LIBRARIES "libyaml-cpp$<$<CONFIG:Debug>:d>")
endif()