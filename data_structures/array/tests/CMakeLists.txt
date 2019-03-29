project(array_tests)

set(ARRAY_TESTS_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/array_tests.c
   )

add_executable(${PROJECT_NAME} ${ARRAY_TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} array_lib)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../ctest/inc)
