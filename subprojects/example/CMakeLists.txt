project(example)

set(SRC_FILES DummyPluginMngr.cpp
              ext.cpp)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

include(${CMAKE_SOURCE_DIR}/cmake/CompilerSetup.cmake)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/public)

if (UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32)
    target_link_options(${PROJECT_NAME} PRIVATE -m32)
else()
    target_link_options(${PROJECT_NAME} PRIVATE /MACHINE:X86)
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
)
