project(sourcepawn)

# Build sourcepawn lib
message(STATUS "Building SourcePawn library")
include(${CMAKE_SOURCE_DIR}/cmake/BuildSPVM.cmake)

set(SRC_FILES AdapterInterface.cpp
              CmdNatives.cpp
              CoreNatives.cpp
              CvarsNatives.cpp
              DebugListener.cpp
              ExtMain.cpp
              FloatNatives.cpp
              ForwardNatives.cpp
              MenuNatives.cpp
              MessageNatives.cpp
              PlayerNatives.cpp
              PluginSystem.cpp
              PrintfImpl.cpp
              SourcePawnAPI.cpp
              StringNatives.cpp
              TimerNatives.cpp
              VTableNatives.cpp
              HooksNatives.cpp
              TraceResultNatives.cpp
              NativeCallback.cpp)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})
add_dependencies(${PROJECT_NAME} spvm-project)

include(${CMAKE_SOURCE_DIR}/cmake/CompilerSetup.cmake)

if (UNIX)
    target_link_options(${PROJECT_NAME} PRIVATE -m32)
    target_compile_options(${PROJECT_NAME} PRIVATE -m32)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
else()
    target_link_options(${PROJECT_NAME} PRIVATE /MACHINE:X86)
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE sourcepawn/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/public
                                                   ${CMAKE_SOURCE_DIR}/include)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY
            DESTINATION exts
)

ExternalProject_Get_Property(spvm-project BINARY_DIR)

# SourcePawn lib
install(FILES
        ${BINARY_DIR}/vm/sourcepawn.vm/$<IF:$<PLATFORM_ID:Linux>,linux-x86,windows-x86>/sourcepawn.vm.$<IF:$<PLATFORM_ID:Linux>,so,dll>
        DESTINATION exts
)

install(PROGRAMS
        ${BINARY_DIR}/compiler/spcomp/$<IF:$<PLATFORM_ID:Linux>,linux-x86,windows-x86>/spcomp$<$<PLATFORM_ID:Windows>:.exe>
        DESTINATION scripting
)
