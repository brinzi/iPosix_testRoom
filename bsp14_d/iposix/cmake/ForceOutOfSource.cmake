if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	message(FATAL_ERROR "This project requires an out-of-source build!\nYou need to remove `CMakeCache.txt' and `CMakeFiles' from the source directory!")
endif()

file(RELATIVE_PATH RPATH ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
string(SUBSTRING ${RPATH} 0 2 RPATH)
if(NOT ${RPATH} STREQUAL "..")
	message(FATAL_ERROR "This project requires an out-of-source build!")
endif()
