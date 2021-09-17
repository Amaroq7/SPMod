include_guard(GLOBAL)

# Building YAML
include(FetchContent)
FetchContent_Declare(
        fmtlib
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 8.1.1
        GIT_SHALLOW ON
)

set(FMT_DOC OFF CACHE INTERNAL "")
set(FMT_TEST OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(fmtlib)

if (UNIX)
    target_compile_options(fmt PUBLIC -m32)
    target_link_options(fmt PUBLIC -m32 PRIVATE -Wl,--disable-new-dtags)

    set_target_properties(fmt PROPERTIES POSITION_INDEPENDENT_CODE ON)

    if (IS_CLANG_COMPILER)
        target_compile_options(fmt PRIVATE -stdlib=libc++ -Wno-pedantic -Wno-user-defined-literals)
    endif ()
endif ()

set(FMT_INCLUDE_DIR ${fmtlib_SOURCE_DIR}/include)
set(FMT_LIBRARIES fmt)