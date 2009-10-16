#ifndef _REMUS_DAL_DATABASE_
#define _REMUS_DAL_DATABASE_

#include "../core/UniStr.h"
#include "domain.h"


class DataBase
{
public:
	enum OpenFlag
	{
		flag_default = 0,
		open_existing = 1,
	};

	class TagIterator
	{
		friend class DataBase;
	protected:
		TagIterator();
	public:
		virtual ~TagIterator();
		TagRef read_tag();
	};
public:
	//DataBase(const UniStr&);
	virtual ~DataBase();

	virtual void initialize() = 0; // initialize a db, if haven't done before
	virtual void close() = 0; // close db connection or such thing.
	
	virtual TagRef create_tag(const UniStr& name = UniStr(), const UniStr& family = UniStr(), OpenFlag = flag_default) = 0;
	virtual void rm_tag(const UniStr& name, const UniStr& family) = 0;
	virtual void flush_tag(TagRef ) = 0;
	virtual void close_tag(TagRef& ) = 0;
	virtual void kill_tag(TagRef& ) = 0;

	virtual TagIterator* create_tag_iterator() = 0;
	virtual TagRef read_tag(TagIterator*) = 0;

	virtual FamilyRef create_family(const UniStr& name, OpenFlag = flag_default) = 0;
	virtual void rm_family(const UniStr& name) = 0;
	virtual void close_family(FamilyRef& ) = 0;
	virtual void kill_family(FamilyRef& ) = 0;
};

#endif
