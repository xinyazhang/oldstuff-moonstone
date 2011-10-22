#include "pm.h"

plugin_manager_base::plugin_manager_base(Database* db)
	:db_(db)
{
	// code for know registered plugin
}

plugin_manager_base::~plugin_manager_base()
{
	for(std::vector<plugin_base*>::iterator iter = known_plugins.begin();
			iter != known_plugins.end();
			iter++)
	{
		plugin_base* plugin = *iter;
		if ( plugin->loaded() )
			plugin->unload_module();
	}
}
