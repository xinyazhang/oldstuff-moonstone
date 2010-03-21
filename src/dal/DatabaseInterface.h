#ifndef DATABASE_ABSTRACT_LAYER_DATABASE_INTERFACE_H
#define DATABASE_ABSTRACT_LAYER_DATABASE_INTERFACE_H

#include <boost/thread/mutex.hpp>

class sql_stmt_interface;
class unistr;

class DatabaseInterface
{
public:
	DatabaseInterface(const unistr& prefix);
	virtual void begin_transaction();
	virtual void final_transaction();
	virtual sql_stmt_interface* create_stmt(const unistr& ) = 0;
	bool initialized() const;
	virtual unistr initialize_sqls(int ) const;
	virtual int initialize_sql_number() const;
	virtual void complete_initialize();
private:
	boost::mutex db_lock_;
	unistr prefix_;
	bool initialized_;
};

#endif
