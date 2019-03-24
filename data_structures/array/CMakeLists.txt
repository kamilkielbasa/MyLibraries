project(array)

set(ARRAY_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/inc/array.h
   )

set(ARRAY_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/array.c
   )

add_library(${PROJECT_NAME}_lib
	        ${ARRAY_HEADER_FILES}
	        ${ARRAY_SOURCE_FILES}
	       )
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../../common/inc)

add_subdirectory(tests)
