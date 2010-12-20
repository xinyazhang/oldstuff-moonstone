cmake_minimum_required(VERSION 2.8)
PROJECT( lain )

INCLUDE_DIRECTORIES(d:/usr/include)
INCLUDE_DIRECTORIES(.)
IF( CMAKE_SIZEOF_VOID_P EQUAL 4 )
	LINK_DIRECTORIES(d:/usr/lib)
ELSE()
	LINK_DIRECTORIES(d:/usr/lib64)
ENDIF()


ADD_SUBDIRECTORY( pal )
ADD_SUBDIRECTORY( dal )
ADD_SUBDIRECTORY( kernel )
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
