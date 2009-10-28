#include "../core/UniStr.h"
#include "../core/PlatformSpec.h"
#include "../DAL/DataBase.h"
#include "../DAL/SQLite/SQLite.h"
#include "../reimu/Defines.h"

Database *cat;
Category *cat;

void split(const uchar* p, UniStr* tag, UniStr* family)
{
	UniStr str(p);
	long pos = 0;
	UniStr* op = tag;

	while ( pos < str.size() )
	{
		(*op) += str[pos];
		if ( str[pos] == UT(':') && op == tag )
			op = family;
	}
}

int work(int argc, uchar* argv[])
{
	UniStr cmd(argv[1]);
	if (cmd == UT("create"))
	{
		UniStr tagname, tagfamily;
		split(argv[2], &tagname, &tagfamily);
		/*
		 * There are two methods to create a new tag:
		 * 1. provide tagname and tagfamily and calls create_tag
		 * 	it would create a new tag
		 * 2. call create_tag without any parameter, it would return
		 * 	a pointer point to a temporary tag, change its tagname and
		 * 	tagfamily, and calls close_tag 
		 *
		 * Why there are two methods?
		 * 	the first one uses donmain layer's default action: create all necessary
		 * 	data, that means if the tag family do not exist, the domain layer's logic would 
		 * 	create a new one
		 *
		 * 	However, on some conditions we want to control the creating procedure
		 *	more meticulously, the second method would becoome a better practice.
		 *
		 * btw, kill_tag won't save the changes to a tag.
		 */
		TagRef tag = cat->create_tag(tagname, tagfamily);
		cat->close_tag(tag);
		/*
		 * Example code for the second practice
		 *
		 * These codes won't create a tag for a non-existing family
		 */
#ifdef _SHOULD_NOT_BE_DEFINED_
		TagRef tag = cat->create_tag();
		tag->set_name(tagname);
		/* 
		 * create a family
		 */
		TagFamily* family = cat->create_family(tagfamily, cat::open_existing);
		if ( !family )
			cat->kill_tag(tag);
		else
		{
			tag->set_family(family);
			cat->close_tag(tag);
		}
#endif
	} else if (cmd == UT("rm"))
	{
		UniStr tagname, tagfamily;
		split(argv[2], &tagname, &tagfamily);

		cat->rm_tag(tagname, tagfamily);
	} else if (cmd == UT("ren"))
	{
		if (argc != 4)
		{
			printf("invalid parameter\n");
			return 0;
		}
		UniStr tagname, tagfamily;
		split(argv[2], &tagname, &tagfamily);

		/*
		 * We wanner to rename a tag, but we don't want to rename 
		 * a non existing tag, using open_existing flag to estimate to create
		 * a new tag
		 */
		TagRef tag = cat->create_tag(tagname, tagfamily, DataBase::open_existing);
		if ( !tag )
		{
			printf("such tag do not exist\n");
			return 0;
		}
		/* 
		 * Notes on split's action:
		 * if user don't provice a tagfamily, it shouldn't modify tagfamiy
		 */
		split(argv[3], &tagname, &tagfamily);
		tag->set_name(tagname);
#if _PROTOTYPE_ > 2
		/*
		 * Set family is a little difficult...
		 */
		FamilyRef family = cat->create_family(tagfamily, cat::open_existing);
		if ( !family )
		{
			printf("New tagfamily do not exist\n");
			cat->kill_tag(tag);
			return 0;
		} else 
		{
			tag->set_family(tagfamily);
		}
#endif
		cat->close_tag(tag);
	} else if (cmd == UT("list"))
	{
		uprintf(UT("%24s %24s\n"), UT("Tag name"), UT("Family name"));
		uprintf(UT("================================================"));
		DataBase::TagIterator* iter = cat->create_tag_iterator();
		TagRef tag = cat->read_tag(iter);
		while( tag )
		{
#if _PROTOTYPE_ > 2
			uprintf(UT("%24s %24s\n"), tag->name(), tag->family()->name());
#else
			uprintf(UT("%s\n"), tag->name());
#endif
			cat->kill_tag(tag);
			tag = cat->read_tag(iter);
		}
	}
	return 0;
}

int umain(int argc, uchar* argv[])
{
	UniStr db_path(ugetenv(SYSTEM_CONFIG_PATH_ENV));
	cat = new SQLite((db_path+REMUS_CONFIG_RELATIVE_PATH).c_str());
	cat->initialize();

	work(argc, argv);

	//cat->close();
	delete cat;
	delete db;
	return 0;
}
/*
 * tag [create|rm|ren|list] <tagname>[:<tagfamily>] [<tagname>[:tagfamily]]
 */
