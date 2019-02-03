project(list1d)

set(LIST1D_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/inc/list1d.h
   )

set(LIST1D_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/list1d.c
   )

add_library(${PROJECT_NAME}_lib
	    ${LIST1D_HEADER_FILES}
	    ${LIST1D_SOURCE_FILES}
	   )
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../../common/inc)

add_subdirectory(tests)
