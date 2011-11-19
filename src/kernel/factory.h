#ifndef FACTORY_H
#define FACTORY_H

namespace factory {
	class fdpool_t* fdpool();
	class tasklet_queue_t* tasklet_queue();
	class vfp_queue_t* vfp_queue(class Database* db, class fdpool_t* sink);
	class tasklet_t* volpreplet(int, const struct volume&, class fdpool_t*, class Database* db);
	class watching_t* watching(const struct volume&, class Database*);
};

#endif
