project(spmod_mm)

# Generate SPConfig.hpp
find_program(GIT git)
if (GIT AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(VERSION ${CMAKE_PROJECT_VERSION})
    execute_process(COMMAND ${GIT} rev-parse --short HEAD OUTPUT_VARIABLE COMMIT OUTPUT_STRIP_TRAILING_WHITESPACE)
    configure_file(SPConfig.hpp.in SPConfig.hpp @ONLY)
else()
    set(VERSION ${CMAKE_PROJECT_VERSION})
    set(COMMIT "<unknown>")
    configure_file(SPConfig.hpp.in SPConfig.hpp @ONLY)
endif()

set(SRC_FILES SPGlobal.cpp
              CmdSystem.cpp
              ForwardSystem.cpp
              PlayerSystem.cpp
              LoggingSystem.cpp
              TimerSystem.cpp
              MenuSystem.cpp
              ExtensionSystem.cpp
              UtilsSystem.cpp
              NativeProxy.cpp
              MessageSystem.cpp
              MetaInit.cpp
              EngineHooks.cpp
              DLLHooks.cpp
              Hooks.cpp)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

include(${CMAKE_SOURCE_DIR}/cmake/CompilerSetup.cmake)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32 -ldl)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Wall -Wextra -Werror -Wpedantic -pedantic-errors)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
    target_link_options(${PROJECT_NAME} PRIVATE /MACHINE:X86)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE SPMOD_CORE)

target_include_directories(
    ${PROJECT_NAME}
    SYSTEM
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include/public
    ${CMAKE_SOURCE_DIR}/include/metamodcpp_sdk
    ${CMAKE_CURRENT_BINARY_DIR}
    ${YAML_CPP_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${YAML_CPP_LIBRARIES})

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
)

add_dependencies(${PROJECT_NAME}
                 ${YAML_CPP_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY
            DESTINATION dlls
)
