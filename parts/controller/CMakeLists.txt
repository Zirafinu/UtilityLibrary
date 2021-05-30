enable_language(CXX)

CreateLibrary(
	NAME CONTROLLER
	IS_INTERFACE
	SRCS
		parameters.cpp
	DEPENDENCIES
		"${PROJECT_NAME}::GENERAL"
	PUBLIC_HEADERS
		pid.hpp
	)

add_executable(TEST_PID test_pid.cpp)
target_link_libraries(TEST_PID PRIVATE doctest::doctest)
target_link_libraries(TEST_PID PRIVATE ${PROJECT_NAME}::CONTROLLER)
add_test(PID TEST_PID)

set_target_properties(TEST_PID PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
add_custom_command(TARGET TEST_PID
				   PRE_BUILD COMMAND clang-format --style=file -i "${CMAKE_CURRENT_SOURCE_DIR}/**.*pp"
				   )

add_custom_command(TARGET TEST_PID
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure
                   )
