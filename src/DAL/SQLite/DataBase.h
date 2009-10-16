#ifndef _REMUS_DAL_SQLITE_
#define _REMUS_DAL_SQLITE_

#include "../DataBase.h"
#include "sqlite3.h"
#include "RefCounter.h"

class SQLite
	:public DataBase
{
	friend class SQLiteTagIterator;
public:
	SQLite(const UniStr&);
	~SQLite();

	void initialize(); // initialize a db, if haven't done before
	void close(); // close db connection or such thing.

	TagRef create_tag(const UniStr& name, const UniStr& family, OpenFlag = flag_default);
	void rm_tag(const UniStr& name, const UniStr& family);
	void flush_tag(TagRef );
	void kill_tag(TagRef& );
	void close_tag(TagRef& );

	TagIterator* create_tag_iterator();
	TagRef read_tag(TagIterator*);
	void close_tag_iterator(TagIterator* );

	FamilyRef create_family(const UniStr& name, OpenFlag = flag_default);
	void rm_family(const UniStr& name);
	void flush_family(FamilyRef );
	void close_family(FamilyRef& );
	void kill_family(FamilyRef& );
private:
	sqlite3* db_;
	SQLiteRefCounter<SQLiteTag>* tagrc_;
#if _PROTOTYPE_ > 2
	SQLiteRefCounter<SQLiteFamily>* familyrc_;
#else
	SQLiteRefCounter<Family>* familyrc_;
#endif
};

#endif
