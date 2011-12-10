#include "volmgr.h"
#include "Database.h"
#include "sql_stmt.h"

volmgr_t::volmgr_t(Database* dbmgr)
	:dbmgr_(dbmgr)
{
	sql_stmt stmt = dbmgr->create_stmt_ex(UT("SELECT * FROM known_vols;"));
	while (stmt.step()) {
		volume vol;
		unistr voluuid;
		stmt.col(1, vol.kpi);
		stmt.col(2, voluuid);
		vol.uuid = unistr2uuid(voluuid);
		stmt.col(3, vol.status);
		stmt.col(4, vol.filesystem);
		stmt.col(5, vol.label);

		known.push_back(vol);
	}

	append_online_volume(known);

	for (std::vector<volume>::iterator iter = known.begin();
			iter != known.end();
			iter++) {
		detect_mount_points(*iter);
	}
}

std::vector<volume> volmgr_t::known_volumes() const
{
	return known;
}

void volmgr_t::witness(const uuids& uuid, int64_t* kpi)
{
	unistr sql(UT("SELECT * FROM known_vols WHERE uuid=$1;"));
	sql_stmt stmt = dbmgr_->create_stmt_ex(sql);
	stmt.bind(1, uuid2unistr(uuid));
	if (stmt.step()) {
		stmt.col(1, *kpi);
	} else {
		unistr insql(UT("INSERT INTO known_vols(uuid,status,filesystem) VALUES($1,0,0);"));
		sql_stmt instmt = dbmgr_->create_stmt_ex(insql);
		instmt.bind(1, uuid2unistr(uuid));
		if (!instmt.execute()) {
			witness(uuid, kpi);
		} else
			*kpi = -1;
	}
}

void volmgr_t::load_ntfs(int64_t kpi, uint64_t* jid, uint64_t* read_usn)
{
	unistr sql(UT("SELECT * FROM known_ntfs WHERE id=$1;"));
	sql_stmt stmt = dbmgr_->create_stmt_ex(sql);
	stmt.bind(1, kpi);
	if (stmt.step()) {
		stmt.col(2, *jid);
		stmt.col(3, *read_usn);
	} else {
		*jid = 0;
		*read_usn = 0;
		sql_stmt instmt = dbmgr_->create_stmt_ex(
				UT("INSERT INTO known_ntfs VALUES($1, $2, $3);"));
		instmt.bind(1, kpi);
		instmt.bind(2, (int64_t)0);
		instmt.bind(3, (int64_t)0);
		instmt.execute();
	}
}

void volmgr_t::update(const volume& vol)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("UPDATE known_vols SET status=$1, filesystem=$2, vollabel=$4 WHERE id=$3;"));
	stmt.bind(1, MASK_BITS(vol.status, VOL_DB_MASK));
	stmt.bind(2, vol.filesystem);
	stmt.bind(3, vol.kpi);
	stmt.bind(4, vol.label);
	stmt.execute();
}

void volmgr_t::update_lastjid(int64_t kpi, uint64_t jid, uint64_t usn_next)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("UPDATE known_ntfs SET journal_id=$1, usn_next=$3 WHERE id=$2;"));
	stmt.bind(1, jid);
	stmt.bind(2, kpi);
	stmt.bind(3, usn_next);
	stmt.execute();
}

void volmgr_t::update_ntfsext(int64_t kpi, uint64_t jid, uint64_t usn)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("UPDATE known_ntfs SET journal_id=$1, usn_pointer=$2, usn_next=$4 WHERE id=$3;"));
	stmt.bind(1, jid);
	stmt.bind(2, usn);
	stmt.bind(3, kpi);
	stmt.execute();

	stmt = dbmgr_->create_stmt_ex(
			UT("UPDATE known_ntfs SET usn_next=MAX(usn_next, usn_pointer) WHERE id=$3;"));
	stmt.bind(3, kpi);
	stmt.execute();
}

unistr volmgr_t::hrid(int64_t kpi) const
{
	for (std::vector<volume>::const_iterator iter = known.begin();
		iter != known.end();
		iter++) {
		if (iter->kpi == kpi) {
			if (iter->mount_points.empty()) {
				if (iter->label.empty())
					return uuid2unistr(iter->uuid);
				return iter->label;
			} else {
				return iter->mount_points.at(0);
			}
		}
	}
	return unistr();
}

float volmgr_t::progress()
{
	int64_t usn_read, usn_next;	
	int64_t sum_usn_read = 0, sum_usn_next = 0;	
	sql_stmt stmt = dbmgr_->create_stmt_ex(
		UT("SELECT usn_pointer, usn_next FROM known_ntfs;"));
	while (stmt.step())
	{
		stmt.col(1, usn_read);
		stmt.col(2, usn_next);
		sum_usn_read += usn_read;
		sum_usn_next += usn_next;
	}
	return (double)sum_usn_read/(double)sum_usn_next;
}