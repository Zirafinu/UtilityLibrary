
if(((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) OR TEST_UTILITY_LIB))
	include(CTest)
endif()

function(run_test executable)
    set(_test_state "${PROJECT_BINARY_DIR}/run_tests/${executable}.passed")
    add_custom_command(
        OUTPUT ${_test_state}
	    COMMAND $<TARGET_FILE:${executable}>
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/run_tests"
        COMMAND ${CMAKE_COMMAND} -E touch ${_test_state}
        DEPENDS ${executable}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        )
    add_custom_target(
        ${executable}_passed
        ALL
        DEPENDS ${_test_state}
        )
endfunction()

function( CreateTest )
    cmake_parse_arguments(
        TEST # prefix of output variables
        ""
        "NAME" # list of names of mono-valued arguments
        "SOURCES;DEPENDS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    	)
    if(((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) OR TEST_UTILITY_LIB) AND BUILD_TESTING)
		add_executable(${TEST_NAME} ${TEST_SOURCES})
		set_target_properties(${TEST_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
		
		foreach(dep ${TEST_DEPENDS})
			target_link_libraries(${TEST_NAME} PRIVATE ${dep})
		endforeach(dep ${TST_DEPENDS})
		
		add_test( ${TEST_NAME} ${TEST_NAME})
		run_test( ${TEST_NAME} )
	endif()
endfunction( CreateTest )
