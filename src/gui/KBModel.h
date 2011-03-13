/*
 * This model organize KBItem and report (or, be reported) information to the
 * QTreeView
 *
 * Item
 * 	+-- Interlaced Item (e.g. if above one is file, this is tag)
 * 	+-- Subitem 0 (subdirectory for directory, tag tagged by Item for tag)
 * 	+-- Subitem 1
 */

#ifndef LAIN_GUI_KNOWLEDGE_BASE_MODEL
#define LAIN_GUI_KNOWLEDGE_BASE_MODEL

#include <QtCore/QAbstractItemModel>
#include <QtCore/QStringList>
#include <kernel/common_declare.h>
#include "KBViewItem.h"

class KBModel
	:public QAbstractItemModel
{
	Q_OBJECT;

public:
	KBModel(Database* db, KBViewItemType type /* the root's type */, QStringList locators, QObject *parent = 0);
	~KBModel();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, 
			Qt::Orientation orientation, 
			int role = Qt::DisplayRole) const;
	QModelIndex index(int row, 
			int column, 
			const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
	Database *db_;
	KBViewItem *root_;
};

#endif
