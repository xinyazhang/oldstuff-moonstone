#ifndef DATABASE_ABSTRACT_LAYER_DATABASE_INTERFACE_H
#define DATABASE_ABSTRACT_LAYER_DATABASE_INTERFACE_H

#include <boost/thread/mutex.hpp>
#include "../pal/stdtype.h"

class sql_stmt_interface;

class EXPORT_TO_DLL DatabaseInterface
{
public:
	DatabaseInterface(const unistr& prefix);
	virtual ~DatabaseInterface();

	virtual bool connect() = 0;
	virtual void begin_transaction() = 0;
	virtual void final_transaction() = 0;
	virtual void abort_transaction() = 0;

	virtual sql_stmt_interface* create_stmt(const unistr& ) = 0;
	bool initialized() const { return initialized_; } 
	virtual unistr initialize_sqls(int ) const = 0;
	virtual int initialize_sql_number() const = 0;
	virtual void complete_initialize() = 0;
	virtual idx_t last_insert() = 0;
	void quick_sql_execute(const unistr& );
	unistr prefix() const { return prefix_; }
protected:
	boost::mutex db_lock_;
	unistr prefix_;
	bool initialized_;
};

#endif
