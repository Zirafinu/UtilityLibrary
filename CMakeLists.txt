cmake_minimum_required (VERSION 3.12)
cmake_policy(SET CMP0076 NEW)

project (UtilityLibrary VERSION 0.0.0 LANGUAGES CXX)

include( cmake/library_macros.cmake )
include( cmake/test_macros.cmake )

include(GNUInstallDirs)

if(NOT APPLE)
	set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

add_subdirectory("remoteProjects")

enable_language(CXX)
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

set( CMAKE_CXX_VISIBILITY_PRESET hidden )
set( CMAKE_VISIBILITY_INLINES_HIDDEN YES )

include(CTest)
#set(CLANG_TIDY_STRING "clang-tidy;--header-filter=.*\\.hpp;--config=;--fix")
add_subdirectory("parts")
