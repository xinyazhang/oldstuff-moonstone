#include "../core/unicode.h"
#include "../core/UniStr.h"
#include <stdio.h>
#include "remus_cmd.h"

int umain(int argc, uchar* argv[])
{
	if ( argc < 2 )
	{
		printf("Try 'remus help' for usage.\n");
		retrun 0;
	}
	// phase commands
	
	UniStr command(argv[1]);

	if (command == "help")
	{
		remus_help(argc, argv);
	} else if (command == "add")
	{
		remus_add(argc, argv);
	} else if (command == "remove" || command == "rm")
	{
		remus_rm(argc, argv);
	} else if (command == "edit")
	{
		remus_edit(argc, argv);
	} else if (command == "ls")
	{
		remus_ls(argc, argv);
	} else if (command == "tag")
	{
		remus_add(argc, argv);
	} else if (command == "attrib")
	{
		remus_add(argc, argv);
	} else
	{
		printf("Unknow command: '%s'\n", argv[1]);
		printf("Try 'remus help' for usage.\n");
		return 0;
	}
}
