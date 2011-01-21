#include "TagSearchResultModel.h"
#include "../kernel/common.h"
#include <QtCore/QTextStream>

TagSearchResultModel::TagSearchResultModel(TagMan* tagman)
	:tagman_(tagman)
{
}

TagSearchResultModel::~TagSearchResultModel()
{
}

void TagSearchResultModel::search(QString& line)
{
	QTextStream parser(&line, QIODevice::ReadOnly);
	unistr_list locator;
	while( !parser.atEnd() )
	{
		QString str;
		parser >> str;
		locator.push_back(str);
	}
	if ( !result_.empty() )
	{
		beginRemoveRows(QModelIndex(), 0, result_.size() - 1);
		result_.clear();
		endRemoveRows();
	}
	result_ = tagman_->locate(locator);
	masternames_.clear();
	if ( !result_.empty() )
	{
		beginInsertRows(QModelIndex(), 0, result_.size() - 1);
		for(taglist_t::const_iterator iter = result_.begin();
				iter != result_.end();
				iter++)
		{
			masternames_.push_back(tagman_->mastername(*iter));
		}
		endInsertRows();
	}
}

tag_t TagSearchResultModel::load_tag()
{
	return tag_t();
}

tnode_t TagSearchResultModel::load_tnode_if_exists()
{
	return tnode_t();
}

int TagSearchResultModel::rowCount(const QModelIndex&  ) const
{
	return result_.size();
}

int TagSearchResultModel::columnCount(const QModelIndex& ) const
{
	return 2;
}

QVariant TagSearchResultModel::data(const QModelIndex& index, int role) const
{
	int row = index.row();
	int col = index.column();

	if ( role != Qt::DisplayRole && role != Qt::EditRole )
		return QVariant();

	if ( col == 0 )
	{
		return QString(result_[row].name);
	} else
		return QString(masternames_[row]);
}

bool TagSearchResultModel::setData (const QModelIndex&, const QVariant&, int )
{
	return false;
}

Qt::ItemFlags TagSearchResultModel::flags(const QModelIndex&) const
{
	return Qt::ItemIsEnabled|Qt::ItemIsEditable;
}
