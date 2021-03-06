#include <QtCore/QMimeData>
#include <QtGui/QMessageBox>
#include "KBModel.h"
#include <kernel/common.h>

KBModel::KBModel(Database* db, KBViewItemType type, QStringList locators, QObject* parent)
	:QAbstractItemModel(parent), db_(db)
{
	root_ = KBViewItem::RootFactory(db, type, locators);
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
		return Qt::ItemIsDropEnabled;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
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
		const QModelIndex &parent_index) const
{
	if (!hasIndex(row, column, parent_index))
		return QModelIndex();

	KBViewItem *parent_item;

	if (!parent_index.isValid())
		parent_item = root_;
	else
		parent_item = static_cast<KBViewItem*>(parent_index.internalPointer());

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
	KBViewItem *cur_parent = cur->parent();

	if (cur_parent == NULL)
		return QModelIndex();

	if (cur_parent == root_)
		return QModelIndex();

	return createIndex(cur_parent->row(), 0, cur_parent);
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

QStringList KBModel::mimeTypes() const
{
	QStringList types;
	types << "application/lain.local_binary_objects";
	return types;
}

QMimeData* KBModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	foreach (const QModelIndex &index, indexes) 
	{
		if (index.isValid()) 
		{
			const KBViewItem* item = static_cast<const KBViewItem*>(index.internalPointer());
			item->append_binary_layerout(stream);
		}
	}

	mimeData->setData("application/lain.local_binary_objects", encodedData);
	return mimeData;
}

bool KBModel::dropMimeData(const QMimeData* mime, Qt::DropAction action, int row, int col, const QModelIndex& parent)
{
	if (action == Qt::IgnoreAction)
		return true;

	if (!mime->hasFormat("application/lain.local_binary_objects"))
		return false;

	QByteArray encodedData = mime->data("application/lain.local_binary_objects");
	QByteArray dt = encodedData;
	dt.append(' ');
	dt.chop(1);
	QDataStream stream(&dt, QIODevice::ReadOnly);
	QString text;

	while (!stream.atEnd())
	{
		tnode_t tnode;
		stream >> tnode;
		tnode = db_->tnodeman()->locate(tnode.idx);
		text += "Tnode: ";
		text += QString::number(tnode.idx);
		text += "\t";
		text += tnode.mastername;
		text += "\n";
	}
	QMessageBox::information(NULL, tr("Drop!"), text);
	return true;
}

Qt::DropActions KBModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}
