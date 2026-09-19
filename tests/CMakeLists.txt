cmake_minimum_required(VERSION 3.19)
project(sts_combat_regressions LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Standalone C++ tests: initialize the json submodule; Python is not required.
set(STS_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
file(GLOB_RECURSE STS_SOURCES CONFIGURE_DEPENDS "${STS_ROOT}/src/*.cpp")
add_library(sts_test_core STATIC ${STS_SOURCES})
target_compile_features(sts_test_core PUBLIC cxx_std_17)
target_include_directories(sts_test_core PUBLIC
    "${STS_ROOT}/include" "${STS_ROOT}/json/single_include")

enable_testing()
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*-test.cpp")
foreach(TEST_SOURCE IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME "${TEST_SOURCE}" NAME_WE)
    add_executable(${TEST_NAME} "${TEST_SOURCE}")
    target_link_libraries(${TEST_NAME} PRIVATE sts_test_core)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
