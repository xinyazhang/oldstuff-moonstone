#include <iostream>
#include <assert.h>
#include <kernel/common.h>
#include <kernel/fso.h>
#include <string>
#include <dal/db_sqlite_impl.h>

using namespace std;

enum env_t
{
	FSO_INTERACTIVE = 0
};

const char* envstring[] =
{
	"fso"
};

env_t current_env = FSO_INTERACTIVE;

bool intractive = true;

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
	while ( ' ' == cin.peek() )
		cin.get();
	getline(cin, ret.para);
	return ret;
}

int exec_cd(Database* db, fso_t& fso, const string& name);
int exec_ls(Database* db, fso_t& fso);
int exec_add(Database* db, const string& name);
int exec_rm(Database* db, const string& name);
int chenv(const string& env);
int snapshot(const char*, bool);
wstring env_reference();

int main(int argc, char* argv[])
{
	load_database();
	if ( argc > 1 && std::string(argv[1]) == "snapshot" )
	{
		intractive = false;
		if ( argc > 2 )
			snapshot(argv[2], true);
		else
			cout << "Usage: "<< argv[0] << " snapshot [FILE]" << endl;

		delete global_db;
		delete global_db_core;
		return 0;
	}
	do
	{
		if (intractive)
		{
			wprintf(L"[%S:%s] ", envstring[current_env], env_reference().c_str());
		}
		act = readline();

		if ( act.cmd == "chenv" )
		{
			chenv(act.para);
			continue;
		}
		switch (current_env)
		{
		case FSO_INTERACTIVE:
			if ( act.cmd == "exit" )
				return 0;
			else if ( act.cmd == "cd" )
				exec_cd(global_db, global_fso, act.para);
			else if ( act.cmd == "ls" )
				exec_ls(global_db, global_fso);
			else if ( act.cmd == "add" )
				exec_add(global_db, act.para);
			else if ( act.cmd == "rm" )
				exec_rm(global_db, act.para);
			else if ( act.cmd == "pwd" )
				wprintf(L"%s%s\n", global_fso.name_.c_str(), global_fso.path_.c_str());
			break;
		default:
			break;
		}

	} while ( true );

	delete global_db_core;
	delete global_db;
	return 0;
}

wstring env_reference()
{
	if ( current_env == FSO_INTERACTIVE )
		return global_fso.name_;
	
	return wstring();
}

int chenv(const string& env)
{
	if ( env == "snapshot" )
	{
		cout << "Error, snapshot don't provide an intractive environment"<<endl;
		cout << "Using shell command snapshot instead"<<endl;
		return 0; 
	}

	// no matching
	cout << "Error, no such environment: "<<env<<endl;
	return 0;
}

int snapshot(const char* path, bool recursive) // no impl. currently
{
	int flag = 0;
	if ( recursive )
		flag = snapshotter::recursive_flag;
	global_db->ss()->add(unistr(path), flag);
	return 0;
}

void printfso(const fso_t& fso)
{
	cout << "File system object No. "<< fso.fsoid_ << endl;
	cout.flush();
	wprintf(L"File name: %s\n", fso.name_.native());
	wprintf(L"Full path: %s\n", fso.path_.native());
	/*
	cout << "File name: " << (wchar_t*)fso.name_.native() << endl;
	cout << "Full path: " << (wchar_t*)fso.path_.native() << endl;
	*/
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
		cerr << "cd: "<< name <<": No such file or directory"<<endl;
	}
	return 0;
}

int exec_ls(Database* db, fso_t& fso)
{
	int64_t total = 0;
	fso_t fsoiter = db->fsodbman()->fsocontent(fso);
	if ( !fsoiter.valid() )
		goto summary;

	fsoiter.path_ = db->fsodbman()->fullpath(fso.fsoid_);
	printfso(fsoiter); total++;
	while ( db->fsodbman()->fsonext(fsoiter) )
	{
		fsoiter.path_ = db->fsodbman()->fullpath(fso.fsoid_);
		printfso(fsoiter); total++;
	}
summary:
	cout<<"Total: "<<total<<endl;
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