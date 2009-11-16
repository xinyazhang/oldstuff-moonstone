#ifndef _KERNEL_TAG_
#define _KERNEL_TAG_

#include "IndexedObject.h"
#include "Declares.h"
#include "RefCounter.h"
#include "CommonDeleter.h"
#include "Parameters.h"
#include "Category.h"

class TagParameters;

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

	// Static part
public:
	typedef CommonDeleter<Tag> Deleter;
	friend class Deleter;
	// typedef part
public:
	typedef RefCounter<Tag>::RCRef RCRef;
	typedef boost::shared_ptr<Tag> Ref;
	typedef TagIterator Iterator;
	typedef boost::shared_ptr<TagIterator> IteratorRef;
	typedef TagParameters Para;
	static const char* table_name;
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

class TagParameters
	:public Parameters
{
public:
	TagParameters();
	TagParameters& arg(const UniStr&);
	TagParameters& arg_family(const UniStr&);
public:
	typedef TagRef Ref;
	typedef Tag::RCRef RCRef;
	static const char* table_name;
	//static const Tag::RCRef Category::*rc = &Category::tagrc_;
	static const Tag::RCRef Category::*rc;
};

#endif
