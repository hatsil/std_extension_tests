cmake_minimum_required(VERSION 3.2)
project(UnitTests CXX)

set(TARGET "unit-tests")

# Add debugging information
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# Include the library
include_directories(
  include
)

include(cmake/stdext.cmake)
include(cmake/gtest.cmake)

file(GLOB SOURCES "src/*.cpp")

add_executable(${TARGET} ${SOURCES})

target_link_libraries(
  ${TARGET} PRIVATE
  gtest
  gtest_main
  stdext
)
