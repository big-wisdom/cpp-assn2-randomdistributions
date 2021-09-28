cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME RandomDistributions)
project(${PROJECT_NAME})

set(SOURCE_FILES Assignment2.cpp)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Export compiler flags for YouCompleteMe

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "CLANG")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()



find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
	unset(SOURCE_FILES_PATHS)
	foreach(SOURCE_FILE ${SOURCE_FILES})
		get_source_file_property(WHERE ${SOURCE_FILE} LOCATION)
		set(SOURCE_FILES_PATHS ${SOURCE_FILES_PATHS} ${WHERE})
	endforeach()

	add_custom_target(
		ClangFormat
		COMMAND ${CLANG_FORMAT}
		-i
		-style=file
		${SOURCE_FILES_PATHS})

	add_dependencies(${PROJECT_NAME} ClangFormat)
endif()
