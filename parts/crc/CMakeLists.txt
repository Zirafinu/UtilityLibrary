enable_language(CXX)

add_library(crc INTERFACE)
target_include_directories(crc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(${PROJECT_NAME}::CRC ALIAS crc)

add_executable(test_crc test_crc.cpp crc.hpp)
target_link_libraries(test_crc PRIVATE doctest::doctest)
target_link_libraries(test_crc PRIVATE ${PROJECT_NAME}::CRC)
add_test(crc test_crc)
#set_target_properties(test_crc PROPERTIES CXX_CLANG_TIDY "clang-tidy;--line-filter=[{\"name\":\"doctest.h\"}];-config=")
	
add_custom_command(TARGET test_crc
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure
                   )
