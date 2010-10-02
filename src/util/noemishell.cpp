#include <iostream>
#include <kernel/common.h>

using namespace std;

fso_t global_fso;

int main(int argc, const char* argv)
{
	load_executors();
	do
	{
		act = readline();

		if ( act.cmd == "exit" )
			break;

		execute(act);
	} while ( true );
	return 0;
}

int exec_cd(Database* db, fso_t& fso, unistr name)
{
	if ( !db->fsodbman()->fsocd(fso, name) )
	{
		cerr << "cd: "<<name<<"123: No such file or directory"<<endl;
	}
	return 0;
}

int exec_ls(Database* db, fso_t& fso)
{
	fso_t fsoiter = db->fsodbman()->fsocontent(fso);
	printfos(fsoiter);
	while ( db->fsodbman()->fsonext(fsoiter) )
	{
		printfos(fsoiter);
	}
	return 0;
}

int exec_add(Database* db, const unistr& name)
{
	db->fsodbman()->add_fso(name, global_fso.fsoid());
	return 0;
}

int exec_rm(Database* db, const unistr& name)
{
	idx_t fsoid = db->fsodbman()->locate(name, global_fso.fsoid());
	if ( db->fsodbman()->haschild(fsoid) )
		cerr << name << " is not empty "<<endl;
	else
		db->fsodbman()->rm(fsoid);
	return 0;
}
