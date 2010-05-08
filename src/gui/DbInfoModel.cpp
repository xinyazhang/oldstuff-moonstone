#include "DbInfoModel.h"
#include "../kernel/metadb.h"

DbInfoModel::DbInfoModel(shared_ptr<metadb_t> meta)
	:selected_(0), meta_(meta)
{
}

DbInfoModel::~DbInfoModel()
{
}

void DbInfoModel::set_dbidx(int idx)
{
	beginRemoveRows(QModelIndex(), 0, meta_->seg_num(selected_) - 1);
	int old_selected_ = selected_;
	selected_ = idx;
	endRemoveRows();

	beginInsertRows(QModelIndex(), 0, meta_->seg_num(selected_) - 1);
	endInsertRows();
}

/*
 * - Name
 * - Connection
 * - Username(optinal)
 * - Password(optinal)
 * - Describe
 */

int DbInfoModel::rowCount(const QModelIndex& ) const
{
	return meta_->seg_num(selected_);
}

int DbInfoModel::columnCount(const QModelIndex& ) const
{
	return 2;
}

QVariant DbInfoModel::data(const QModelIndex& index, int role ) const
{
	int row = index.row();
	int col = index.column();

	if ( role != Qt::DisplayRole && role != Qt::EditRole )
		return QVariant();

	if ( col == 0 )
	{
		return meta_->seg_name(selected_, row);
	} else
		return meta_->seg_data(selected_, row);
}

#if 0
QVariant DbInfoModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
}
#endif

bool DbInfoModel::setData (const QModelIndex& index, const QVariant& value, int role)
{
	return false;
}

Qt::ItemFlags DbInfoModel::flags (const QModelIndex& ) const
{
	return Qt::ItemIsEnabled|Qt::ItemIsEditable;
}

