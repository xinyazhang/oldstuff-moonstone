#include "../pal/stdtype.h"
#include "version.h"
#include "serialization.h"

// Using "" for headers only including functions, or cross-platform data structs
#include "daemon.h"
#include "special_path.h"
#include "daemon_error.h"
#include "ipc.h"

using boost::serialization::make_nvp;

filestream conf;
filestream open_conf();

int daemon_init()
{
	conf = open_conf();

	if (!conf.is_open())
	{
		daemon_report_error(DERR_CANNOT_OPEN_CONF_FILE); // May or may not exist
		return DERR_CANNOT_OPEN_CONF_FILE;
	}

	/* MILESTONE 1: Write "OK!" to configure file */
#if MILESTONE == 1
	conf << UT("OK!") << std::endl;
	conf.close();
	return 0;
#endif

#if MILESTONE >= 2
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
#endif

#if MILESTONE >= 3
	start_tracing();
#endif

#if MILESTONE >= 4
	open_ipc();
#endif
}

void daemon_release()
{
	/* DON'T CLOSE FILE CURRENTLY, FOR SYNC ISSUES */
#if 0
	conf->close();
	delete conf;
	conf = NULL;
#endif
#if MILESTONE >= 3
	stop_tracing();
#endif 
#if MILESTONE >= 4
	close_ipc();
#endif
#if MILESTONE >= 2
	conf.seekg(0, std::ios::beg);
	barc_o ar(conf);
	ar << BOOST_SERIALIZATION_NVP(known_partitions);
	ar << BOOST_SERIALIZATION_NVP(tracing_paths);
	conf.close();
#endif
}

filestream open_conf()
{
	unistr path = locate_conf_dir();
	path += "/records.conf";

	filestream fs;
	fs.open(path.native(), std::ios_base::in|std::ios_base::out);
	if (!fs.is_open())
		fs.open(path.native(), std::ios_base::in|std::ios_base::out|std::ios_base::trunc);

	return fs;
}
