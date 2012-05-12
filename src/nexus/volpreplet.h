#ifndef VOLPREPLET_H
#define VOLPREPLET_H

#include <vector>
//#include <pal/volume.h>

struct volume;

namespace statistic {
	void get_watching_volume(std::vector<volume>&);
};

#endif
