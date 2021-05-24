enable_language(CXX)

add_library(Configuration
		parameters.cpp
	)
target_include_directories(Configuration PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Configuration PUBLIC Microsoft.GSL::GSL)
target_link_libraries(Configuration PUBLIC ${PROJECT_NAME}::StringConversion)
target_link_libraries(Configuration PUBLIC ${PROJECT_NAME}::GENERAL)
add_library(${PROJECT_NAME}::Configuration ALIAS Configuration)
set_property(TARGET Configuration PROPERTY
	PUBLIC_HEADER
		parameters_converters.hpp
		parameters_helper.hpp
		parameters_table.hpp
		parameters.hpp  
	)

install(TARGETS Configuration
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

add_executable(test_parameters
	test_parameters.cpp
	test_converters.cpp
	test_parameters_table.cpp
	)
target_link_libraries(test_parameters PRIVATE doctest::doctest)
target_link_libraries(test_parameters PRIVATE ${PROJECT_NAME}::Configuration)
add_test(parameters test_parameters)

set_target_properties(test_parameters PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
add_custom_command(TARGET test_parameters
	PRE_BUILD 
	COMMAND clang-format --style=file -i "${CMAKE_CURRENT_SOURCE_DIR}/**.*pp"
	)

add_custom_command(TARGET test_parameters
    POST_BUILD
    COMMAND ctest -C $<CONFIGURATION> --output-on-failure
    )
