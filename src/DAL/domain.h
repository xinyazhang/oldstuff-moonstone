#ifndef _REMUS_DATABASE_DOMAIN_CLASS_
#define _REMUS_DATABASE_DOMAIN_CLASS_

#include "../core/int_type.h"
#include "../core/UniStr.h"

#include <boost/shared_ptr.hpp>

class Tag;
#if _PROTOTYPE_ > 2
class TagFamily;
class TagAliasGroup;
class AttrFamily;
class Resource;
class Container;
class TagAttach;
class Attr;
#endif

typedef int64_t index_t;

class IndexedObject
{
protected:
	IndexedObject();
	virtual ~IndexedObject();
public:
	index_t idx() { return idx_; }
	void set_idx(index_t idx) { idx_ = idx; }
private:
	int64_t idx_;
};

#if _PROTOTYPE_ > 2
class Family
	:public IndexedObject
{
public:
	virtual ~Family() = 0;
	virtual UniStr name() = 0;
	virtual void set_name(const UniStr&) = 0;
};

#else
class Family
	:public IndexedObject
{
public:
	Family();
	UniStr name() { return name_; }
	void set_name(const UniStr& name) { name_ = name; }
private:
	UniStr name_;
};
#endif
typedef boost::shared_ptr<Family> FamilyRef;

class Tag
	:public IndexedObject
{
public:
	virtual ~Tag() = 0;
	virtual UniStr name() = 0;
	virtual void set_name(const UniStr&) = 0;
#if _PROTOTYPE_ > 2
	virtual TagFamilyRef family() = 0;
	virtual void set_family(const TagFamily&) = 0;
	virtual TagAliasGroupRef alias() = 0;
#endif

#if _PROTOTYPE_ > 3
	/* 
	 * These interfaces below shall be added future.
	 */
	virtual TagRef allian_next() = 0;
	virtual TagRef allian_prev() = 0;
	virtual void ally(const Tag&) = 0; // set ally
#endif
	// no means as we uses shared_ptr
	// virtual bool operator==(const Tag&) = 0;
};

typedef boost::shared_ptr<Tag> TagRef;

#if _PROTOTYPE_ > 2
class TagAliasGroup
{
public:
	virtual ~TagAliasGroup() = 0;
	virtual Tag master() = 0;
	virtual void set_master(const& Tag) = 0;
	virtual bool test(const& Tag) = 0;
};

class AttrFamily
{
public:
	const int IntegerAttrib = 1;
	const int FloatAttrib = 2;
	const int DateAttrib = 3;

	virtual ~AttrFamily() = 0;
	virtual UniStr name() = 0;
	virtual void set_name(const UniStr&) = 0;
	virtual int data_type() = 0;
	virtual void set_data_type(int) = 0;
};

class Resource
{
public:
	virtual ~Resource() = 0;
	virtual UniStr location() = 0;
	virtual void set_location(const UniStr&) = 0;
	// access method 
	// plan: 
	// return access object to access
	// various resources in different type location, planed:
	// 1. Local Volume
	// 2. FTP
};

class Container
{
public:
	virtual ~Container() = 0;
	virtual UniStr location() = 0;
	virtual void set_location(const UniStr&) = 0;
	// access method
	// same as Resource class
};

class TagAttach
{
public:
	virtual ~TagAttach() = 0;
	/*
	 * Same name, different usage
	 * first one is to return Tag
	 * second one is to tag Tag
	 */
	virtual Tag tag() = 0;
	virtual void tag(const Tag&) = 0;
	virtual Resource res() = 0;
	virtual void set_res(const Resource&) = 0;
};

class Attr
{
public:
	virtual ~Attr() = 0;
	virtual AttrFamily family() = 0;
	virtual UniStr value() = 0;
	virtual void set_value(const UniStr&) = 0;
};
#endif

#endif
