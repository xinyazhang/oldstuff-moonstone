/*
 * Database class
 * note: 
 * 	this class is used as intermediate layer between real Database's interfaces 
 * 	and kernel operators
 */

#ifndef KERNEL_DATABASE_H
#define KERNEL_DATABASE_H

#include "common_declare.h"
#include <vector>
#include <boost/thread/mutex.hpp>
class DatabaseInterface;

class EXPORT_TO_DLL Database
{
public:
	Database(DatabaseInterface*);
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
	sql_stmt create_insert_stmt(Database::TableSelector, int col_number);
	sql_stmt create_simsel_stmt(TableSelector, const unistr& locator, const unistr& selcontent);
	sql_stmt create_selall_stmt(TableSelector, int locatorn, const char* locators[]);
	/* restrictions must contain comparision operators selves! */
	sql_stmt create_list_stmt(TableSelector, 
			int restrictn, const char* restrictions[],
			int ordern, const char* order_cols[], const bool asc[]);
	sql_stmt create_update_stmt(TableSelector, const unistr& locator, int updatecoln, const char* updatecols[]);
	sql_stmt create_del_stmt(TableSelector, int locatorn, const char* locators[]);

	sql_stmt create_stmt_ex(const unistr& );
	unistr table(TableSelector);
	idx_t last_serial();

	TagMan* tagman();
	TnodeMan* tnodeman();
	RelationMan* relman();
	FsodbMan* fsodbman();
	AsyncWQ* asyncwq();
	snapshotter* ss();
	ftman_t ftman();
private:
	unistr prefix_;
	static const unistr table_name_postfix_[]; // it seems we must use this
	unistr_list table_name_;
	DatabaseInterface *db_;
	TagMan* tagman_;
	TnodeMan* tnodeman_;
	RelationMan* relman_;
	FsodbMan* fsodbman_;
	AsyncWQ* asyncwq_;
	snapshotter* ss_;
	ftman_t* ftman_;

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
