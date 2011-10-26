#include "filemgr.h"
#include "Database.h"
#include "sql_stmt.h"
#include "dentry.h"

filemgr_t::filemgr_t(Database* mgr)
	:dbmgr_(mgr)
{
}


void filemgr_t::checkbegin(int64_t kpi)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("UPDATE known_dentry SET checked=0 WHERE volid=$1;"));
	stmt.bind(1, kpi);
	stmt.execute();

	stmt = dbmgr_->create_stmt_ex(
			UT("UPDATE known_file SET refc=0 WHERE volid=$1;"));
	stmt.bind(1, kpi);
	stmt.execute();
}

void filemgr_t::witness(const dentry_t& dentry)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("UPDATE OR IGNORE known_dentry SET checked=1 WHERE volid=$1 AND inode=$2;"));
	stmt.bind(1, dentry.kpi);
	stmt.bind(2, dentry.inode);
	stmt.execute();
}

void filemgr_t::blobchange(const dentry_t&)
{
	/* TO DO */
}

void filemgr_t::ack(const dentry_t& dentry)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("INSERT OR IGNORE INTO known_dentry VALUES($1, $2, $3, $4, 0);"));
	stmt.bind(1, dentry.kpi);
	stmt.bind(2, dentry.inode);
	stmt.bind(3, dentry.pinode);
	stmt.bind(4, dentry.fname);
	stmt.execute();

	stmt = dbmgr_->create_stmt_ex(
			UT("INSERT INTO known_file VALUES($1, $2, 1);"));
	stmt.bind(1, dentry.kpi);
	stmt.bind(2, dentry.inode);
	if (!stmt.execute()) {
		/* Insertion of new inode failed, update existence inode */
		stmt = dbmgr_->create_stmt_ex(
				UT("UPDATE known_file SET refc = refc + 1 WHERE volid = $1 AND inode = $2;"));
		stmt.bind(1, dentry.kpi);
		stmt.bind(2, dentry.inode);
		stmt.execute();
	}
}

void filemgr_t::nak(const dentry_t& dentry)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("DELETE FROM known_dentry WHERE volid = $1 AND inode = $2;"));
	stmt.bind(1, dentry.kpi);
	stmt.bind(2, dentry.inode);
	stmt.execute();

	stmt = dbmgr_->create_stmt_ex(
			UT("UPDATE known_file SET refc=refc-1 WHERE volid = $1 AND inode = $2;"));
	stmt.bind(1, dentry.kpi);
	stmt.bind(2, dentry.inode);
	stmt.execute();

	stmt = dbmgr_->create_stmt_ex(
			UT("DELETE FROM known_file WHERE volid = $1 AND inode = $2 AND refc=0;"));
	stmt.bind(1, dentry.kpi);
	stmt.bind(2, dentry.inode);
	stmt.execute();
}

void filemgr_t::existance_flip(const dentry_t& dentry)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("SELECT * FROM known_dentry WHERE volid=$1 AND inode=$2"));
	stmt.bind(1, dentry.kpi);
	stmt.bind(2, dentry.inode);
	if (stmt.step()) {
		/* exist, then nak it */
		nak(dentry);
	} else {
		ack(dentry);
	}
}

void filemgr_t::rename(const dentry_t& dentry)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("UPDATE known_dentry SET name=$1 WHERE volid=$1 AND inode=$2;"));
	stmt.bind(1, dentry.fname);
	stmt.bind(2, dentry.kpi);
	stmt.bind(3, dentry.inode);
	stmt.execute();
}

void filemgr_t::symlinkchange(const dentry_t&)
{
	/* TO DO */
}

void filemgr_t::checkdone(int64_t kpi)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("DELETE FROM known_dentry WHERE volid=$1 AND checked=0;"));
	stmt.bind(1, kpi);
	stmt.execute();
	stmt = dbmgr_->create_stmt_ex(
			UT("DELETE FROM known_file WHERE volid=$1 AND refc=0;"));
	stmt.bind(1, kpi);
	stmt.execute();
}
