#ifndef DB_SEL_MODEL_H
#define DB_SEL_MODEL_H

#include <QtCore/QAbstractListModel>
#include "../pal/stdtype.h"
class metadb_t;

/*
 * A Dummy Model Class
 * Provided for initial model test
 */
class DbSelModel
	:public QAbstractListModel
{
	Q_OBJECT;
public:
	DbSelModel(shared_ptr<metadb_t> );
	~DbSelModel();

	int selected() const { return selected_; }

signals:
	void selectionChanged(int );
public slots:
	void selectionChange(const QModelIndex&);
public:
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	//QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	bool setData (const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	Qt::ItemFlags flags (const QModelIndex& index) const;
private:
	shared_ptr<metadb_t> metadb_;
	int selected_;
};

#endif
