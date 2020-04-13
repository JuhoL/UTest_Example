cmake_minimum_required(VERSION 3.10)

project(UTest_Example)

enable_testing()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include(CTest)
include(Catch.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/../Sources/Supervisor/tests/utest_supervisor.cmake)
