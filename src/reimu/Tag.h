#ifndef _KERNEL_TAG_
#define _KERNEL_TAG_

#include "IndexedObject.h"
#include "Declares.h"
#include "RefCounter.h"
#include "CommonDeleter.h"

class Tag
	:public IndexedObject
{
	friend class Category;
public:
	Tag(index_t idx, Category* cat, Database* db, RefCounter<Tag>::RCRef rc);
	UniStr name() const;
	index_t inode() const;
	void set_name(const UniStr&);
	void load();
	void store();
	FamilyRef family() const;
	void set_family(FamilyRef );

	typedef CommonDeleter<Tag> Deleter;
	friend class Deleter;
private:
	UniStr name_;
	RefCounter<Tag>::RCRef rc_;
	index_t family_;
	index_t inode_;
	mutable FamilyRef family_ref_;
	TagNodeRef node_ref_;

private:
	void load(Database* );
};

#endif
