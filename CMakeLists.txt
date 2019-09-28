cmake_minimum_required(VERSION 3.5)
project(AceOfSpace CXX)

find_package(Boost REQUIRED program_options system)
find_package(Threads)
find_package(GTest)

set(CMAKE_BUILD_TYPE Debug)
set_property(GLOBAL PROPERTY CXX_STANDARD 17)

include_directories(include ${GTEST_INCLUDE_DIR})

add_executable(test_game src/GameTest.cpp)
target_link_libraries(test_game ${GTEST_MAIN_LIBRARY} ${GTEST_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

enable_testing()

add_test(game test_game)