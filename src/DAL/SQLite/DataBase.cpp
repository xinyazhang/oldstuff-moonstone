#include "DataBase.h"
#include "DomainImpl.h"
#include "../../core/UniStr.h"
#include "SQLiteEasy.h"
#include "SQL.h"
#include "TagIterator.h"

#if _PROTOTYPE_ < 2
typedef UniStr SqlStr;
#endif

SQLite::SQLite(const UniStr& filename)
	:db_(NULL)
{
	sqlite3_open_u(filename.c_str(), &db_);
	tagrc_ = new SQLiteRefCounter<SQLiteTag>(db_);
	familyrc_ = new SQLiteRefCounter<Family>(db_);
}

SQLite::~SQLite()
{
	close();
}

void SQLite::initialize()
{
	easyquery(db_, sql_unicode);
	easyquery(db_, sql_create_tag_table);
	easyquery(db_, sql_initial_tag_table);

#if _PROTOTYPE_ > 2
	easyquery(sql_create_alias);
	easyquery(sql_initial_alias);

	easyquery(sql_create_family);
	easyquery(sql_initial_family);
#endif
}

void SQLite::close()
{
	if (db_)
	{
		sqlite3_close(db_);
		db_ = NULL;
	}
}

TagRef SQLite::create_tag(const UniStr& name = UniStr(), const UniStr& family = UniStr(), OpenFlag flag)
{
	index_t idx = 0;
	int count = 0;
	bool exactily = false;
	bool hit = false;

	sqlite3_stmt *stmt;
	sqlite3_prepare_u(db_, (UT("select * from remus_tag where name='")+escape(name)+UT("';")).c_str(), -1, &stmt, NULL);
	int result = sqlite3_step(stmt);
	
	if ( result == SQLITE_ROW )
		idx = split_idx(stmt);

	while ( result == SQLITE_ROW )
	{
		count++;
#if _PROTOTYPE_ > 2
		UniStr sql_family = split_tag_family(stmt);
		if ( !family.empty() && family == sql_family )
		{
			exactily = true;
			idx = split_idx(stmt);
			break;
		}
#endif

		result = sqlite3_step(stmt);
	}
	sqlite3_finalize(stmt);

	hit = (family.empty() && count == 1) || exactily;

	if ( hit || flag != open_existing )
	{
		TagRef ref = tagrc_->create(idx);
		if ( idx == 0 )
		{
			ref->set_name(name);
#if _PROTOTYPE_ > 2
			ref->set_family(create_family(family, flag));
#endif
		}
	} else
		return TagRef();
}

void SQLite::rm_tag(const UniStr& name, const UniStr& family)
{
	TagRef tag = create_tag(name, family, open_existing);
	if ( tag )
	{
		guarntee_query(db_, UT("delete from remus_tag where idx=")+tag->idx());
		kill_tag(tag);
	}
}

void SQLite::flush_tag(TagRef tag)
{
	SqlStr sql;
	if ( tag->idx() == 0 )
	{
		sql = UT("INSERT INTO remus_tag values(NULL, ");
		sql += tag->name();
#if _PROTOTYPE_ > 2 
		sql += ',';
		sql += tag->family()->idx();
#endif
#if _PROTOTYPE_ > 3
		sql += ',';
		sql += tag->alias()->idx();
		sql += ',';
		sql += "NULL";
		sql += ',';
		sql += "NULL";
#endif
		sql += UT(");");
		easyquery(db_, sql.c_str());

		sqlite3_stmt* stmt;
		sqlite3_prepare_u(db_, UT("SELECT MAX(idx) FROM remus_tag;"), -1, &stmt, NULL);
		sqlite3_step(stmt);

		tag->set_idx(sqlite3_column_int64(stmt, 0));

		sqlite3_finalize(stmt);

#if _PROTOTYPE_ > 2
		tagrc_->replace(tag);
#endif
	} else
	{
		sql = UT("UPDATE remus_tag SET ");
		sql += UT("name = ");
		sql += tag->name();
#if _PROTOTYPE_ > 2 
		sql += UT(", family = ");
		sql += tag->family()->idx();
#endif
#if _PROTOTYPE_ > 3
		sql += UT(", alias = ");
		sql += tag->alias()->idx();
		sql += UT(", prev_allian = ");
		sql += tag->allian_prev()->idx();
		sql += UT(", post_allian = ");
		sql += tag->allian_post()->idx();
#endif
		sql += UT(" WHERE idx = ");
		sql += tag->idx();
		sql += UT(';');
		easyquery(db_, sql.c_str());
	}
}

