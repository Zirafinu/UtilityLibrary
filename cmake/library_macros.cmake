
include(CMakeParseArguments)

function(CreateLibrary)
    cmake_parse_arguments(
        LIB # prefix of output variables
        "IS_INTERFACE" # list of names of the boolean arguments (only defined ones will be true)
        "NAME" # list of names of mono-valued arguments
        "SOURCES;DEPENDS;PUBLIC_HEADER" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    	)

	if(LIB_IS_INTERFACE)
		add_library(${LIB_NAME} INTERFACE)
	else(LIB_IS_INTERFACE)
		add_library(${LIB_NAME} ${LIB_PARAMS} ${LIB_SOURCES})
	endif(LIB_IS_INTERFACE)
	
	target_include_directories(${LIB_NAME} INTERFACE 
    	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    	$<INSTALL_INTERFACE:include/${PROJECT_NAME}/${LIB_NAME}>
    	)
	foreach(dep ${LIB_DEPENDS})
	
		if(LIB_IS_INTERFACE)
			target_link_libraries(${LIB_NAME} INTERFACE ${dep})
		else(LIB_IS_INTERFACE)
			target_link_libraries(${LIB_NAME} PUBLIC ${dep})
		endif(LIB_IS_INTERFACE)
  		
	endforeach(dep ${LIB_DEPENDS})
	
	add_library(${PROJECT_NAME}::${LIB_NAME} ALIAS ${LIB_NAME})
	set_property(TARGET ${LIB_NAME} PROPERTY PUBLIC_HEADER ${LIB_PUBLIC_HEADER})
	
	install(TARGETS ${LIB_NAME}
		EXPORT "${LIB_NAME}Config"
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
		PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${LIB_NAME}
		)
		
	install(EXPORT "${LIB_NAME}Config"
		NAMESPACE ${PROJECT_NAME}:: 
		DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}
		)

	export(TARGETS ${LIB_NAME} NAMESPACE ${PROJECT_NAME}:: FILE ${LIB_NAME}Config.cmake)
	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake
		COMPATIBILITY SameMajorVersion
		ARCH_INDEPENDENT
    	)
endfunction()
