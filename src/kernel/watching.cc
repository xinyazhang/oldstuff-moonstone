#include "watching.h"

watching_t::watching_t(const struct volume& _vol, class Database* db)
	:dbmgr(db), vol(_vol)
{
}
