project(list_tests)

set(LIST_TESTS_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/list_tests.c
   )

add_executable(${PROJECT_NAME} ${LIST_TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} list_lib)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../ctest/inc)
