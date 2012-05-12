#ifndef KERNEL_PLUGIN_MANAGER_H
#define KERNEL_PLUGIN_MANAGER_H

#include <pal/libop.h>
#include <pal/UniStr.h>
#include <vector>

struct plugin_base;

class plugin_manager_base
{
public:
	plugin_manager_base(Database* );
	~plugin_manager_base();

#if 0 // Proper error code should be defined
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
#endif

	const plugin_base* plugins() const;

	virtual unistr type() const = 0;
	ERROR eno() const;
private:
	Database* db_;
	ERROR err_;

	std::vector<plugin_base*> known_plugins;

#if 0 // we need better stragetry
	enum plugin_type_enum // move to plugin/plugin_type
	{
		LOCAL = 0,
		FTP = 1,
		LAST = 2
	};
#endif
};

#endif
