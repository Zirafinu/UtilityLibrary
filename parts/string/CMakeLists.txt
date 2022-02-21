enable_language(CXX)

CreateLibrary(
	NAME StringConversion
	IS_INTERFACE
	DEPENDENCIES
		Microsoft.GSL::GSL
	PUBLIC_HEADERS
		conversion_constants.hpp
		from_string_conversion.hpp
		to_string_conversion.hpp
	)

CreateTest(
	NAME
		StringConversion_Test
	SOURCES
		test_string_conversion.cpp
	DEPENDS
		doctest::doctest
		${PROJECT_NAME}::StringConversion
	)
