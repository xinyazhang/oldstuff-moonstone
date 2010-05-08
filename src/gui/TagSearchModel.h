#ifndef TAG_SEARCH_RESULT_MODEL_H
#define TAG_SEARCH_RESULT_MODEL_H

class TagMan;

class TagSearchResultModel
	:public QAbstractTableModel
{
	Q_OBJECT;
public:
	TagSearchResultModel(TagMan* );
	~TagSearchResultModel();

private:
	TagMan* tagman_;
};

#endif
