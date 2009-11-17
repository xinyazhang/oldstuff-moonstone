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

	// attributes
	index_t inode() const;
	UniStr name() const;
	FamilyRef family() const;
	bool primary() const;

	void load(); // synchronize from db
	void store(); // synchronize to db

	bool chname(const UniStr&, const FamilyRef);
	bool chname(const UniStr&);
	bool setprimary();
private:
	UniStr name_;
	RefCounter<Tag>::RCRef rc_;
	index_t family_;
	index_t inode_;
	mutable FamilyRef family_ref_;
	TagNodeRef node_ref_;

private:
	void load(Database* );

	// Static part
public:
	typedef CommonDeleter<Tag> Deleter;
	static TagParameters Para();
	friend class Deleter;
	// typedef part
public:
	typedef RefCounter<Tag>::RCRef RCRef;
	typedef boost::shared_ptr<Tag> Ref;
	typedef TagIterator Iterator;
	typedef boost::shared_ptr<TagIterator> IteratorRef;
	typedef TagParameters Para;
	static const char* table_name;
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
