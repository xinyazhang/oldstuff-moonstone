#include "volumemodel.h"
#include "Preferences.h"
#include <kernel/Database.h>
#include <kernel/volmgr.h>
#include <kernel/index_engine.h>
#include <pal/volume.h>

VolumeModel::VolumeModel(Preferences* preference)
    :pref(preference)
{
	vol_list = pref->db_mgr->volmgr()->known_volumes();
}

VolumeModel::~VolumeModel()
{
}

QVariant VolumeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::CheckStateRole && index.column() == 0) {
		int64_t st = vol_list[index.row()].status;
		if (st & (VOL_UI_STILL_TRACING))
			return QVariant(Qt::Checked);
		else
			return QVariant(Qt::Unchecked);
	}

	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if (index.column() == 1)
		return uuid2unistr(vol_list[index.row()].uuid);
	else if (index.column() == 0) {
		if (!vol_list[index.row()].mount_points.empty()) {
			return vol_list[index.row()].mount_points.front();
		} 
	} else if (index.column() == 2) {
		unistr st;
		const volume& vol(vol_list[index.row()]);
		if (vol.status & VOL_ONLINE)
			st += UT("ONLINE");
		if (vol.status & VOL_TRACING)
			st += UT(" TRACING");
		if (!!(vol.status & VOL_TRACING) != !!(vol.status & VOL_UI_STILL_TRACING)) {
			if (vol.status & VOL_UI_STILL_TRACING)
				st += UT(" Will be tracing");
			else
				st += UT(" Will not be tracing");
		}
		return st;
	}
	return QVariant();
}

bool VolumeModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (role == Qt::CheckStateRole && index.column() == 0) {
		volume& vol(vol_list[index.row()]);
		bool still = vol.status & VOL_UI_STILL_TRACING;
		SET_BITS(vol.status, VOL_UI_STILL_TRACING, !still);
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

Qt::ItemFlags VolumeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (index.column() == 0)
		flags |= Qt::ItemIsUserCheckable;

	return flags;
}

//const char* headers[] = {"Name", "Path", "Size", "Date Modified"};

QVariant VolumeModel::headerData(int section,
		Qt::Orientation orientation,
		int role) const
{
	return QVariant();
}

QModelIndex VolumeModel::index(int row,
		int column,
		const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if ( row < 0 || row >= vol_list.size() )
		return QModelIndex();

	return createIndex(row, column);
}

QModelIndex VolumeModel::parent(const QModelIndex &index) const
{
	return QModelIndex(); /* No tree structure ... */
}

int VolumeModel::rowCount(const QModelIndex& index) const
{
	if (!index.isValid()) /* Root */
		return (int)vol_list.size();
	else
		return 0;
}

int VolumeModel::columnCount(const QModelIndex&) const
{
	return 3;
}

void VolumeModel::apply_changes()
{
	for(std::vector<volume>::iterator iter = vol_list.begin();
		iter != vol_list.end();
		iter++) {
		volume& vol(*iter);
		if (!!(vol.status & VOL_TRACING) != !!(vol.status & VOL_UI_STILL_TRACING)) {
			if (vol.status & VOL_UI_STILL_TRACING) {
				/* Add */
				pref->indexer->queue_volume(vol);
			} else {
				/* Remove */
				pref->indexer->remove_volume(vol);
			}
			SET_BITS(vol.status, VOL_TRACING, !!(vol.status&VOL_UI_STILL_TRACING));
		}
	}
}

void VolumeModel::clear_changes()
{
	for(std::vector<volume>::iterator iter = vol_list.begin();
		iter != vol_list.end();
		iter++)
	{
		SET_BITS(iter->status, VOL_UI_STILL_TRACING, !!(iter->status & VOL_TRACING));
	}
}
