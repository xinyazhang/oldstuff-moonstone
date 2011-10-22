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

native_fd create_event(bool initvalue); // for platform uniform, must be AUTO UNFLAG
void close_event(native_fd);
void wait_event(native_fd);
void flag_event(native_fd);

/* NO WAIT */

#endif
