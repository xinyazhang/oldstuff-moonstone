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

	void redirect_fsodb(FsodbMan* fsodbman);

	enum ERROR
	{
		NOERR = 0,
		NOTEXIST = -1,
		NOTEMPTY = -2,
		MODLOADERR = -3,
		PLUGIN_NOINITPOINT= -4,
		INVALIDSLOT = -5,
		SLOTPLUGGED = -6
	};

	bool add(const unistr& path, int flag = 0);
	bool undoadd(const unistr& path, int flag = 0);
	bool rm(const unistr& path, int flag = 0);
	bool mv(const unistr& path, const unistr& pathnew);
	bool cp(const unistr& path, const unistr& pathnew);

	bool load_plugin(const unistr&);
	void unload_plugin(libmodule);

	ERROR eno() const;
public:
	enum FsoFlagShift
	{
		recursive_shift = 1
	};
	static const int recursive_flag = 1 << recursive_shift;

private:
	Database* db_;
	FsodbMan* fsodb_;
	ERROR err_;

	//bool add_fso(const unistr& path, idx_t parentid);
	bool add_recursive(const unistr& path, idx_t rootfso);

	struct plugin_t
	{
		libmodule module;
		struct modinoti context;
	};

	enum plugin_type_enum
	{
		LOCAL = 0,
		FTP = 1,
		LAST = 2
	};

	static plugin_type_enum conv_plugin_type(const unichar*);
	std::vector<plugin_t> plugin_slots;

	static int plugin_fb_add(void*, const unichar*);
	static int plugin_fb_remove(void*, const unichar*);
	static int plugin_fb_modify(void*, const unichar*);
	static int plugin_fb_rename(void*, const unichar*, const unichar*);

private:
	void rm_complete(const unistr& path, int flag = 0);
	void mv_complete(const unistr& path, const unistr& pathnew);
	void cp_complete(const unistr& path, const unistr& pathnew);
};

#endif
