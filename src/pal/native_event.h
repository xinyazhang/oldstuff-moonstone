#ifndef NATIVE_EVENT_H
#define NATIVE_EVENT_H

/*
 * Triggered/Nontriggered Event object
 * 	Must can be wait/select/etc.
 *
 * For win32, it is Event
 * For linux, it is event_fd
 */

#include "stdtype.h"

EXPORT_TO_DLL native_fd create_event(bool initvalue); // for platform uniform, must be AUTO UNFLAG
EXPORT_TO_DLL void close_event(native_fd);
EXPORT_TO_DLL void wait_event(native_fd);
EXPORT_TO_DLL void flag_event(native_fd);

/* NO WAIT */

#endif
