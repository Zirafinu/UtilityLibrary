enable_language(CXX)

add_library(controller INTERFACE)
target_include_directories(controller INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(${PROJECT_NAME}::CONTROLLER ALIAS controller)
set_target_properties(controller PROPERTIES
	PUBLIC_HEADER
		pid.hpp
	)

install(TARGETS controller
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

add_executable(test_pid test_pid.cpp pid.hpp)
target_link_libraries(test_pid PRIVATE doctest::doctest)
target_link_libraries(test_pid PRIVATE ${PROJECT_NAME}::CONTROLLER)
add_test(pid test_pid)

set_target_properties(test_pid PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
add_custom_command(TARGET test_pid
				   PRE_BUILD COMMAND clang-format --style=file -i "${CMAKE_CURRENT_SOURCE_DIR}/**.*pp"
				   )

add_custom_command(TARGET test_pid
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure
                   )
