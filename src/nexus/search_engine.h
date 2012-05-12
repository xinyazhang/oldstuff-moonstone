#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include <pal/stdtype.h>

enum ENGINE_EVENT
{
	SE_JOB_DONE = 1,

	SE_LAST_EVENT
};

class EXPORT_TO_DLL search_engine_t
{
public:
	search_engine_t(class Database* );

	class search_service* setup_service(void* cookie, cb_func_t);
	void finalize_service(class search_service*);

	void post_search(class search_service*, const unichar* );
	void update_service_state(class search_service*);

	int line_count(class search_service* serv);
	unistr data(class search_service* line, int row, int column);
private:
	class Database* db_;
};

#endif
