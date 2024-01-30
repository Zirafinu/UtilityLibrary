
CreateLibrary(
	NAME StringConversion
	IS_INTERFACE
	DEPENDS
		Microsoft.GSL::GSL
	PUBLIC_HEADER
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
