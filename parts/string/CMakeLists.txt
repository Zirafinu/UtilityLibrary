enable_language(CXX)

add_library(StringConversion INTERFACE)
target_include_directories(StringConversion INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}/StringConversion>
    )
set_property(TARGET StringConversion PROPERTY
	PUBLIC_HEADER
		conversion_constants.hpp
		from_string_conversion.hpp
		to_string_conversion.hpp
	)
add_library(${PROJECT_NAME}::StringConversion ALIAS StringConversion)

install(TARGETS StringConversion
	EXPORT StringConversionConfig
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/StringConversion
	)
install(EXPORT StringConversionConfig
	NAMESPACE ${PROJECT_NAME}:: 
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/StringConversion
	)

export(TARGETS StringConversion NAMESPACE ${PROJECT_NAME}:: FILE StringConversionConfig.cmake)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/StringConversionConfig.cmake
	COMPATIBILITY SameMajorVersion
	ARCH_INDEPENDENT
    )

add_executable(test_string_conversion test_string_conversion.cpp conversion_constants.hpp from_string_conversion.hpp to_string_conversion.hpp)
target_link_libraries(test_string_conversion PRIVATE doctest::doctest)
target_link_libraries(test_string_conversion PRIVATE ${PROJECT_NAME}::StringConversion)
add_test(string_conversion test_string_conversion)

set_target_properties(test_string_conversion PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
add_custom_command(TARGET test_string_conversion
				   PRE_BUILD COMMAND clang-format --style=file -i "${CMAKE_CURRENT_SOURCE_DIR}/**.*pp"
				   )
				   
add_custom_command(TARGET test_string_conversion
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure
                   )
