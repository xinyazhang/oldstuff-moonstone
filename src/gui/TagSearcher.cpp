#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "TagSearcher.h"
#include "KBModel.h"
#include <kernel/common.h>

TagSearcher::TagSearcher(Database* db)
	:SearchHandler(db) 
{
}

void TagSearcher::launch_search(const QString& str)
{
	/* slice */
	QString ss(str);
	QTextStream ts(&ss);
	QString iter;
	QStringList qsl;
	while( !ts.atEnd() )
	{
		ts >> iter;
		qsl << iter;
	}

	QAbstractItemModel *m = view_->model();
	view_->setModel(new KBModel(db_, KB_tag_item, qsl));
	delete m;
}
