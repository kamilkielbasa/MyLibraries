project(list1d_tests)

set(LIST1D_TESTS_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/list1d_tests.c
   )

add_executable(${PROJECT_NAME} ${LIST1D_TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} list1d_lib)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../ctest/inc)
