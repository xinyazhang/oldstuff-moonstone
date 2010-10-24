#include <iostream>
#include <assert.h>
#include <kernel/common.h>
#include <kernel/fso.h>
#include <string>
#include <dal/db_sqlite_impl.h>

using namespace std;

struct act_t
{
	string cmd;
	string para;
} act;

fso_t global_fso;
Database* global_db = NULL;
DatabaseInterface* global_db_core = NULL;

void load_database()
{
	global_db_core = new db_sqlite_impl("Lain.db");
	assert(global_db_core->connect() == true);
	global_db = new Database(global_db_core);
	if ( !global_db->initialized() )
		assert(global_db->initialize() == true);

	global_fso = global_db->fsodbman()->locate(0);
}

act_t readline()
{
	act_t ret;
	cin >> ret.cmd;
	getline(cin, ret.para);
	return ret;
}

int exec_cd(Database* db, fso_t& fso, const string& name);
int exec_ls(Database* db, fso_t& fso);
int exec_add(Database* db, const string& name);
int exec_rm(Database* db, const string& name);

int main(int argc, const char* argv)
{
	load_database();
	do
	{
		act = readline();

		if ( act.cmd == "exit" )
			break;
		else if ( act.cmd == "cd" )
			exec_cd(global_db, global_fso, act.para);
		else if ( act.cmd == "ls" )
			exec_ls(global_db, global_fso);
		else if ( act.cmd == "add" )
			exec_add(global_db, act.para);
		else if ( act.cmd == "rm" )
			exec_rm(global_db, act.para);

	} while ( true );
	return 0;
}

void printfso(const fso_t& fso)
{
	cout << "File system object No. "<< fso.fsoid_ << endl;
	cout << "File name: " << fso.name_.native() << endl;
	cout << "Full path: " << fso.path_.native() << endl;
	cout << "Recorded size: " << fso.size_ << endl;
	cout << "Recorded modification time: " << fso.mtime_ << endl;
	cout << "Recorded recursive modification time: " << fso.mtimer_<< endl;
	cout << "Hash algorithm: " << fso.hash_algo_ << endl;
	cout << endl;
}

int exec_cd(Database* db, fso_t& fso, const string& name)
{
	if ( !db->fsodbman()->fsocd(fso, unistr(name.c_str()))  )
	{
		cerr << "cd: "<< name <<"123: No such file or directory"<<endl;
	}
	return 0;
}

int exec_ls(Database* db, fso_t& fso)
{
	fso_t fsoiter = db->fsodbman()->fsocontent(fso);
	fsoiter.path_ = db->fsodbman()->fullpath(fso.fsoid_);
	printfso(fsoiter);
	while ( db->fsodbman()->fsonext(fsoiter) )
	{
		printfso(fsoiter);
	}
	return 0;
}

int exec_add(Database* db, const string& name)
{
	db->fsodbman()->add_fso(unistr(name.c_str()), global_fso.fsoid());
	return 0;
}

int exec_rm(Database* db, const string& name)
{
	idx_t fsoid = db->fsodbman()->locate(unistr(name.c_str()), global_fso.fsoid());
	if ( db->fsodbman()->haschild(fsoid) )
		cerr << name << " is not empty "<<endl;
	else
		db->fsodbman()->rm(fsoid);
	return 0;
}
