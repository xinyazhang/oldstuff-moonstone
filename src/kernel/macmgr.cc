#include "macmgr.h"
#include "Database.h"
#include "access_point.h"
#include "machine.h"


macmgr_t::macmgr_t(class Database* mgr)
	:dbmgr_(mgr)
{
}

void macmgr_t::add_machine(machine_t& mac)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("INSERT INTO ")\
		UT("machine_list(name, comment) VALUES($1, $2);"));
	stmt.bind(1, mac.name);
	stmt.bind(2, mac.comment);
	if (stmt.execute()) {
		mac.id = db_->last_serial();
	}
}

 /* would change machine_t::id */
void macmgr_t::update_machine(const machine_t& mac)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("UPDATE machine_list SET name=$1, comment=$2")\
		UT("WHERE id=$3;"));
	stmt.bind(1, mac.name);
	stmt.bind(2, mac.comment);
	stmt.bind(3, mac.id);
	
	stmt.execute();
}

void macmgr_t::rm_machine(const machine_t& mac)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("DELETE FROM machine_list")\
		UT("WHERE id=$1;"));
	stmt.bind(1, mac.id);
	stmt.execute();
}


void macmgr_t::add_ap(access_point& ap)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("INSERT INTO ")\
		UT("access_point(host, url, comment) VALUES($1, $2, $3);"));
	stmt.bind(1, ap.host->id);
	stmt.bind(2, ap.host->url);
	stmt.bind(3, ap.host->comment);
	if (stmt.execute()) {
		ap.id = db_->last_serial();
	}
}

void macmgr_t::update_ap(const access_point& ap)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("UPDATE access_point SET url=$1, comment=$2")\
		UT("WHERE id=$3;"));
	stmt.bind(1, ap.url);
	stmt.bind(2, ap.comment);
	stmt.bind(3, ap.id);
	
	stmt.execute();
}

void macmgr_t::rm_ap(const access_point& ap)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("DELETE FROM access_point")\
		UT("WHERE id=$1;"));
	stmt.bind(1, ap.id);
	stmt.execute();
}

machine_t macmgr_t::macfirst()
{
	machine_t mac;
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("SELECT * FROM machine_list"));
	if (stmt.step()) {
		stmt.col(1, mac.id);
		stmt.col(2, mac.name);
		stmt.col(3, mac.comment);
	}
	return mac;
}

machine_t macmgr_t::macthis(idx_t id)
{
	machine_t mac;
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("SELECT * FROM machine_list WHERE id=$1"));
	stmt.bind(1, id);
	if (stmt.step()) {
		stmt.col(1, mac.id);
		stmt.col(2, mac.name);
		stmt.col(3, mac.comment);
	}
	return mac;
}

machine_t macmgr_t::macnext(const machint_t& m)
{
	machine_t mac;
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("SELECT * FROM machine_list WHERE id > $1 ORDER BY id ASC"));
	stmt.bind(1, m.id);
	if (stmt.step()) {
		stmt.col(1, mac.id);
		stmt.col(2, mac.name);
		stmt.col(3, mac.comment);
	}
	return mac;
}

access_point macmgr_t::apfirst()
{
	access_point ap;
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("SELECT * FROM access_point ORDER BY id ASC"));
	if (stmt.step()) {
		stmt.col(1, ap.id);
		stmt.col(2, ap.host);
		stmt.col(3, ap.url);
		stmt.col(4, ap.comment);
	}
	return ap;
}

access_point macmgr_t::apthis(idx_t id)
{
	access_point ap;
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("SELECT * FROM access_point WHERE id=$1"));
	stmt.bind(1, id);
	if (stmt.step()) {
		stmt.col(1, ap.id);
		stmt.col(2, ap.host);
		stmt.col(3, ap.url);
		stmt.col(4, ap.comment);
	}
	return ap;
}

access_point macmgr_t::apnext(const access_point&)
{
	access_point ap;
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("SELECT * FROM access_point WHERE id>$1 ORDER BY id ASC"));
	stmt.bind(1, id);
	if (stmt.step()) {
		stmt.col(1, ap.id);
		stmt.col(2, ap.host);
		stmt.col(3, ap.url);
		stmt.col(4, ap.comment);
	}
}
