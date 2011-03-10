#include "KBModel.h"

KBModel::KBModel(Database* db, KBViewItemType type, QObject* parent)
	:QAbstractItemModel(parent), db_(db)
{
	root_ = KBViewItem::RootFactory(db, type);
}

KBModel::~KBModel()
{
	delete root_;
}

QVariant KBModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	KBViewItem *item = static_cast<KBViewItem*>(index.internalPointer());

	return item->col_data(db_, index.column());
}

Qt::ItemFlags KBModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant KBModel::headerData(int section, 
		Qt::Orientation orientation, 
		int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return root_->col_data(db_, section);

	return QVariant();
}

QModelIndex KBModel::index(int row, 
		int column, 
		const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	KBViewItem *parent_item;

	if (!parent.isValid())
		parent_item = root_;
	else
		parent_item = static_cast<KBViewItem*>(parent.internalPointer());

	if ( row < 0 || row >= parent_item->children_count(db_) )
		return QModelIndex();

	KBViewItem *child_item = parent_item->child(db_, row);
	if (child_item)
		return createIndex(row, column, child_item);
	else
		return QModelIndex();
}

QModelIndex KBModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	KBViewItem *cur = static_cast<KBViewItem*>(index.internalPointer());
	KBViewItem *parent = cur->parent();

	if (parent == root_)
		return QModelIndex();

	return createIndex(parent->row(), 0, parent);
}

int KBModel::rowCount(const QModelIndex &parent) const
{
	KBViewItem *parent_item;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parent_item = root_;
	else
		parent_item = static_cast<KBViewItem*>(parent.internalPointer());

	return parent_item->children_count(db_);
}

int KBModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<KBViewItem*>(parent.internalPointer())->header_count();
	else
		return root_->header_count();
}

