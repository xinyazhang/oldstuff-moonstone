/*
 * sql_stmt:
 * 	it's a simple forwarding class,
 * 	provide convience for reference counting
 *
 * p.s. As it's a forwarding class, it means
 * 	the interface is the same as sql_stmt_interface
 */


#ifndef KERNEL_SQL_STMT_H
#define KERNEL_SQL_STMT_H

#include <boost/shared_ptr.hpp>
#include <pal/stdtype.h>

class sql_stmt_interface;

class EXPORT_TO_DLL sql_stmt
{
public:
	sql_stmt(sql_stmt_interface* );

	void bind(int c);//bind void;
	void bind(int c, idx_t );
	void bind(int c, uint64_t );
	void bind(int c, const unistr&);

	int execute();

	bool step();
	void col(int c, unistr& );
	void col(int c, idx_t& );
	void col(int c, uint64_t& );

	void clear();
private:
	boost::shared_ptr<sql_stmt_interface> internal_;
};

#endif
