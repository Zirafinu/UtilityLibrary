enable_language(CXX)

CreateLibrary(
	NAME CRC
	IS_INTERFACE
	SOURCES 
		parameters.cpp
	DEPENDS
		"${PROJECT_NAME}::General"
	PUBLIC_HEADER
		crc.hpp
	)

CreateTest( NAME Crc_Test
	SOURCES
		test_crc.cpp
	DEPENDS
		doctest::doctest
		${PROJECT_NAME}::CRC
	)                   
