project(dll)

set(DLL_HEADER_FILES
    ${CMAKE_CURRENT_LIST_DIR}/inc/dll.h
   )

set(DLL_SOURCE_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/dll.c
   )

add_library(${PROJECT_NAME}_lib
	        ${DLL_HEADER_FILES}
	        ${DLL_SOURCE_FILES}
	       )
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ../../common/inc)

add_subdirectory(tests)
