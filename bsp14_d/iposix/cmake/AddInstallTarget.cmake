function(Add_Install_Target Target Dependency_Target Install_Component)
	add_custom_target(${Target}
		COMMAND ${CMAKE_COMMAND} -DCOMPONENT=${Install_Component} -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
		VERBATIM
	)
	add_dependencies(${Target} ${Dependency_Target})
endfunction()
