enable_language(CXX)

add_library(general INTERFACE)
target_include_directories(general INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(${PROJECT_NAME}::GENERAL ALIAS general)

# set_target_properties(general PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_STRING}")
# add_custom_command(TARGET general PRE_BUILD COMMAND clang-format --style=file -i "${CMAKE_CURRENT_SOURCE_DIR}/**.*pp")
