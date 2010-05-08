#ifndef DB_INFO_MODEL_H
#define DB_INFO_MODEL_H

#include <QtCore/QAbstractTableModel>
#include "../pal/stdtype.h"
class metadb_t;

/*
 * A Dummy Model Class
 * 
 * Note for a real class:
 * 	- Supervisor(MainWindow Class) must connect their's dataChanged signals
 */
class DbInfoModel
	:public QAbstractTableModel
{
	Q_OBJECT;
public:
	DbInfoModel(shared_ptr<metadb_t> );
	~DbInfoModel();

public slots:
	void set_dbidx(int idx);

public: // override function
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
//	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	bool setData (const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	Qt::ItemFlags flags (const QModelIndex& index) const;

private:
	shared_ptr<metadb_t> meta_;
	int selected_;
};

#endif
