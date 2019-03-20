project(llist)

set(LLIST_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/inc/llist.h
   )

set(LLIST_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/llist.c
   )

add_library(${PROJECT_NAME}_lib
	    ${LLIST_HEADER_FILES}
	    ${LLIST_SOURCE_FILES}
	   )
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../../common/inc)

add_subdirectory(tests)
