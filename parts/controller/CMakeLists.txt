enable_language(CXX)

CreateLibrary(
	NAME Controller
	IS_INTERFACE
	SRCS
		parameters.cpp
	DEPENDENCIES
		"${PROJECT_NAME}::General"
	PUBLIC_HEADERS
		pid.hpp
	)

CreateTest( NAME PID_Controler_Test
	SOURCES
		test_pid.cpp
	DEPENDS
		doctest::doctest
		${PROJECT_NAME}::Controller
	)
	