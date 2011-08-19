#ifndef DAEMON_ERROR_H
#define DAEMON_ERROR_H

enum DEER
{
	DERR_CANNOT_OPEN_CONF_FILE = 1,
	DERR_CANNOT_LOCATE_CONF_FILE,
	DERR_LAST
};

inline void daemon_report_error(int ) { }

#endif
