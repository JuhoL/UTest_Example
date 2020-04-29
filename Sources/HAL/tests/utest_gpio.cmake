add_executable(run_utest_gpio
               ${CMAKE_CURRENT_LIST_DIR}/utest_gpio.cpp
               ${CMAKE_CURRENT_LIST_DIR}/../../../TestingUtils/Helpers/utest_helpers.cpp
               ${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/mocks/stm32f429xx_mock.c
               ${CMAKE_CURRENT_LIST_DIR}/../sources/gpio.c)

target_include_directories(run_utest_gpio PUBLIC
                           "${CMAKE_CURRENT_LIST_DIR}/../../../TestingUtils/Catch2"
                           "${CMAKE_CURRENT_LIST_DIR}/../../../TestingUtils/FFF"
                           "${CMAKE_CURRENT_LIST_DIR}/../../../TestingUtils/Helpers"
                           "${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/include"
                           "${CMAKE_CURRENT_LIST_DIR}/../../CMSIS/mocks"
                           "${CMAKE_CURRENT_LIST_DIR}/../../System/include"
                           "${CMAKE_CURRENT_LIST_DIR}/../../System/mocks"
                           "${CMAKE_CURRENT_LIST_DIR}/../../Utils/include"
                           "${CMAKE_CURRENT_LIST_DIR}/../mocks"
                           "${CMAKE_CURRENT_LIST_DIR}/../include")

catch_discover_tests(run_utest_gpio)
