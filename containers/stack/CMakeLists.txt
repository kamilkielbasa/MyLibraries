project(stack)

set(STACK_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/inc/stack.h
   )

set(STACK_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/stack.c
   )

add_library(${PROJECT_NAME}_lib STATIC
	        ${STACK_SOURCE_FILES}
	        ${STACK_HEADER_FILES}
	       )

target_link_libraries(${PROJECT_NAME}_lib darray_lib)
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../darray/inc)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../../common/inc)

add_subdirectory(tests)
