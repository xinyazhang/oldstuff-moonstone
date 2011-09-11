cmake_minimum_required(VERSION 2.8)
PROJECT( lain )

# FORCE Unicode Support
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

INCLUDE_DIRECTORIES(d:/usr/local/include)
INCLUDE_DIRECTORIES(.)
IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
	LINK_DIRECTORIES(d:/usr/lib)
ELSE()
	LINK_DIRECTORIES(d:/usr/lib64)
ENDIF()

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "d:/usr/local/include")


ADD_SUBDIRECTORY( pal )
ADD_SUBDIRECTORY( dal )
ADD_SUBDIRECTORY( kernel )
ADD_SUBDIRECTORY( monitor )
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
