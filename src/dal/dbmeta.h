#ifndef DATABASE_ABSTRACT_LAYER_DATABASE_METADATA_H
#define DATABASE_ABSTRACT_LAYER_DATABASE_METADATA_H

#include "../pal/stdtype.h"

class DatabaseInterface;

class QXmlStreamReader;

class EXPORT_TO_DLL dbmeta_t
{
public:
	virtual bool read(QXmlStreamReader&, shared_ptr<dbmeta_t>&); // default actions
	virtual int seg_num() const = 0;
	virtual unistr seg_name(int segidx) const = 0;
	virtual unistr seg_data(int segidx) const = 0;
	virtual unistr database_typename() const = 0;
	virtual DatabaseInterface* opendb() = 0;
	virtual bool compare(const dbmeta_t* )const = 0;
private:
	virtual bool parse(QXmlStreamReader&, shared_ptr<dbmeta_t>&) = 0;
};

inline bool EXPORT_TO_DLL operator==(const dbmeta_t& lp, const dbmeta_t& rp)
{
	if ( typeid(lp) != typeid(rp) )
		return false;
	else
		return lp.compare(&rp);
}

#define DBMETA_CHECK_RANGE(x, size, y) { if ( (x) < 0 || (x) >= size) return y; }

#endif
