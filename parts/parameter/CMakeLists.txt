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

CreateTest(
	NAME Parameters_Test
	SOURCES
		test_parameters.cpp
		test_converters.cpp
		test_parameters_table.cpp
	DEPENDS
		doctest::doctest
		${PROJECT_NAME}::Parameters
	)
