
CreateLibrary(
	NAME Parameters
	SOURCES
		parameters.cpp
	DEPENDS
		Microsoft.GSL::GSL
		"${PROJECT_NAME}::StringConversion"
		"${PROJECT_NAME}::General"
	PUBLIC_HEADER
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
