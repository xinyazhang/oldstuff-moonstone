#include "../core/UniStr.h"
#include "../core/PlatformSpec.h"
#include "../DAL/domain.h"
#include "../DAL/DataBase.h"
#include "../DAL/SQLite/DataBase.h"

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
		 * p.s. if one wanner a temporary tag, call the db's factory class,
		 * 	use: DataBase::factory to grain a pointer to a factory singleton.
		 *
		 * btw, kill_tag won't save the changes to a tag.
		 */
		Tag* tag = db->create_tag(tagname, tagfamily);
		db->close_tag(tag);
		/*
		 * Example code for the second practice
		 *
		 * These codes won't create a tag for a non-existing family
		 */
#ifdef _SHOULD_NOT_BE_DEFINED_
		Tag* tag = db->create_tag();
		tag->set_name(tagname);
		/* 
		 * create a family
		 */
		TagFamily* family = db->create_family(tagfamily, db::open_existing);
		if ( !family )
			db->kill_tag(tag);
		else
		{
			tag->set_family(family);
			db->close_tag(tag);
		}
#endif
	} else if (cmd == UT("rm"))
	{
		UniStr tagname, tagfamily;
		split(argv[2], &tagname, &tagfamily);

		db->rm_tag(tagname, tagfamily);
	} else if (cmd == UT("ren"))
	{
		if (argv != 4)
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
		Tag* tag = db->create_tag(tagname, tagfamily, db::open_existing);
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
		/*
		 * Set family is a little difficult...
		 */
		TagFamily* family = db->create_family(tagfamily, db::open_existing);
		if ( !family )
		{
			printf("New tagfamily do not exist\n");
			db->kill_tag(tag);
			return 0;
		} else 
		{
			tag->set_family(tagfamily);
		}
		db->close_tag(tag);
	} else if (cmd == UT("list"))
	{
		uprintf(UT("%24s %24s\n"), UT("Tag name"), UT("Family name"));
		uprintf(UT("================================================"));
		DataBase::TagIterator* iter = db->create_tag_iterator();
		Tag* tag = db->read_tag(iter);
		while( tag )
		{
			uprintf(UT("%24s %24s\n"), tag->name(), tag->family()->name());
			db->kill_tag(tag);
			tag = db->read_tag(iter);
		}
	}
	return 0;
}

int umain(int argc, uchar* argv[])
{
	UniStr db_path(ugetenv(SYSTEM_CONFIG_PATH_ENV));
	DataBase *db = new SQLiteDB(db_path+REMUS_CONFIG_RELATIVE_PATH);
	db->initialize();

	work(argc, argv);

	db->close();
	delete db;
	return 0;
}
/*
 * tag [create|rm|ren|list] <tagname>[:<tagfamily>] [<tagname>[:tagfamily]]
 */
/*
create table remus_tag(
	idx INTEGER PRIMARY KEY ASC AUTOINCREMENT,
	name TEXT,
	family INTEGER REFERENCES remus_tagfamily(idx) ON DELETE SET NULL,
	alias INTEGER REFERENCES remus_alias_group(idx) ON DELETE SET NULL,
	prev_alian INTEGER REFERENCES remus_tag(idx) ON DELETE SET NULL,
	post_alian INTEGER REFERENCES remus_tag(idx) ON DELETE SET NULL, -- notice: software shall manually manage this, rather than left them to db.
	UNIQUE (idx, family)
);
create table remus_alias_group(
	idx INTEGER PRIMARY KEY ASC AUTOINCREMENT,
	master INTEGER REFERENCES remus_tag(idx) ON DELETE SET NULL
);
create table remus_family(
	idx INTEGER PRIMARY KEY ASC AUTOINCREMENT,
	name TEXT UNIQUE
);
*/
