#include "metadb.h"

#define RANGE_CHECK(x, size, y) { if ( (x) < 0 || (x) >= (size)) return (y); }

std::vector<dbmeta_t*> metadb_t::metas_;

metadb_t::metadb_t(QXmlStreamReader& reader)
{
	shared_ptr<dbmeta_t> dbmeta;
	reader.readNextStartElement();
	if ( !reader.isStartElement() || reader.name() != "MetaDB" )
	{
		//QStringRef m = reader.name();
		return ;
	}
	while (reader.readNextStartElement())
	{
		for(std::vector<dbmeta_t*>::const_iterator iter = metas_.begin();
				iter != metas_.end();
				iter++)
		{
			//reader.readNext();
			if ((*iter)->read(reader, dbmeta))
			{
				dbs_.push_back(dbmeta);
			}
		}
	};
}

metadb_t::~metadb_t()
{
}

int metadb_t::dbnumber() const
{
	return dbs_.size();
}

int metadb_t::seg_num(int dbidx) const
{
	RANGE_CHECK(dbidx, dbs_.size(), 0);
	return dbs_[dbidx]->seg_num();
}

unistr metadb_t::seg_name(int dbidx, int segidx) const
{
	RANGE_CHECK(dbidx, dbs_.size(), unistr());
	return dbs_[dbidx]->seg_name(segidx);
}

unistr metadb_t::seg_data(int dbidx, int segidx) const
{
	RANGE_CHECK(dbidx, dbs_.size(), unistr());
	return dbs_[dbidx]->seg_data(segidx);
}

void metadb_t::save(QXmlStreamWriter& )
{
	// stub
}

void metadb_t::registermeta(dbmeta_t& meta)
{
	metas_.push_back(&meta);
}
