include_guard(DIRECTORY)
if (UNIX)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/third_party/lib)
        target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
        target_link_libraries(${PROJECT_NAME} PRIVATE c++ c++abi unwind)
        target_link_options(${PROJECT_NAME} PRIVATE -fuse-ld=${LLD} -stdlib=libc++ --rtlib=compiler-rt)
    elseif (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR "Either Clang or GCC is supported.")
    endif()
else()
    if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(FATAL_ERROR "Only MSVC is supported.")
    endif()
endif()
