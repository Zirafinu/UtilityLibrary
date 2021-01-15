enable_language(CXX)

add_library(configuration
		parameters.cpp 
		parameters.hpp
		parameters_helper.hpp
	)
target_include_directories(configuration PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(configuration PUBLIC Microsoft.GSL::GSL)
target_link_libraries(configuration PUBLIC ${PROJECT_NAME}::STRING_CONVERSION)
target_link_libraries(configuration PUBLIC ${PROJECT_NAME}::GENERAL)
add_library(${PROJECT_NAME}::CONFIGURATION ALIAS configuration)

add_executable(test_parameters test_parameters.cpp test_converters.cpp)
target_link_libraries(test_parameters PRIVATE doctest::doctest)
target_link_libraries(test_parameters PRIVATE ${PROJECT_NAME}::CONFIGURATION)
add_test(parameters test_parameters)

set_target_properties(test_parameters PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
	
add_custom_command(TARGET test_parameters
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure
                   )
