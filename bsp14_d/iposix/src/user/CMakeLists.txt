include(iposix_userland)

file(GLOB DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/bin "bin/*")

foreach(DIR ${DIRS})
	add_executable(userland_bin_${DIR} bin/${DIR}/main.cpp ${IPOSIX_SYSCALL_CXX_SOURCES} ${IPOSIX_UTIL_CXX_SOURCES})
	set_property(TARGET userland_bin_${DIR} PROPERTY COMPILE_FLAGS "${IPOSIX_USERLAND_COMPILE_FLAGS}")
	set_property(TARGET userland_bin_${DIR} PROPERTY LINK_FLAGS "${IPOSIX_USERLAND_LINK_FLAGS}")
	set_property(TARGET userland_bin_${DIR} PROPERTY OUTPUT_NAME ${DIR})
	install(TARGETS userland_bin_${DIR} DESTINATION bin COMPONENT userland)
	add_dependencies(userland_bin_${DIR} toolchain)
	add_dependencies(userland userland_bin_${DIR})
endforeach()
