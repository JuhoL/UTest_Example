add_executable(run_utest_supervisor
               ${CMAKE_CURRENT_LIST_DIR}/utest_supervisor.cpp
               ${CMAKE_CURRENT_LIST_DIR}/../../../TestingUtils/Helpers/utest_helpers.cpp
               ${CMAKE_CURRENT_LIST_DIR}/../sources/supervisor.c)

target_include_directories(run_utest_supervisor PUBLIC
                           "${CMAKE_CURRENT_LIST_DIR}/../../../TestingUtils/Catch2"
                           "${CMAKE_CURRENT_LIST_DIR}/../../../TestingUtils/FFF"
                           "${CMAKE_CURRENT_LIST_DIR}/../../../TestingUtils/Helpers"
                           "${CMAKE_CURRENT_LIST_DIR}/../../HAL/include"
                           "${CMAKE_CURRENT_LIST_DIR}/../../HAL/mocks"
                           "${CMAKE_CURRENT_LIST_DIR}/../../System/include"
                           "${CMAKE_CURRENT_LIST_DIR}/../../System/mocks"
                           "${CMAKE_CURRENT_LIST_DIR}/../../Utils/include"
                           "${CMAKE_CURRENT_LIST_DIR}/../include")

catch_discover_tests(run_utest_supervisor)
