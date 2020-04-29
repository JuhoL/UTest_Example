cmake_minimum_required(VERSION 3.10)

project(UTest_Example)

enable_testing()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -DUNIT_TEST")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DUNIT_TEST")

include(CTest)
include(Catch.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/../Sources/Supervisor/tests/utest_supervisor.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../Sources/HAL/tests/utest_gpio.cmake)
