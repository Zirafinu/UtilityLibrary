enable_language(CXX)

add_library(general INTERFACE)
target_include_directories(general INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(${PROJECT_NAME}::GENERAL ALIAS general)

set_target_properties(test_parameters PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
