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

add_executable(test_crc test_crc.cpp crc.hpp)
target_link_libraries(test_crc PRIVATE doctest::doctest)
target_link_libraries(test_crc PRIVATE ${PROJECT_NAME}::CRC)
add_test(Crc test_crc)

set_target_properties(test_crc PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
add_custom_command(TARGET test_crc
                   PRE_BUILD
                   COMMAND clang-format --style=file -i "${CMAKE_CURRENT_SOURCE_DIR}/**.*pp"
                   )

add_custom_command(TARGET test_crc
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure
                   )
                   
