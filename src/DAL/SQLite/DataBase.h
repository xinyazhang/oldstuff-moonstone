#ifndef _REMUS_DAL_SQLITE_
#define _REMUS_DAL_SQLITE_

#include "../DataBase.h"
#include "sqlite3.h"
#include "SQLiteRefCounter.h"

class SQLite
	:public DataBase
{
public:
	SQLite(const UniStr&);
	~SQLite();

	void initialize(); // initialize a db, if haven't done before
	void close(); // close db connection or such thing.

	Tag* create_tag();
	Tag* create_tag(const UniStr& name, const UniStr& family, OpenFlag = flag_default);
	void rm_tag(const UniStr& name, const UniStr& family);
	void flush_tag(Tag* );
	void kill_tag(Tag* );
	void close_tag(Tag* );

	TagIterator* create_tag_iterator();
	Tag* read_tag(TagIterator*);

	TagFamily* create_family(const UniStr& name, OpenFlag = flag_default);
	void rm_family(const UniStr& name);
	void close_family(TagFamily* );
	void kill_family(TagFamily* );
private:
	sqlite3* db_;
	SQLiteRefCounter<SQLiteTag>* tagrc_;
};

#endif
