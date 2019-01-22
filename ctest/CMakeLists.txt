project(ctest)

set(CTEST_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/inc/ctest.h
    )

set(CTEST_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/ctest.c
    )

add_library(${PROJECT_NAME}_lib
	        ${CTEST_HEADER_FILES}
	        ${CTEST_SOURCE_FILES}
           )
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../common/inc/)

add_executable(${PROJECT_NAME} src/ctest.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

add_subdirectory(tests)
