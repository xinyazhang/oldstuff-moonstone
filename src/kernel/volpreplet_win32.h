#ifndef VOLPREPLET_WIN32_H
#define VOLPREPLET_WIN32_H

#include "tasklet.h"
#include <pal/volume.h>

class volpreplet_win32
	:public tasklet_t
{
public:
	volpreplet_win32(int evid, const volume& vol, class fdpool_t* pool, class Database* db);
	~volpreplet_win32();

	int doit();
	bool terminated() const;
private:
	class fdpool_t* pool_;
	volume vol_;
}

#endif
