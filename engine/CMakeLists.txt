cmake_minimum_required(VERSION 3.22)
project(ProjectAmigoNative VERSION 0.2.0 LANGUAGES CXX)

option(AMIGO_BUILD_ANDROID "Build the Android JNI shared library" OFF)
option(AMIGO_BUILD_TESTS "Build native runtime tests" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(amigo_core STATIC
    src/runtime.cpp
    src/rhythm.cpp
)
target_include_directories(amigo_core PUBLIC include)
set_target_properties(amigo_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(AMIGO_BUILD_ANDROID)
    if(NOT ANDROID)
        message(FATAL_ERROR "AMIGO_BUILD_ANDROID requires an Android NDK toolchain.")
    endif()
    add_library(amigo_android SHARED android/jni_bridge.cpp)
    target_link_libraries(amigo_android PRIVATE amigo_core)
    target_include_directories(amigo_android PRIVATE include)
    set_target_properties(amigo_android PROPERTIES OUTPUT_NAME "project_amigo")
else()
    add_executable(amigo_inspect src/main.cpp)
    target_link_libraries(amigo_inspect PRIVATE amigo_core)
    if(AMIGO_BUILD_TESTS)
        enable_testing()
        add_executable(amigo_runtime_tests tests/runtime_tests.cpp)
        target_link_libraries(amigo_runtime_tests PRIVATE amigo_core)
        add_test(NAME amigo_runtime_tests COMMAND amigo_runtime_tests)
        add_executable(amigo_rhythm_tests tests/rhythm_tests.cpp)
        target_link_libraries(amigo_rhythm_tests PRIVATE amigo_core)
        add_test(NAME amigo_rhythm_tests COMMAND amigo_rhythm_tests)
    endif()
endif()
