project(rbt_tests)

set(RBT_TESTS_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/rbt_tests.c
   )

add_executable(${PROJECT_NAME} ${RBT_TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} rbt_lib)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../ctest/inc)
