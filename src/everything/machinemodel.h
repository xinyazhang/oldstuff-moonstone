#ifndef MACHINEMODE_H
#define MACHINEMODE_H

#include <QtCore/QAbstractItemModel>
#include <pal/volume.h>
#include <kernel/machine.h>
#include "treeitem.h"

class MachineRoot;

class MachineModel: public QAbstractItemModel
{
	Q_OBJECT;
public:
	MachineModel();
	~MachineModel();

	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	QVariant headerData(int section,
			Qt::Orientation orientation,
			int role = Qt::DisplayRole) const;
	QModelIndex index(int row,
			int column,
			const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	bool insertColumns(int position, int columns,
					const QModelIndex &parent = QModelIndex());
	bool removeColumns(int position, int columns,
					const QModelIndex &parent = QModelIndex());
	bool insertRows(int position, int rows,
					const QModelIndex &parent = QModelIndex());
	bool removeRows(int position, int rows,
					const QModelIndex &parent = QModelIndex());
public slots:
	void applyChanges();
	void discardChanges();
private:
	/** State tracking **/
	/* volume state */
	std::vector<volume> vol_list;

	MachineRoot *rootItem;
	TreeItem* getItem(const QModelIndex &index) const;
};

#endif
