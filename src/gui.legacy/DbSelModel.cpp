#include "DbSelModel.h"
#include "../kernel/metadb.h"

DbSelModel::DbSelModel(shared_ptr<metadb_t> metadb)
	:metadb_(metadb), selected_(-1)
{
}

DbSelModel::~DbSelModel()
{
}

int DbSelModel::rowCount(const QModelIndex& parent) const
{
	return metadb_->dbnumber();
}

QVariant DbSelModel::data(const QModelIndex& index, int role) const
{
	int idx = index.row();
	if ( role != Qt::DisplayRole && role != Qt::EditRole )
		return QVariant();
	return metadb_->seg_data(idx, metadb_t::NameSeg);
}

#if 0
QVariant DbSelModel::headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
{
	if (role != Qt::DisplayRole)
		return QVariant();
}
#endif

bool DbSelModel::setData (const QModelIndex& index, const QVariant& value, int role)
{
	return false; //single :memory: sqlite db, can't be changed
}

Qt::ItemFlags DbSelModel::flags (const QModelIndex& index) const
{
	return Qt::ItemIsEnabled/*|Qt::ItemIsEditable*/;
}

void DbSelModel::selectionChange(const QModelIndex& index)
{
	int sel_bak = selected_;
	selected_ = index.row();

	if ( sel_bak != selected_ )
		emit selectionChanged(selected_);
}
