project(sourcepawn)

# Build sourcepawn lib
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sourcepawn_deps)
    message(STATUS "Building SourcePawn library")
    if (UNIX)
        execute_process(
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build_sourcepawnlib.sh
            ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE RESULT
        )
        if (NOT RESULT EQUAL 0)
            message(WARNING "Failed to build SourcePawn library")
        endif()
    else()
        message(VERBOSE "Finding PowerShell Core")
        find_program(PWSH pwsh)
        if (NOT PWSH)
            message(VERBOSE "PowerShell Core not found. Searching for PowerShell instead...")
            find_program(PWSH powershell)
            if (NOT PWSH)
                message(VERBOSE "PowerShell could not be found")
            endif()
        endif()

        if (PWSH)
            execute_process(
                COMMAND ${PWSH}
                -ExecutionPolicy Bypass
                -File ${CMAKE_CURRENT_SOURCE_DIR}\\build_sourcepawnlib.ps1 ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
                -Scope CurrentUser
                RESULT_VARIABLE RESULT
            )

            if (NOT RESULT EQUAL 0)
                message(WARNING "Failed to build SourcePawn library")
            endif()
        else()
            message(WARNING "Could not build SourcePawn library")
        endif()
    endif()
endif()

set(SRC_FILES AdapterInterface.cpp
              CmdNatives.cpp
              CoreNatives.cpp
              CvarsNatives.cpp
              DebugListener.cpp
              ExtMain.cpp
              FloatNatives.cpp
              ForwardsNatives.cpp
              Listeners.cpp
              MenuNatives.cpp
              MessageNatives.cpp
              PlayerNatives.cpp
              PluginSystem.cpp
              PrintfImpl.cpp
              SourcePawnAPI.cpp
              StringNatives.cpp
              TimerNatives.cpp)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

include(${CMAKE_SOURCE_DIR}/CompilerSetup.cmake)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32 -ldl)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32)
else()
    target_link_options(${PROJECT_NAME} PRIVATE /MACHINE:X86)
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE sourcepawn/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/public)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CXX_VISIBILITY_PRESET hidden
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY
            DESTINATION exts
)

# SourcePawn lib
install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/sourcepawn_deps/sourcepawn_build/vm/sourcepawn.jit.x86/sourcepawn.jit.x86.$<IF:$<PLATFORM_ID:Linux>,so,dll>
        DESTINATION exts
)

install(PROGRAMS
        ${CMAKE_CURRENT_SOURCE_DIR}/sourcepawn_deps/sourcepawn_build/compiler/spcomp/spcomp$<$<PLATFORM_ID:Windows>:.exe>
        DESTINATION scripting
)
