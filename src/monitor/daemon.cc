#include "../pal/stdtype.h"
#include "serialization.h"

// Using "" for headers only including functions, or cross-platform data structs
#include "daemon.h"
#include "special_path.h"
#include "daemon_error.h"
#include <ipc.h>

using boost::serialization::make_nvp;

filestream conf;
partition_list known_partitions;
partition_list online_partitions;
tracing_path_list tracing_paths;
tracing_object tracings;

filestream open_conf();

void daemon_init()
{
	conf = open_conf();

	if (!conf.is_open())
	{
		daemon_report_error(DERR_CANNOT_OPEN_CONF_FILE); // May or may not exit
		return ;
	}

	conf.seekg(0, std::ios::end);
	if (conf.tellg() == 0)
	{
		partition_blank();
		tracing_blank();
	} else
	{
		conf.seekg(0, std::ios::beg);

		/* load partition list */
		barc_i ar(conf);
		ar >> BOOST_SERIALIZATION_NVP(known_partitions);
		ar >> BOOST_SERIALIZATION_NVP(tracing_paths);
	}
	scan_online_partitons();
	start_tracing();
	start_ipc();
}

void daemon_release()
{
	/* DON'T CLOSE FILE CURRENTLY, FOR SYNC ISSUES */
#if 0
	conf->close();
	delete conf;
	conf = NULL;
#endif
	conf.seekg(0, std::ios::beg);
	barc_o ar(conf);
	ar << BOOST_SERIALIZATION_NVP(known_partitions);
	ar << BOOST_SERIALIZATION_NVP(tracing_paths);
}

filestream open_conf()
{
	unistr path = locate_conf_dir();
	path += "/records.conf";

	return filestream(path.native());
}