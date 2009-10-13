#include "DataBase.h"
#include "SQL.h"
#include "SQLiteDomain.h"

SQLite::SQLite(const UniStr& filename)
	:db_(NULL)
{
	sqlite3_openu(filename.c_str(), &db_);
}

SQLite::~SQLite()
{
	close();
}

void SQLite::initialize()
{
	easyquery(sql_unicode);
	easyquery(sql_create_tag_table);
	easyquery(sql_initial_tag_table);

	easyquery(sql_create_alias);
	easyquery(sql_initial_alias);

	easyquery(sql_create_family);
	easyquery(sql_initial_family);
}

void SQLite::close()
{
	if (db_)
	{
		sqlite3_close(db_);
		db_ = NULL;
	}
}

Tag* SQLite::create_tag()
{
	return new SQLiteTag(0);
}

TagRef SQLite::create_tag(const UniStr& name = UniStr(), const UniStr& family = UniStr(), OpenFlag flag = flag_default)
{
	index_t idx = 0;
	bool exactily = false;
	bool hit = false;

	sqlite3_stmt *stmt;
	sqlite3_prepare_u(db_, UT("select * from remus_tag where name='"+escape(name)+"';"), &stmt, NULL);
	int result = sqlite3_step(stmt);
	
	if ( result == SQLITE_ROW )
		idx = split_tag_row(stmt);

	while ( result == SQLITE_ROW )
	{
		count++;
		UniStr sql_family = split_tag_family(stmt);
		if ( !family.empty() && family == sql_family )
		{
			exactily = true;
			idx = split_tag_idx(stmt);
			break;
		}

		result = sqlite3_step(stmt);
	}

	hit = (family.empty() && count == 1) || exactily;

	if ( hit || flag != open_existing )
	{
		return rc_->create_tag(idx);
	} else
		return TagRef::null_ref;

}

Tag* create_tag(const UniStr& name, const UniStr& family, OpenFlag flag = flag_default)
{
	Tag* ret = new SQLiteTag;
	int idx = 0;
	// check for existing
	sqlite3_stmt *stmt;
	sqlite3_prepare_u(db_, UT("select * from remus_tag where name='"+escape(name)+"';"), &stmt, NULL);

	int result = sqlite3_step(stmt);
	int count = 0;
	bool hit = false;

	/*
	 * In fact, there are much more optimization can be made here
	 * However, works firstly, then optimization.
	 *
	 * Premature optimization is the root of all evil. -- Donald Knuth
	 */
	while ( result == SQLITE_ROW )
	{
		count++;
		if( count == 1 )
		{
			split_tag_row(stmt, ret);
		} else 
		{
			UniStr sql_family = split_tag_family(stmt);
			if ( family == sql_family )
			{
				hit = true;
				split_tag_row(stmt, ret);
				break;
			}
		}
		result = sqlite3_step(stmt);
	}
	// the routines above dos these things:
	// 1. count the row counts
	// 2. Fill the Tag with the first line if count <= 1
	// 3. If and only if name and family meets, replace the ret with new row data.
	
	// While count = 1, it means the name single can locate the tag
	if ( count == 1 )
	{
		if ( family.empty() )
		{
			// user didn't appoint the family name
			// ok, don't check
		} else 
		{
			if ( family == ret->family() )
			{
				// user appointed the family name, and they are the same
			} else
			{
				// user appointed the family name
				// but they are different, ok, that's a new tag.
				ret->set_idx(0);
			}
		}
	} else 
	{
		// OK, now count > 1
		if ( hit )
		{
			// hit a row
			// didn't do any thing
		} else
			ret->set_idx(0);
	}

	if ( OpenFlag == open_existing )
	{
		if ( ret->idx() == 0 )
		{
			delete ret;
			ret = NULL;
		}
	}
	return ret;
}

void SQLite::rm_tag(const UniStr& name, const UniStr& family)
{
	Tag* tag = create_tag(name, family, open_existing);
	if ( tag )
	{
		guarntee_query(UT("delete from remus_tag where idx=")+tag->idx());
		kill_tag(tag);
	}
}

void SQLite::flush_tag(Tag* tag)
{
	SqlStr sql;
	if ( tag->idx() == 0 )
	{
		sql = UT("INSERT INTO remus_tag values(NULL, ");
		sql += tag->idx();
		sql += ',';
		sql += tag->name();
		sql += ',';
		sql += tag->family()->idx();
		sql += ',';
		sql += tag->alias()->idx();
		sql += ',';
		sql += "NULL";
		sql += ',';
		sql += "NULL";
		sql += ");";
	} else
	{
		sql = UT("UPDATE remus_tag SET ");
		sql += UT("name = ");
		sql += tag->name();
		sql += UT(", family = ");
		sql += tag->family()->idx();
		sql += UT(", alias = ");
		sql += tag->alias()->idx();
		sql += UT(", prev_allian = ");
		sql += tag->allian_prev()->idx();
		sql += UT(", post_allian = ");
		sql += tag->allian_post()->idx();
		sql += UT(" WHERE idx = ");
		sql += tag->idx();
		sql += ';';
	}
}

void SQLite::close_tag(Tag* tag)
{
	flush_tag(tag);
	kill_tag(tag);
}

DataBase::TagIterator* SQLite::create_tag_iterator()
{
	SQLiteTagIterator* iter = new SQLiteTagIterator(this);

	sqlite3_prepare_u(db_, UT("SELECT * FROM remus_tag WHERE idx > 0;"), &iter->stmt_, NULL);
}

Tag* SQLite::read_tag(TagIterator* iter_in)
{
	SQLiteTagIterator* iter = dynamic_cast<SQLiteTagIterator*> iter_in;
	if ( !iter )
		return NULL;
	int result = sqlite3_step(iter->stmt);
	if ( result == SQLITE_ROW )
	{
		Tag* ret = new SQLiteTag;
		split_tag_row(iter->stmt, ret);
		return ret;
	} else if ( result == SQLITE_DONE )
		return NULL;
}

TagFamily* SQLite::create_family(const UniStr& name, OpenFlag flag = flag_default)
{
	TagFamily* ret = new SQLiteTagFamily;
	sqlite3_stmt *stmt;

	SqlStr sql = UT("SELECT * FROM remus_family WHERE name = ");
	sql += name;
	sql += ';'
	sqlite3_prepare_u(db_, &stmt, NULL);
	int result = sqlite3_step(stmt);
	if ( result == SQLITE_ROW )
	{
		// get a result
		split_family_row(stmt, ret);
	} else
	{
		ret->set_idx(0);
		ret->set_name(name);
	}

	if ( flag == open_existing )
	{
		delete ret;
		ret = NULL;
	}
	return ret;
}
