enable_language(CXX)

add_library(stabilizer INTERFACE)
target_include_directories(stabilizer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(${PROJECT_NAME}::STABILIZER ALIAS stabilizer)

add_executable(test_pid test_pid.cpp pid.hpp)
target_link_libraries(test_pid PRIVATE doctest::doctest)
target_link_libraries(test_pid PRIVATE ${PROJECT_NAME}::STABILIZER)
add_test(pid test_pid)

set_target_properties(test_pid PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")

add_custom_command(TARGET test_pid
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure
                   )
