#ifndef KERNEL_MODIFICATION_SOURCE_H
#define KERNEL_MODIFICATION_SOURCE_H

#include "common_declare.h"
#include "plugin/modinoti.h"
#include <pal/libop.h>
#include <vector>

class EXPORT_TO_DLL ModSource
{
	friend class AsyncWQ;
public:
	ModSource(Database* db);
	~ModSource();

	void redirect(Snapshotter* snapshotter);

	enum ERROR // should be inherited
	{
		NOERR = 0,
		NOTEXIST = -1,
		NOTEMPTY = -2,
		MODLOADERR = -3,
		PLUGIN_NOINITPOINT= -4,
		INVALIDSLOT = -5,
		SLOTPLUGGED = -6
	};

	bool load_plugin(const unistr&); // also should be inherited
	void unload_plugin(libmodule);

	ERROR eno() const;
public:
private:
	Database* db_;
	Snapshotter* snapshotter_;
	ERROR err_;

	struct plugin_t // move to plugin/plugin_base
	{
		libmodule module;
		struct modinoti context;
	};

	enum plugin_type_enum // move to plugin/plugin_type
	{
		LOCAL = 0,
		FTP = 1,
		LAST = 2
	};

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
