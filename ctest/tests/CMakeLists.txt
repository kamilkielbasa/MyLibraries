cmake_minimum_required(VERSION 3.12)

project(ctest_tests)
add_compile_options("-std=gnu99" "-Werror" "-Wall" "-Wextra" "-pedantic")

set(CTEST_TESTS_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/ctest_tests.c
   )

add_executable(${PROJECT_NAME} ${CTEST_TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ctest_lib)
