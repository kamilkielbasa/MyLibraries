cmake_minimum_required(VERSION 3.12)

add_compile_options("-std=gnu99" "-g" "-O3" "-Werror" "-Wall" "-Wextra" "-pedantic"
		    "-Wcast-align" "-Winit-self" "-Wlogical-op" "-Wmissing-include-dirs" 
		    "-Wredundant-decls" "-Wshadow" "-Wstrict-overflow=5" "-Wundef"  
		    "-Wwrite-strings" "-Wpointer-arith" "-Wmissing-declarations"
		    "-Wuninitialized" "-Wold-style-definition" "-Wstrict-prototypes" 
		    "-Wmissing-prototypes" "-Wswitch-default" "-Wbad-function-cast" 
		    "-Wnested-externs" "-Wconversion" "-Wunreachable-code")

project(MyLibraries)
add_subdirectory(common)
add_subdirectory(ctest)
add_subdirectory(containers)
