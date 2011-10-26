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

		known.push_back(vol);
	}

	{
		std::vector<volume> sys = ls_volume();
		known.insert(known.end(), sys.begin(), sys.end());
	}

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

void volmgr_t::update_lastjid(int64_t kpi, uint64_t jid)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("UPDATE known_ntfs SET journal_id=$1 WHERE id=$2;"));
	stmt.bind(1, jid);
	stmt.bind(2, kpi);
	stmt.execute();
}

void volmgr_t::update_ntfsext(int64_t kpi, uint64_t jid, uint64_t usn)
{
	sql_stmt stmt = dbmgr_->create_stmt_ex(
			UT("UPDATE known_ntfs SET journal_id=$1, usn_pointer=$2 WHERE id=$3;"));
	stmt.bind(1, jid);
	stmt.bind(2, usn);
	stmt.bind(3, kpi);
	stmt.execute();
}
