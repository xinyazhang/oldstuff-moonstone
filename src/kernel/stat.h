#ifndef STAT_H
#define STAT_H

#include <pal/volume.h>
#include <boost/thread.hpp>

namespace stat
{
	std::vector<volume> watching_volumes;
	boost::mutex	watching_vol_lock;
};

#endif
