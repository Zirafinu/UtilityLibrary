enable_language(CXX)

CreateLibrary(
	NAME Parameters
	SRCS 
		parameters.cpp
	DEPENDENCIES
		Microsoft.GSL::GSL
		"${PROJECT_NAME}::StringConversion"
		"${PROJECT_NAME}::General"
	PUBLIC_HEADERS
		parameters_converters.hpp
		parameters_table.hpp
		parameters.hpp
	)

add_executable(TEST_PARAMETERS
	test_parameters.cpp
	test_converters.cpp
	test_parameters_table.cpp
	)
target_link_libraries(TEST_PARAMETERS PRIVATE doctest::doctest)
target_link_libraries(TEST_PARAMETERS PRIVATE ${PROJECT_NAME}::Parameters)
add_test(Parameters TEST_PARAMETERS)

set_target_properties(TEST_PARAMETERS PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
add_custom_command(TARGET TEST_PARAMETERS
	PRE_BUILD 
	COMMAND clang-format --style=file -i "${CMAKE_CURRENT_SOURCE_DIR}/**.*pp"
	)

add_custom_command(TARGET TEST_PARAMETERS
    POST_BUILD
    COMMAND ctest -C $<CONFIGURATION> --output-on-failure
    )
