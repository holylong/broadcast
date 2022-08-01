cmake_minimum_required(VERSION 3.2)

project(broadcast)

#add_executable(function_main ${function_main_SRC})
include_directories(${PROJECT_SOURCE_DIR}/../../common)
file(GLOB_RECURSE mains RELATIVE "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/*.c")
foreach(mainfile IN LISTS mains)
    # Get file name without directory
    get_filename_component(mainname ${mainfile} NAME_WE)
    add_executable(${mainname} ${mainfile})
	if(WIN32)
	add_definitions(-D _CRT_SECURE_NO_WARNINGS)
	endif()
	#target_link_libraries(${mainname} HTKLib)
	SET_TARGET_PROPERTIES(${mainname} PROPERTIES FOLDER sockets)
	MESSAGE(status "==>> ${mainname}")
if(MSVC)
  target_link_libraries(${mainname} PRIVATE "ws2_32")
endif()
endforeach()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
