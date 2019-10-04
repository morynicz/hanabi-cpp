cmake_minimum_required(VERSION 3.5)
project(AceOfSpace CXX)

set(CMAKE_BUILD_TYPE Debug)
set_property(GLOBAL PROPERTY CXX_STANDARD 17)

find_package(Boost REQUIRED program_options system)
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



include_directories(include)

add_executable(test_game src/GameTest.cpp)
target_link_libraries(test_game gmock_main gtest gmock ${CMAKE_THREAD_LIBS_INIT})

enable_testing()

add_test(game test_game)