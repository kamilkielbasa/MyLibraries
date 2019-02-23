project(rbt)

set(RBT_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/inc/rbt.h
   )

set(RBT_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/rbt.c
   )

add_library(${PROJECT_NAME}_lib
	        ${RBT_HEADER_FILES}
	        ${RBT_SOURCE_FILES}
	       )
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../../common/inc)

add_subdirectory(tests)
