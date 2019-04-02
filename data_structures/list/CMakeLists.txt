project(list)

set(LIST_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/inc/list.h
   )

set(LIST_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/list.c
   )

add_library(${PROJECT_NAME}_lib
	    ${LIST_HEADER_FILES}
	    ${LIST_SOURCE_FILES}
	   )
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../../common/inc)

add_subdirectory(tests)
