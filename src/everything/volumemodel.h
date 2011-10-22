#ifndef VOLUMEMODE_H
#define VOLUMEMODE_H

#include <QtCore/QAbstractItemModel>
#include <pal/volume.h>

class VolumeModel: public QAbstractItemModel
{
	Q_OBJECT;
public:
	VolumeModel(class Preferences* pref);
	~VolumeModel();

	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section,
			Qt::Orientation orientation,
			int role = Qt::DisplayRole) const;
	QModelIndex index(int row,
			int column,
			const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	void apply_changes();
	void clear_changes();
private:
	class Preferences* pref;
	std::vector<volume> vol_list;
	std::vector<volume> watching_list;
};

#endif
