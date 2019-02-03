project(dll_tests)

set(DLL_TESTS_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/dll_tests.c
   )

add_executable(${PROJECT_NAME} ${DLL_TESTS_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} dll_lib)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../ctest/inc)
