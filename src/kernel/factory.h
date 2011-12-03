#ifndef FACTORY_H
#define FACTORY_H

class fdpool_t;
class tasklet_queue_t;
class vfp_queue_t;
class tasklet_t;
class watching_t;
struct volume;
class Database;

namespace factory {
	fdpool_t* fdpool();
	tasklet_queue_t* tasklet_queue();
	vfp_queue_t* vfp_queue(Database* db, fdpool_t* sink);
	tasklet_t* volpreplet(int, const volume&, fdpool_t*, Database* db);
	watching_t* watching(const volume&, Database*);
};

#endif
