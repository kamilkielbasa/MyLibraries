project(darray_tests)

set(DARRAY_TESTS_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/darray_tests.c
   )

add_executable(${PROJECT_NAME} ${DARRAY_TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} darray_lib m)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../ctest/inc)
