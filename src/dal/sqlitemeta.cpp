#include "sqlitemeta.h"
#include <QtCore/QXmlStreamReader>
#include "db_sqlite_impl.h"

#define SEGNUM 2
//#define DBMETA_CHECK_RANGE(x, size, y) { if ( (x) < 0 || (x) >= (size)) return (y); }

sqlitemeta::sqlitemeta(const unistr& name, const unistr& file)
	:name_(name), file_(file)
{
}

sqlitemeta& sqlitemeta::instance() 
{ 
	static sqlitemeta i; 
	return i; 
}

bool sqlitemeta::parse(QXmlStreamReader& reader, shared_ptr<dbmeta_t>& retv)
{
	name_.clear();
	file_.clear();
	while ( reader.readNextStartElement() )
	{
		if ( reader.name() == "Name" )
			name_ = reader.readElementText();
		else if ( reader.name() == "File" )
			file_ = reader.readElementText();
		else
			reader.skipCurrentElement();
	}
	if ( name_.empty() || file_.empty() )
		return false;
	retv = shared_ptr<dbmeta_t>(new sqlitemeta(name_, file_));
	return true;
}

int sqlitemeta::seg_num() const
{
	return SEGNUM;
}

static unistr name[] = {"Database Name", "File"};

unistr sqlitemeta::seg_name(int segidx) const
{
	DBMETA_CHECK_RANGE(segidx, SEGNUM, unistr());
	return name[segidx];
}

unistr sqlitemeta::seg_data(int segidx) const
{
	DBMETA_CHECK_RANGE(segidx, SEGNUM, unistr());
	if ( segidx == 0 )
		return name_;
	else
		return file_;
}

unistr sqlitemeta::database_typename() const
{
	return unistr("SQLite");
}

DatabaseInterface* sqlitemeta::opendb()
{
	db_sqlite_impl* db = new db_sqlite_impl(file_);
	return db;
}

bool sqlitemeta::compare(const dbmeta_t* r) const
{
	const sqlitemeta* rhs = dynamic_cast<const sqlitemeta*>(r);

	return rhs->name_ ==  name_ && rhs->file_ == file_;
}
