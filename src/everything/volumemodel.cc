#include "volumemodel.h"

VolumeModel::VolumeModel(Preferences* preference)
    :pref(preference)
{
	vol_list = ls_volume();
	watching_list = pref->indexer->volume_list();
}

VolumeModel::~VolumeModel()
{
}

QVariant VolumeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	return uuid2unistr(vol_list[index.row()].uuid);
}

Qt::ItemFlags VolumeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

//const char* headers[] = {"Name", "Path", "Size", "Date Modified"};

QVariant VolumeModel::headerData(int section,
		Qt::Orientation orientation,
		int role = Qt::DisplayRole) const
{
#if 0
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return headers[section];
#endif

	return QVariant();
}

QModelIndex VolumeModel::index(int row,
		int column,
		const QModelIndex &parent = QModelIndex()) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if ( row < 0 || row >= pref->search_engine->line_count(se_cookie) )
		return QModelIndex();

	return createIndex(row, column);
}

QModelIndex VolumeModel::parent(const QModelIndex &index) const
{
	return QModelIndex(); /* No tree structure ... */
}

int VolumeModel::rowCount(const QModelIndex &) const
{
	return vol_list.size();
}

int VolumeModel::columnCount(const QModelIndex&) const
{
	return 1;
}
