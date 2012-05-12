#ifndef KERNEL_MODIFICATION_SOURCE_H
#define KERNEL_MODIFICATION_SOURCE_H

#include "common_declare.h"
#include "plugin/pm.h"
#include "plugin/fsnotify.h"
#include <pal/libop.h>
#include <vector>

class EXPORT_TO_DLL fswatcher
	:public plugin_manager_base
{
public:
	fswatcher(Database* db);
	~fswatcher();

	bool add_watch(const unistr&, bool recursive = true);
	bool remove_watch(const unistr&); // note: recursive watch is a single watch
	void redirect(snapshotter* );
private:
	snapshotter* snapshotter_;

	static plugin_type_enum conv_plugin_type(const unichar*); // move to base class
	std::vector<plugin_t> plugin_slots;

	static int plugin_fb_add(void*, const unichar*); // that's it!
	static int plugin_fb_remove(void*, const unichar*);
	static int plugin_fb_modify(void*, const unichar*);
	static int plugin_fb_rename(void*, const unichar*, const unichar*);

private:
	// used to manage temporary watching directory, should move to working queue...
	void rm_complete(const unistr& path, int flag = 0);
	void mv_complete(const unistr& path, const unistr& pathnew); 
	void cp_complete(const unistr& path, const unistr& pathnew);
};

#endif
