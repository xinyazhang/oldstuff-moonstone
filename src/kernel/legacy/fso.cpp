#include "common.h"

fso_t::fso_t()
	:fsoid_(0), parent_(0), size_(0), mtime_(0), mtimer_(0), hash_algo_(0), hash_(NULL)
{
}

fso_t::fso_t(idx_t fsoid)
	:fsoid_(fsoid), parent_(0), size_(0), mtime_(0), mtimer_(0), hash_algo_(0), hash_(NULL)
{
}

fso_t::fso_t(const unistr& path)
	:fsoid_(0), hash_algo_(0), hash_(0)
{
	name_ = path;
	/*
	QFileInfo info_(path);
	name_ = info_.fileName();
	path_ = info_.absoluteFilePath();
	size_ = info_.size();
	mtimer_ = mtime_ = info_.lastModified().toTime_t();
	*/
}

void fso_t::addbind(sql_stmt& stmt, idx_t parentid)
{
	stmt.bind(1, parentid);
	stmt.bind(2, name_);
	stmt.bind(3, size_);
	stmt.bind(4, mtime_);
	stmt.bind(5, mtimer_);
	stmt.bind(6);
	stmt.bind(7);

	parent_ = parentid;
}

void fso_t::updatebind(sql_stmt& stmt)
{
	stmt.bind(1, fsoid_);
	stmt.bind(2, parent_);
	stmt.bind(3, name_);
	stmt.bind(4, size_);
	stmt.bind(5, mtime_);
	stmt.bind(6, mtimer_);
	stmt.bind(7);
	stmt.bind(8);
}

bool fso_t::is_root() const
{
	return fsoid_ == 0;
}

unistr fso_t::name() const
{
	return name_;
}

/*
	fso(
		fsoid INTEGER PRIMARY KEY ASC AUTOINCREMENT
		parentid INTEGER REFERENCES $PREFIX_fso(fsoid) ON DELETE CASCADE
		name TEXT
		size INTEGER
		fs_date INTEGER
		recusive_date INTEGER
		hash_algo INTEGER
		hash BLOB
		)
	UNIQUE(parentid, name));")
*/

void fso_t::load(sql_stmt& stmt)
{
	stmt.col(1, fsoid_);
	stmt.col(2, parent_);
	stmt.col(3, name_);
	//stmt.col(4, );
	stmt.col(5, mtime_);
	stmt.col(6, mtimer_);
}

bool fso_t::valid() const
{
	return fsoid_ != 0 || ( fsoid_ == 0 && name_ == "/");
}
