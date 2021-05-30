enable_language(CXX)

CreateLibrary(
	NAME CRC
	IS_INTERFACE
	SRCS 
		parameters.cpp
	DEPENDENCIES
		"${PROJECT_NAME}::GENERAL"
	PUBLIC_HEADERS
		crc.hpp
	)

add_library(crc INTERFACE)
target_include_directories(crc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(crc INTERFACE ${PROJECT_NAME}::GENERAL)
add_library(${PROJECT_NAME}::CRC ALIAS crc)

set_target_properties(crc PROPERTIES
	PUBLIC_HEADER
		crc.hpp
	)

install(TARGETS crc
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/CRC
	)

add_executable(test_crc test_crc.cpp crc.hpp)
target_link_libraries(test_crc PRIVATE doctest::doctest)
target_link_libraries(test_crc PRIVATE ${PROJECT_NAME}::CRC)
add_test(crc test_crc)

set_target_properties(test_crc PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
add_custom_command(TARGET test_crc
                   PRE_BUILD
                   COMMAND clang-format --style=file -i "${CMAKE_CURRENT_SOURCE_DIR}/**.*pp"
                   )

add_custom_command(TARGET test_crc
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure
                   )
                   
