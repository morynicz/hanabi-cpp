cmake_minimum_required(VERSION 3.10)
project(AceOfSpace CXX)

set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall -Wextra -pedantic -Werror -Wunused)

find_package(Threads)

# Download and unpack googletest at configure time
configure_file(GTestCMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

set(PYBIND11_CPP_STANDARD -std=c++17)

add_subdirectory(pybind11)

include_directories(include)

add_library(hanabi src/Game.cpp src/Hand.cpp src/Table.cpp src/Deck.cpp)

add_executable(test_game src/GameTest.cpp src/TableTest.cpp src/DeckTest.cpp)
set_property(TARGET test_game PROPERTY CXX_STANDARD 17)

target_link_libraries(test_game hanabi gmock_main gtest gmock ${CMAKE_THREAD_LIBS_INIT})

pybind11_add_module(hanabi_py SHARED src/PyGame.cpp src/Game.cpp src/Hand.cpp src/Table.cpp src/Deck.cpp)
set_property(TARGET hanabi PROPERTY CXX_STANDARD 17)
enable_testing()

add_test(game test_game)

configure_file(binding_test.py binding_test.py)