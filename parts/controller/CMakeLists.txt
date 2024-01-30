
CreateLibrary(
	NAME Controller
	IS_INTERFACE
	SOURCES
		parameters.cpp
	DEPENDS
		"${PROJECT_NAME}::General"
	PUBLIC_HEADER
		pid.hpp
	)

CreateTest( NAME PID_Controler_Test
	SOURCES
		test_pid.cpp
	DEPENDS
		doctest::doctest
		${PROJECT_NAME}::Controller
	)
	