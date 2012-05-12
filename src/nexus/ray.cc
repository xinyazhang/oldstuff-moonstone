#include "ray.h"
#include "Database.h"
#include "dentry.h"
#include "filemgr.cc"
#include <pal/volume.h>

ray_t::ray_t(const volume& vol, Database* db, tracer_t* tracer)
	:db_(db), tracer_(tracer), vol_(vol)
{
}

ray_t::~ray_t()
{
}

int ray_t::doit()
{
	filemgr_t *fmgr = db_->filemgr();
	tracer->wait_until_start();
	fmgr->checkbegin(vol.kpi);
	dentry_t dentry;
	dentry.kpi = vol.kpi;
	while (!next(dentry))
		fmgr->ack(dentry);
	fmgr->checkdone(vol.kpi);
	tracer->update_after_ray();
	return 0;
}
