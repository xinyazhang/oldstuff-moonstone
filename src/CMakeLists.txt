cmake_minimum_required(VERSION 2.8)
PROJECT( lain )

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
# FORCE Unicode Support
	ADD_DEFINITIONS(-DUNICODE -D_UNICODE -D_SCL_SECURE_NO_WARNINGS)

	INCLUDE_DIRECTORIES(d:/usr/local/include)
	IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
		LINK_DIRECTORIES(d:/usr/lib)
	ELSE()
		LINK_DIRECTORIES(d:/usr/lib64)
	ENDIF()
	SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "d:/usr/local/include")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(.)

ADD_SUBDIRECTORY( pal )
ADD_SUBDIRECTORY( kernel )
ADD_SUBDIRECTORY( moonstone )
ADD_SUBDIRECTORY( everything )
#ADD_SUBDIRECTORY( testcases )

#add_subdirectory( gui )
#add_subdirectory( util )

#SET( gui_src gui/*.cpp )
#file(GLOB gui_src "./gui/*.cpp")
#AUX_SOURCE_DIRECTORY( gui gui_src )
#ADD_EXECUTABLE( lain_gui ${gui_src} )

#SET( kernel_src kernel/*.cpp pal/*.cpp pal/*.c dal/*.cpp dal/*.c )
#ADD_library( kernel SHARED kernel/ pal/ dal/ )

#SET( util_shell_src util/shell/*.cpp )
#add_executable( lain_util_shell $(util_shell_src) )
