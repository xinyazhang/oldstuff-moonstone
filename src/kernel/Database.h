#ifndef KERNEL_DATABASE_H
#define KERNEL_DATABASE_H

#include <vector>
#include <boost/thread/mutex.hpp>
#include "sql_stmt.h"

class EXPORT_TO_DLL Database
{
public:
	Database(class DatabaseInterface*);
	~Database();

	enum TableSelector
	{
		TnodeTable = 0,
		TagTable = 1,
		TagTagRelationTable = 2,
		FsoTable = 3,
		FtTable = 4
	};
public:
	bool initialized() const;
	bool initialize();

	void begin_transaction();
	void final_transaction();
	void abort_transaction();

	sql_stmt create_stmt_ex(const unistr& );

	class filemgr_t* filemgr();
	class volmgr_t* volmgr();
	class macmgr_t* macmgr();
private:
	class DatabaseInterface* db_;
	class filemgr_t* filemgr_;
	class volmgr_t* volmgr_;

	/*
	 * Transaction
	 */
	int nest_;
	bool breaked_;
	boost::mutex mutex_;
};

class EXPORT_TO_DLL TransactionFinaler
{
public:
	TransactionFinaler(Database* d):d_(d){}
	~TransactionFinaler(){d_->final_transaction();}
private:
	Database* d_;
};

#endif
