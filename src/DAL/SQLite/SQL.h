const uchar* sql_unicode = 
UT("PRAGMA encoding = \"UTF-16\";"); 

const uchar* sql_create_tag_table = 
UT("create table IF NOT EXISTS remus_tag(")
UT("idx INTEGER PRIMARY KEY ASC AUTOINCREMENT,")
UT("name TEXT,")
UT("family INTEGER REFERENCES remus_family(idx) ON DELETE SET NULL,")
UT("alias INTEGER REFERENCES remus_alias_group(idx) ON DELETE SET NULL,")
UT("prev_allian INTEGER REFERENCES remus_tag(idx) ON DELETE SET NULL,")
UT("post_allian INTEGER REFERENCES remus_tag(idx) ON DELETE SET NULL,")
UT("UNIQUE (idx, family)")
UT(");");

const uchar* sql_initial_tag_table = 
UT("INSERT INTO remus_tag(idx) values(0);");

const uchar* sql_create_alias =
UT("create table IF NOT EXISTS remus_alias_group(")
UT("idx INTEGER PRIMARY KEY ASC AUTOINCREMENT,")
UT("master INTEGER REFERENCES remus_tag(idx) ON DELETE SET NULL")
UT(");");

const uchar* sql_initial_alias = 
UT("INSERT INTO remus_alias_group(idx) values(0);");

const uchar* sql_create_family = 
UT("create table remus_family(")
UT("idx INTEGER PRIMARY KEY ASC AUTOINCREMENT,")
UT("name TEXT UNIQUE")
UT(");");

const uchar* sql_initial_family =
UT("INSERT INTO remus_family(idx) values(0);");
