project(stack_tests)

set(STACK_TESTS_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/stack_tests.c
   )

add_executable(${PROJECT_NAME} ${STACK_TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} stack_lib)
target_include_directories(${PROJECT_NAME} PUBLIC ../../darray/inc)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../ctest/inc)
