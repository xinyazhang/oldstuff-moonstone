#ifndef LAIN_GUI_TAG_SERACHER_H
#define LAIN_GUI_TAG_SERACHER_H

#include "SearchHandler.h"

class TagSearcher
	:public SearchHandler
{
public:
	TagSearcher(Database* db);
	~TagSearcher() {}
	virtual void launch_search(const QString& );
private:
};

#endif
