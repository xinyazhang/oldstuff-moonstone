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
		 * Create Tag:
		 * 1. Generate a TagParameter object to store parameters 
		 * 2. Call template function "create" of Category object
		 * 3. change the returned tag.
		 * 4. store tag.
		 */
		TagRef tag = cat->create(Tag::Para().arg(tagname).arg_family(tag_family));
		tag->store();
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
		TagRef tag = cat->create(tagname, tagfamily, Category::open_existing);
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
		/*
		 * Set family is a little difficult...
		 */
		FamilyRef family = cat->create_family(tagfamily, Category::open_existing);
		if ( !family )
		{
			printf("New tagfamily do not exist\n");
			cat->kill_tag(tag);
			return 0;
		} else 
		{
			tag->chname(tagname, tagfamily);
		}
	} else if (cmd == UT("list"))
	{
		uprintf(UT("%24s %24s\n"), UT("Tag name"), UT("Family name"));
		uprintf(UT("================================================"));
		Tag::Iterator* iter = cat->create_iterator<Tag>();
		while( !iter->done() )
		{
			TagRef tag = iter->read();
			uprintf(UT("%24s %24s\n"), tag->name(), tag->family()->name());
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
