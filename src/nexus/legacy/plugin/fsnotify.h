#ifndef KERNEL_PLUGIN_MODIFICATION_NOTIFY_H
#define KERNEL_PLUGIN_MODIFICATION_NOTIFY_H

#include <pal/unicode.h>

struct modinoti;

typedef int (*fp_notify_single_parameter)(void* cookie, const unichar*);
typedef int (*fp_notify_double_parameter)(void* cookie, const unichar*, const unichar*);
typedef int (*fp_ctl)(const unichar*, int flag);
typedef int (*fp_modinoti_init)(const struct modinoti*);
typedef int (*fp_modinoti_free)(void* plugin_cookie);

struct modinoti
{
	/* plugin read part */
	void* sink_cookie;
	fp_notify_single_parameter fp_add_file, fp_remove, fp_modify;
	fp_notify_double_parameter fp_rename;
	/* plugin write part */
	void* plugin_cookie;
	fp_modinoti_free fp_free;
	const unichar* plugin_type;
	fp_ctl fp_add_watch, fp_rm_watch;
};


#endif
