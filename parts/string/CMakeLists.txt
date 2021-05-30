enable_language(CXX)

CreateLibrary(
	NAME StringConversion
	IS_INTERFACE
	PUBLIC_HEADERS
		conversion_constants.hpp
		from_string_conversion.hpp
		to_string_conversion.hpp
	)

add_executable(test_string_conversion
	test_string_conversion.cpp
	)
target_link_libraries(test_string_conversion PRIVATE doctest::doctest)
target_link_libraries(test_string_conversion PRIVATE ${PROJECT_NAME}::StringConversion)
add_test(StringConversion test_string_conversion)

set_target_properties(test_string_conversion PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
add_custom_command(TARGET test_string_conversion
				   PRE_BUILD COMMAND clang-format --style=file -i "${CMAKE_CURRENT_SOURCE_DIR}/**.*pp"
				   )
				   
add_custom_command(TARGET test_string_conversion
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure
                   )
