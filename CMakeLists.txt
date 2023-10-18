cmake_minimum_required(VERSION 3.26)
project(CopyOnWritePtr)

set(CMAKE_CXX_STANDARD 17)

add_library(CopyOnWritePtr INTERFACE)
target_include_directories(CopyOnWritePtr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(GTest)
if (GTEST_FOUND)
    enable_testing()
    add_subdirectory(tests)
endif ()
