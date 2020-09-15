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

set(SRC_FILES h_export.cpp
              engine_api.cpp
              dllapi.cpp
              meta_api.cpp
              SPGlobal.cpp
              RehldsApi.cpp
              CmdSystem.cpp
              ForwardSystem.cpp
              PlayerSystem.cpp
              LoggingSystem.cpp
              CvarSystem.cpp
              TimerSystem.cpp
              MenuSystem.cpp
              ExtensionSystem.cpp
              ValveInterface.cpp
              UtilsSystem.cpp
              NativeProxy.cpp
              engine/Funcs.cpp
              metamod/Funcs.cpp
              metamod/Metamod.cpp
              engine/Edict.cpp
              valve/BaseEntity.cpp
              valve/BasePlayer.cpp
              engine/EntVars.cpp
              VTableHookManager.cpp
              engine/Globals.cpp
              engine/TraceResult.cpp
              engine/Engine.cpp)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

include(${CMAKE_SOURCE_DIR}/cmake/CompilerSetup.cmake)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32 -ldl)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32 -Werror)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
    target_link_options(${PROJECT_NAME} PRIVATE /MACHINE:X86 /EXPORT:GiveFnptrsToDll=_GiveFnptrsToDll@8 /SECTION:.data,RW)
endif()

target_include_directories(
    ${PROJECT_NAME}
    SYSTEM
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include/metamod-r/metamod/src
    ${CMAKE_SOURCE_DIR}/include/rehlds
    ${CMAKE_SOURCE_DIR}/include/rehlds/rehlds/common
    ${CMAKE_SOURCE_DIR}/include/rehlds/rehlds/dlls
    ${CMAKE_SOURCE_DIR}/include/rehlds/rehlds/engine
    ${CMAKE_SOURCE_DIR}/include/rehlds/rehlds/game_shared
    ${CMAKE_SOURCE_DIR}/include/rehlds/rehlds/pm_shared
    ${CMAKE_SOURCE_DIR}/include/rehlds/rehlds/public
    ${CMAKE_SOURCE_DIR}/include/rehlds/rehlds/public/rehlds
    ${CMAKE_CURRENT_BINARY_DIR}
    ${YAML_CPP_INCLUDE_DIR}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/public)
target_link_libraries(${PROJECT_NAME} PUBLIC ${YAML_CPP_LIBRARIES})

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
)

add_dependencies(${PROJECT_NAME} ${YAML_CPP_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
        LIBRARY
            DESTINATION dlls
)
