#ifndef DATABASE_ABSTRACT_LAYER_SQLITE_METADATA_H
#define DATABASE_ABSTRACT_LAYER_SQLITE_METADATA_H

#include "../pal/stdtype.h"
#include "dbmeta.h"

class QXmlStreamReader;

class EXPORT_TO_DLL sqlitemeta
	:public dbmeta_t
{
public:
	sqlitemeta(const unistr& name = unistr(), const unistr& file = unistr());
	virtual bool parse(QXmlStreamReader&, shared_ptr<dbmeta_t>&);
	virtual int seg_num() const;
	virtual unistr seg_name(int segidx) const;
	virtual unistr seg_data(int segidx) const;
	virtual unistr database_typename() const;
	virtual DatabaseInterface* opendb();
	virtual bool compare(const dbmeta_t* )const;

	static sqlitemeta& instance();
private:
	unistr name_;
	unistr file_;
};

#endif
