#ifndef KERNEL_METADB_H
#define KERNEL_METADB_H

#include "../pal/stdtype.h"
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>
#include <vector>
#include "../dal/dbmeta.h"

/*
 * metadb is meta database of databases
 * dbmeta is metadata of kinds of databases
 */

class EXPORT_TO_DLL metadb_t
{
public:
	metadb_t(QXmlStreamReader& );
	~metadb_t();

	int dbnumber() const; 
	int seg_num(int dbidx) const;
	unistr seg_name(int dbidx, int segidx) const;
	unistr seg_data(int dbidx, int segidx) const;
	void save(QXmlStreamWriter& );

	enum
	{
		NameSeg = 0
	};

	static void registermeta(dbmeta_t&);
	shared_ptr<dbmeta_t> dbmeta(int dbidx) const { return dbs_[dbidx]; }
private:
	static std::vector<dbmeta_t*> metas_;// stub dbmeta, every kind
	std::vector<shared_ptr<dbmeta_t> > dbs_;
};

#endif
