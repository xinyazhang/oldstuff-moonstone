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

	template<class P>
		typename P::Ref create(P args, OpenFlag = flag_default);
	template<class T>
		typename T::Ref create(index_t idx);
	template<class P>
		void rm(P args);

	template<class T>
		typename T::IteratorRef create_iterator();
	/* 
	 * Read from T::iterator is removed
	 * this function is provided as T::iterator's member function
	 */

	/*
	 * Close T::iterator is also removed
	 * shared_ptr managed it for us
	 */

	const char* table_prefix() const { return table_prefix_; }
private:
	/* 
	 * return positive value when unique hit in solid database
	 * 		,that's the idx of object in solid database;
	 * return negative value when unique hit in memory(imaginary) database
	 * 		,that's the idx of object in imaginary database;
	 * return 0 when not hit or multiple hit
	 */
	index_t find_in_db(Database* db, 
			const UniStr& table, const Parameters& args);

	template <typename P>
		typename P::RCRef& get_rc()
		{
			return this->*(P::rc);
		}

private:
	Database* db_;
	Database* memdb_;
	RefCounter<Tag>::RCRef tagrc_;
	RefCounter<Family>::RCRef familyrc_;
	UniStr table_prefix_;
public:
	static RefCounter<Tag>::RCRef Category::* get_tagrc_ptr() { return &Category::tagrc_; }
	static RefCounter<Family>::RCRef Category::* get_familyrc_ptr() { return &Category::familyrc_; }
};

#include "CategoryT.cpp"

#endif
