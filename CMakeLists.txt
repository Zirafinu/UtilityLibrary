cmake_minimum_required (VERSION 3.12)
cmake_policy(SET CMP0076 NEW)

project (UtilityLibrary VERSION 0.0.0 LANGUAGES CXX)
include( CTest )
#set( CTEST_USE_LAUNCHERS YES )
enable_testing()

if(NOT APPLE)
	set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

enable_language( CXX )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

set( CMAKE_CXX_VISIBILITY_PRESET hidden )
set( CMAKE_VISIBILITY_INLINES_HIDDEN YES )
 
#include( GenerateExportHeader )
#generate_export_header( ${PROJECT_NAME} )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

#add_library(${PROJECT_NAME})
#add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
#set_target_properties(${PROJECT_NAME} PROPERTIES
#	SOVERSION ${PROJECT_VERSION_MAJOR}
#	VERSION ${PROJECT_VERSION}
#	)

#target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
#target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
#target_compile_options(${PROJECT_NAME} PRIVATE -Wextra)

find_package(doctest REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/remoteProjects/install/")
find_package(Microsoft.GSL REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/remoteProjects/install/")

#add_subdirectory("remoteProjects")
add_subdirectory("parts")

add_library(UtilityLibrary INTERFACE)
target_link_libraries(UtilityLibrary INTERFACE crc)

#target_link_libraries(${PROJECT_NAME} PUBLIC Microsoft.GSL::GSL)

#--------------
# Installer Stuff
# Use GNUInstallDirs to provide the right locations on all platforms
#include(GNUInstallDirs)
#install(TARGETS ${PROJECT_NAME}
#	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#		COMPONENT ${PROJECT_NAME}_Runtime
#	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#		COMPONENT ${PROJECT_NAME}_Runtime
#		NAMELINK_COMPONENT ${PROJECT_NAME}_Development
#	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#		COMPONENT ${PROJECT_NAME}_Development
#	)

#set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
#set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
#set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
#include(CPack)
# not yet intending to package this as anything 

# -Generate Version File
#include( CMakePackageConfigHelpers )
#write_basic_package_version_file(
#	${PROJECT_NAME}ConfigVersion.cmake
#	VERSION ${PROJECT_VERSION}
#	COMPATIBILITY SameMajorVersion
#	)
