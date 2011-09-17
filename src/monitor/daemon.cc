#include "version.h"
#include "../pal/stdtype.h"
#include "serialization.h"

// Using "" for headers only including functions, or cross-platform data structs
#include "daemon.h"
#include "lsconf.h"
#include "special_path.h"
#include "daemon_error.h"
#include <stdio.h>

using boost::serialization::make_nvp;

filestream conf;
filestream open_conf();
filestream write_conf();

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
	uint64_t fsize = conf.tellg();
	if (0 == fsize)
	{
		/*
		 * Empty configure file, create empty data structure in memory
		 */
		partition_blank();
#if MILESTONE >= 3
		tracing_blank();
		platform_depedent_init();
#endif
	} else
	{
		/*
		 * Load configuration to memory
		 */
		conf.seekg(0, std::ios::beg);

		/* load partition list */
		barc_i ar(conf);
		ar >> BOOST_SERIALIZATION_NVP(kpi_last);
		ar >> BOOST_SERIALIZATION_NVP(known_partitions);
#if MILESTONE >= 3
		ar >> BOOST_SERIALIZATION_NVP(tracing_paths);
		platform_depedent_load(ar);
#endif
	}
	scan_online_partitons();
#endif

#if MILESTONE >= 3
	tracing_start();
#endif

#if MILESTONE >= 4
	open_ipc();
#endif
	conf.close();
	return 0;
}

int daemon_main_loop()
{
	getchar();
	return 0;
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
	tracing_stop();
#endif 
#if MILESTONE >= 4
	close_ipc();
#endif
#if MILESTONE >= 2
	conf = write_conf();
	barc_o ar(conf);
	ar << BOOST_SERIALIZATION_NVP(kpi_last);
	ar << BOOST_SERIALIZATION_NVP(known_partitions);
#if MILESTONE >= 3
	ar << BOOST_SERIALIZATION_NVP(tracing_paths);
	platform_depedent_save(ar);
#endif
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

filestream write_conf()
{
	unistr path = locate_conf_dir();
	path += "/records.conf";

	filestream fs;
	fs.open(path.native(), std::ios_base::in|std::ios_base::out|std::ios_base::trunc);

	return fs;
}
