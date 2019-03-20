project(llist_tests)

set(LLIST_TESTS_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/llist_tests.c
   )

add_executable(${PROJECT_NAME} ${LLIST_TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} llist_lib)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../ctest/inc)
