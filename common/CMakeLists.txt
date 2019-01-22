project(common)

set(COMMON_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/inc/common.h
    )

set(COMMON_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/common.c
    )

add_library(${PROJECT_NAME}_lib
	    ${COMMON_HEADER_FILES}
    	    ${COMMON_SOURCE_FILES}
           )
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)

