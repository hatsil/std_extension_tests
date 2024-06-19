cmake_minimum_required(VERSION 3.22)
project(UnitTests CXX)

set(TARGET "unit-tests")

# Add debugging information
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# Include the library
include_directories(
  include
)

include(cmake/gtest.cmake)

add_subdirectory(submodules/std_extention)

add_definitions(-DALL_TESTS)

file(GLOB SOURCES "src/*.cpp")

add_executable(${TARGET} ${SOURCES})

target_link_libraries(
  ${TARGET} PRIVATE
  gtest
  gtest_main
  std_extention
)
