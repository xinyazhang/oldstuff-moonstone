#ifndef GUI_TAG_SEARCH_RESULT_MODEL_H
#define GUI_TAG_SEARCH_RESULT_MODEL_H

#include <QtCore/QAbstractTableModel>

class TagMan;
#include "../kernel/common_declare.h"
#include "../kernel/kernellist.h"

class TagSearchResultModel
	:public QAbstractTableModel 
{
	Q_OBJECT;
public:
	TagSearchResultModel(TagMan* tagman);
	~TagSearchResultModel();
	void search(QString& line); // parse and search
	tag_t load_tag();
	tnode_t load_tnode_if_exists();

public: // override function
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
//	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	/*
	 * Convience for Ctrl+C
	 */
	bool setData (const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	Qt::ItemFlags flags (const QModelIndex& index) const;

private:
	TagMan* tagman_;
	taglist_t result_;
	unistr_list masternames_;
};

#endif
