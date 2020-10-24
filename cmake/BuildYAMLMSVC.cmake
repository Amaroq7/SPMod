include_guard(GLOBAL)

# Building YAML
include(FetchContent)
FetchContent_Declare(
        yaml-cpp-project
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG        yaml-cpp-0.6.3
        GIT_SHALLOW    ON
)

set(YAML_CPP_BUILD_TESTS OFF CACHE INTERNAL "")
set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE INTERNAL "")
set(YAML_BUILD_SHARED_LIBS ${BUILD_DYNAMIC} CACHE INTERNAL "")
set(YAML_MSVC_SHARED_RT ${BUILD_DYNAMIC} CACHE INTERNAL "")
FetchContent_MakeAvailable(yaml-cpp-project)

set(YAML_CPP_INCLUDE_DIR "${yaml-cpp-project_SOURCE_DIR}/include")
set(YAML_CPP_LIBRARIES "yaml-cpp")
if (WIN32)
    target_compile_definitions(${YAML_CPP_LIBRARIES} PRIVATE /wd4251 /wd4275)
endif()