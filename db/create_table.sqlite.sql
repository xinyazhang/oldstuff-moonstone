create table remus_tag(
	idx INTEGER PRIMARY KEY ASC AUTOINCREMENT,
	name TEXT,
	family INTEGER REFERENCES remus_tagfamily(idx) ON DELETE SET NULL,
	alias INTEGER REFERENCES remus_alias_group(idx) ON DELETE SET NULL,
	prev_allian INTEGER REFERENCES remus_tag(idx) ON DELETE SET NULL,
	post_allian INTEGER REFERENCES remus_tag(idx) ON DELETE SET NULL, -- notice: software shall manually manage this, rather than left them to db.
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
create table remus_attr_family(
	idx INTEGER PRIMARY KEY ASC AUTOINCREMENT,
	name TEXT UNIQUE,
	class INTEGER
);
create table remus_attr(
	family INTEGER REFERENCES remus_attr_family(idx) ON DELETE CASCADE,
	target INTEGER REFERENCES remus_object(idx) ON DELETE CASCADE,
	value TEXT,
	PRIMARY KEY (family, target)
);
create table remus_resource(
	idx INTEGER PRIMARY KEY ASC AUTOINCREMENT,
	location TEXT UNIQUE,
	access_method INTEGER
);
create table remus_tagging(
	tag INTEGER REFERENCES remus_tag(idx) ON DELETE CASCADE,
	obj INTEGER REFERENCES remus_object(idx) ON DELETE CASCADE,
	PRIMARY KEY (tag, obj)
);
