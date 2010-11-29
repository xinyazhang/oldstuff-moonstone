#ifndef KERNEL_PLUGIN_INFRASTRUCTURE_H
#define KERNEL_PLUGIN_INFRASTRUCTURE_H

#include <pal/libop.h>
#include <pal/UniStr.h>

struct plugin_base
{
	unistr lib_path;
	libmodule module;

	bool loaded();
	bool load_module();
	bool unload_module();
	void* getfp(libmodule, const unistr&);
};

#endif
