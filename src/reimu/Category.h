#ifndef _KERNEL_CATEGORY_
#define _KERNEL_CATEGORY_

#include "Declares.h"
#include "RefCounter.h"

class TagIterator;
/*
 * Create memory objects which stands for db objects
 */
class Category
{
public:
	enum OpenFlag
	{
		flag_default = 0,
		open_existing = 1
	};

	Category(Database* db);
	~Category();

	TagRef create_tag(const UniStr& name = UniStr(), const UniStr& family = UniStr(), OpenFlag = flag_default);
	TagRef create_tag(index_t idx);
	void rm_tag(const UniStr& name, const UniStr& family);

	FamilyRef create_family(const UniStr& name = UniStr(), OpenFlag = flag_default);
	FamilyRef create_family(index_t idx);

	TagIterator* create_tag_iterator();
	TagRef read_tag(TagIterator*);
	void close_tag_iterator(TagIterator* );

private:
	index_t find_tag_in_db(Database* db, const UniStr& name, const UniStr& family);

private:
	Database* db_;
	Database* memdb_;
	RefCounter<Tag>::RCRef tagrc_;
};

#endif
