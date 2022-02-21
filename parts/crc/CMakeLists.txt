enable_language(CXX)

CreateLibrary(
	NAME CRC
	IS_INTERFACE
	SRCS 
		parameters.cpp
	DEPENDENCIES
		"${PROJECT_NAME}::General"
	PUBLIC_HEADERS
		crc.hpp
	)

CreateTest( NAME Crc_Test
	SOURCES
		test_crc.cpp
	DEPENDS
		doctest::doctest
		${PROJECT_NAME}::CRC
	)                   
