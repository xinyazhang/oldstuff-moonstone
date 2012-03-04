#include "volumemodel.h"
#include "Preferences.h"
#include "machinemodel.h"
#include <kernel/Database.h>
#include <kernel/volmgr.h>
#include <kernel/index_engine.h>
#include <pal/volume.h>

MachineModel::MachineModel()
{
#if 0
	vol_list = pref()->db_mgr->volmgr()->known_volumes();
	for(std::vector<volume>::iterator iter = vol_list.begin();
		iter != vol_list.end();
		iter++)	{
			if (iter->status & VOL_TRACING) {
				pref()->indexer->queue_volume(*iter);
				FLAG_BITS(iter->status, VOL_UI_STILL_TRACING);
			}
	}
#endif
	rootItem = new MachineRoot;
}

MachineModel::~MachineModel()
{
	delete rootItem;
}

QVariant MachineModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	TreeItem* item = getItem(index);

	return item->data(index.column(), role);

#if 0
	====MOVE TO VolItem====
	/* Volume state */
	if (role == Qt::CheckStateRole && index.column() == 0) {
		int64_t st = vol_list[index.row()].status;
		if (st & (VOL_UI_STILL_TRACING))
			return QVariant(Qt::Checked);
		else
			return QVariant(Qt::Unchecked);
	}

	====Move To TreeItem as default====
	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	====Move To VolItem====
	if (index.column() == 2)
		return uuid2unistr(vol_list[index.row()].uuid);
	else if (index.column() == 0) {
		if (!vol_list[index.row()].mount_points.empty()) {
			return vol_list[index.row()].mount_points.front();
		} 
	} else if (index.column() == 3) {
		unistr st;
		const volume& vol(vol_list[index.row()]);
		if (vol.kpi > 0)
			st += UT("KNOWN");
		if (vol.status & VOL_ONLINE)
			st += UT(" ONLINE");
		if (vol.status & VOL_TRACING)
			st += UT(" TRACING");
		if (!!(vol.status & VOL_TRACING) != !!(vol.status & VOL_UI_STILL_TRACING)) {
			if (vol.status & VOL_UI_STILL_TRACING)
				st += UT(" Will be tracing");
			else
				st += UT(" Will not be tracing");
		}
		return st;
	} else if (index.column() == 1) {
		return vol_list[index.row()].label;
	}
	return QVariant();
#endif
}

TreeItem* MachineModel::getItem(const QModelIndex& index) const
{
	if (index.isValid()) {
		TreeItem * item = static_cast<TreeItem*>(index.internalPointer());
		if (item)
			return item;
	}
	return rootItem;
}

bool MachineModel::setData(const QModelIndex & index, const QVariant & value, int role)
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

QVariant MachineModel::headerData(int section,
		Qt::Orientation orientation,
		int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);
	return QVariant();
}

QModelIndex MachineModel::index(int row, int column,
		const QModelIndex &parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();

	TreeItem* parentItem = getItem(parent);
	TreeItem* childItem = parentItem->child(row);

	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}


bool MachineModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginInsertColumns(parent, position, position + columns - 1);
	success = rootItem->insertColumns(position, columns);
	endInsertColumns();

	return success;
}

bool MachineModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows, rootItem->columnCount());
	endInsertRows();

	return success;
}

QModelIndex MachineModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = getItem(index);
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool MachineModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginRemoveColumns(parent, position, position + columns - 1);
	success = rootItem->removeColumns(position, columns);
	endRemoveColumns();

	if (rootItem->columnCount() == 0)
		removeRows(0, rowCount());

	return success;
}

bool MachineModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

int MachineModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem = getItem(parent);

	return parentItem->childCount();
}

bool MachineModel::setData(const QModelIndex &index, const QVariant &value,
		int role)
{
	if (role != Qt::EditRole)
		return false;

	TreeItem *item = getItem(index);
	bool result = item->setData(index.column(), value);

	if (result)
		emit dataChanged(index, index);

	return result;
}

bool MachineModel::setHeaderData(int section, Qt::Orientation orientation,
		const QVariant &value, int role)
{
	if (role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;

	bool result = rootItem->setData(section, value);

	if (result)
		emit headerDataChanged(orientation, section, section);

	return result;
}

Qt::ItemFlags MachineModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	if (index.column() == 0)
		flags |= Qt::ItemIsUserCheckable;

	return flags;
}

void MachineModel::apply_changes()
{
	for(std::vector<volume>::iterator iter = vol_list.begin();
		iter != vol_list.end();
		iter++) {
		volume& vol(*iter);
		if (!!(vol.status & VOL_TRACING) != !!(vol.status & VOL_UI_STILL_TRACING)) {
			SET_BITS(vol.status, VOL_TRACING, !!(vol.status&VOL_UI_STILL_TRACING));
			if (vol.status & VOL_UI_STILL_TRACING) {
				/* Add */
				pref()->indexer->queue_volume(vol);
				pref()->db_mgr->volmgr()->update(vol);
			} else {
				/* Remove */
				pref()->indexer->remove_volume(vol);
			}
		}
	}
}

void MachineModel::clear_changes()
{
	for(std::vector<volume>::iterator iter = vol_list.begin();
		iter != vol_list.end();
		iter++)
	{
		SET_BITS(iter->status, VOL_UI_STILL_TRACING, !!(iter->status & VOL_TRACING));
	}
}
