SET(UNIX_SRC pathc_unix.c)
SET(WIN32_SRC pathc_win.c pathxx_win.cpp ipc_win.cc volume_win32.cc native_event_win32.cc)
SET(COMMON_SRC pathxx.cpp unistr.cpp ipc.cc)

IF(UNIX)
	SET(PAL_SRC ${COMMON_SRC} ${UNIX_SRC})
ENDIF(UNIX)

IF(WIN32)
	SET(PAL_SRC ${COMMON_SRC} ${WIN32_SRC})
ENDIF(WIN32)

ADD_LIBRARY(pal SHARED ${PAL_SRC})
TARGET_LINK_LIBRARIES(pal QtCore4)
