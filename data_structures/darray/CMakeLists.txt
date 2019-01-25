project(darray)

set(DARRAY_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/inc/darray.h
   )

set(DARRAY_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/darray.c
   )

add_library(${PROJECT_NAME}_lib
	        ${DARRAY_HEADER_FILES}
	        ${DARRAY_SOURCE_FILES}
	       )
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../../common/inc)

add_subdirectory(tests)