void SQLite::kill_tag(TagRef& tag)
{
	tag.reset();
}

void SQLite::close_tag(TagRef& tag)
{
	flush_tag(tag);
	kill_tag(tag);
}

DataBase::TagIterator* SQLite::create_tag_iterator()
{
	SQLiteTagIterator* iter = new SQLiteTagIterator(this, tagrc_);

	return iter;
}

TagRef SQLite::read_tag(TagIterator* iter_in)
{
	/*
	SQLiteTagIterator* iter = dynamic_cast<SQLiteTagIterator*> iter_in;
	if ( !iter )
		return NULL;
	int result = sqlite3_step(iter->stmt);
	if ( result == SQLITE_ROW )
	{
		TagRef ret = tagrc_->create(split_idx(stmt));
		return ret;
	} else if ( result == SQLITE_DONE )
		return NULL;
	*/
	return iter_in->read_tag();
}

void SQLite::close_tag_iterator(TagIterator* iter)
{
	delete iter;
}

FamilyRef SQLite::create_family(const UniStr& name, OpenFlag flag)
{
#if _PROTOTYPE_ > 2
	FamilyRef ret;
	sqlite3_stmt *stmt;

	SqlStr sql = UT("SELECT * FROM remus_family WHERE name = ");
	sql += name;
	sql += ';'
	sqlite3_prepare_u(db_, &stmt, NULL);
	int result = sqlite3_step(stmt);
	if ( result == SQLITE_ROW )
	{
		// get a result
		ret = familyrc_->create(split_idx(stmt));
	} else if (flag != open_existing)
	{
		ret = familyrc_->create(0);
	}

	return ret;
#else
	return FamilyRef();
#endif
}

void SQLite::rm_family(const UniStr& name)
{
#if _PROTOTYPE_ > 2
	FamilyRef family = create_family(name, open_existing);
	if ( family )
	{
		guarntee_query(db_, UT("delete from remus_family where idx=")+tag->idx());
	}
#endif
}

void SQLite::flush_family(FamilyRef family)
{
#if _PROTOTYPE_ > 2
	SqlStr sql;
	if ( family->idx() != 0 )
	{
		sql = UT("UPDATE remus_family SET name = ");
		sql += family->name();
		sql += UT(" WHERE idx = ");
		sql += family->idx();
		sql += UT(";");
		easyquery(db_, sql);
	} else 
	{
		sql = UT("INSERT INTO remus_family VALUES(NULL, ");
		sql += family->name();
		sql += UT(");");
		easyquery(db_, sql);

		sqlite3_stmt* stmt;
		sqlite3_prepare_u(db_, UT("SELECT MAX(idx) FROM remus_family;"), -1, &stmt, NULL);
		sqlite3_step(stmt);

		family->set_idx(sqlite3_column_int64(stmt, 0));

		sqlite3_finalize(stmt);

		familyrc_->replace(family);
	}
#endif
}

void SQLite::close_family(FamilyRef& family)
{
#if _PROTOTYPE_ > 2
	flush_family(family);
	kill_family(family);
#endif
}

void SQLite::kill_family(FamilyRef& family)
{
#if _PROTOTYPE_ > 2
	family.reset();
#endif
}
