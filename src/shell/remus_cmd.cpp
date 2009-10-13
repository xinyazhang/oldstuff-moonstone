#include "remus_cmd.h"
#include "../DAL/domain.h"
#include "dbshell.h"

static RemusDBShell *db = NULL;
union OpObj
{
	Tag* tag;
	TagFamily* family;
	AttrFamily* attrfamily;
	Resource* res;
	Container* cont;
	TagAttach* tagattach;
	Attr* attrattach;
} target;

#define TAGT 1
#define TAGFAMILYT 2
#define ATTRFAMILYT 3
#define REST 4
#define CONTT 5
#define TAGATTACHT 6
#define ATTRT 7
int target_type;

// phase command line in to query objects
// return 0 while no objects founded
int phase_object(int argc, uchar* argv[]); 

int opendb()
{
	UniStr dblocation;
	db = new RemusDB(dblocation);
}

static const uchar helpstring[]=
		UT("usage: remus <command> <objects> [options] [args]\n")
		UT("Type 'remus help <command>' for help on a specific command\n")
		UT("Available commands:\n")
		UT("\tadd\n")
		UT("\tremove (rm)\n")
		UT("\tedit\n")
		UT("\tls\n")
		UT("\ttag\n")
		UT("\tattrib\n")
		UT("\nAvailable objects\n")
		UT("tag\n")
		UT("tagfamily (tf)\n")
		UT("attribfamily (af)\n")
		UT("resource (res)\n")
		UT("container (cont)\n");

int remus_help(int argc, uchar* argv[])
{
	if ( argc < 3 )
	{
		// "remus help", argc = 2
		wprintf("%s", helpstring);
	}

	printf("懒得挨个写帮助了，另外现在的程序架构编制大量的帮助还是太难了\n自己翻手册(manual)吧，bye~\n");
}

int remus_add(int argc, uchar* argv[])
{
	if (phase_object(argc, argv))
	{
		printf("More arguments are needed.\n");
	}
	opendb();
	if ( target_type == TAGT )
	{
		db->add(target.tag);
	} else if ( target_type == TAGFAMILYT )
	{
		db->add(target.family);
	} else if ( target_type == ATTRFAMILYT )
	{
		db->add(target.attrfamily);
	} else if ( target_type == REST )
	{
		db->add(target.res);
	} else if ( target_type == CONTT )
	{
		db->add(target.cont);
	} else if ( target_type == TAGATTACHT )
	{
		db->add(target.tagattach);
	} else if ( target_type == ATTRT )
	{
		db->add(target.attrattach);
	}
}

int remus_rm(int argc, uchar* argv[])
{
	if (phase_object(argc, argv))
	{
		printf("More arguments are needed.\n");
	}
	opendb();
	if ( target_type == TAGT )
	{
		db->rm(target.tag);
	} else if ( target_type == TAGFAMILYT )
	{
		db->rm(target.family);
	} else if ( target_type == ATTRFAMILYT )
	{
		db->rm(target.attrfamily);
	} else if ( target_type == REST )
	{
		db->rm(target.res);
	} else if ( target_type == CONTT )
	{
		db->rm(target.cont);
	} else if ( target_type == TAGATTACHT )
	{
		db->rm(target.tagattach);
	} else if ( target_type == ATTRT )
	{
		db->rm(target.attrattach);
	}
}

int remus_edit(int argc, uchar* argv[])
{
	if (phase_object(argc, argv))
	{
		printf("More arguments are needed.\n");
	}
	opendb();
	if ( target_type == TAGT )
	{
		db->edit(target.tag);
	} else if ( target_type == TAGFAMILYT )
	{
		db->edit(target.family);
	} else if ( target_type == ATTRFAMILYT )
	{
		db->edit(target.attrfamily);
	} else if ( target_type == REST )
	{
		db->edit(target.res);
	} else if ( target_type == CONTT )
	{
		db->edit(target.cont);
	} else if ( target_type == TAGATTACHT )
	{
		db->edit(target.tagattach);
	} else if ( target_type == ATTRT )
	{
		db->edit(target.attrattach);
	}
}

int remus_ls(int argc, uchar* argv[])
{
	if (phase_object(argc, argv))
	{
	}
	opendb();
	if ( target_type == TAGT )
	{
		db->ls(target.tag);
	} else if ( target_type == TAGFAMILYT )
	{
		db->ls(target.family);
	} else if ( target_type == ATTRFAMILYT )
	{
		db->ls(target.attrfamily);
	} else if ( target_type == REST )
	{
		db->ls(target.res);
	} else if ( target_type == CONTT )
	{
		db->ls(target.cont);
	} else if ( target_type == TAGATTACHT )
	{
		db->ls(target.tagattach);
	} else if ( target_type == ATTRT )
	{
		db->ls(target.attrattach);
	}
}

/*
int remus_tag(int argc, uchar* argv[])
{
	if (phase_object(argc, argv))
	{
		printf("More arguments are needed.\n");
	}
	opendb();
	if ( target_type == TAGT )
	{
		db->tag(target.tag);
	} else if ( target_type == TAGFAMILYT )
	{
		db->tag(target.family);
	} else if ( target_type == ATTRFAMILYT )
	{
		db->tag(target.attrfamily);
	} else if ( target_type == REST )
	{
		db->tag(target.res);
	} else if ( target_type == CONTT )
	{
		db->tag(target.cont);
	} else if ( target_type == TAGATTACHT )
	{
		db->tag(target.tagattach);
	} else if ( target_type == ATTRT )
	{
		db->tag(target.attrattach);
	}
}

int remus_attr(int argc, uchar* argv[])
{
	if (phase_object(argc, argv))
	{
		printf("More arguments are needed.\n");
	}
}
*/

int phase_object(int argc, uchar* argv[])
{
}
