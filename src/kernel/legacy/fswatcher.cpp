#include "common.h"
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFileInfoList>

ModSource::ModSource(Database* db)
	:db_(db), fsodb_(db->fsoman()), loadedlib_(LAST)
{
#if 0
	for(int i = 0; i < LAST; i++)
		plugin_slots[i].module = pal_invalidmodule();
#endif
}

bool ModSource::load_plugin(const unistr& lib)
{
	libmodule module = pal_loadlib(lib);
	if( !pal_module_test(module) )
	{
		err_ = MODLOADERR;
		return false;
	}

	fp_modinoti_init probe_func = pal_getfp(module, "probe");
	fp_modinoti_init init_func = pal_getfp(module, "init");
	if( !pal_module_test_fp(probe_func) || !pal_module_test_fp(init_func) )
	{
		err_ = PLUGIN_NOINITPOINT;
		return false;
	}

	struct plugin_t plugin;
	probe_func(&plugin.context);
	plugin_type_enum slot = conv_plugin_type(plugin.context.plugin_type);
	if ( slot == LAST )
	{
		err_ = INVALIDSLOT;
		return false;
	} else if ( pal_module_test(plugin_slots[slot].module) )
	{
		err_ = SLOTPLUGGED;
		return false;
	}
	/*
	 * Check passed, we can start the plugin now
	 */
	plugin.context.sink_cookie = this;
	plugin.context.fp_add_file = plugin_fb_add;
	plugin.context.fp_remove = plugin_fb_remove;
	plugin.context.fp_modify = plugin_fb_modify;
	plugin.context.fp_rename = plugin_fb_rename;

	init_func(&plugin.context);
	/*
	 * register.
	 * however it's too complex, original design is right but I'd forgot it...
	 */
	plugin_slots[slot] = plugin;

	return true;
}

ModSource::~ModSource()
{
	for(std::vector<plugin_t>::iterator iter = plugin_slots.begin();
			iter != plugin_slots.end();
			iter++)
	{
		iter->context.fp_free(iter->cookie);
		pal_unloadlib(iter->module);
	}
}

ModSource::plugin_type_enum ModSource::conv_plugin_type(const unichar* character)
{
	if ( character == unistr("local") )
		return LOCAL;
	else if ( character == unistr("ftp") )
		return FTP;
	else
		return LAST;
}

int ModSource::plugin_fb_add(void* cookie, const unichar* path)
{
	(ModSource*)cookie->add(path);
}

int ModSource::plugin_fb_remove(void* cookie, const unichar* path)
{
	(ModSource*)cookie->undoadd(path);
}

int ModSource::plugin_fb_modify(void* cookie, const unichar* path)
{
	(ModSource*)cookie->add(path);
}

int ModSource::plugin_fb_rename(void* cookie, const unichar* path, const unichar* pathnew)
{
	/*
	 * Needto Improvement as current solution would erase checksum
	 */
	(ModSource*)cookie->undoadd(path);
	(ModSource*)cookie->add(pathnew);
}

void ModSource::rm_complete(const unistr& path, int flag)
{
	remove_watch(path);
}

void ModSource::mv_complete(const unistr& path, const unistr& pathnew)
{
	remove_watch(path);
	remove_watch(pathnew);
}

void ModSource::cp(const unistr& path, const unistr& pathnew)
{
	remove_watch(path);
	remove_watch(pathnew);
}
