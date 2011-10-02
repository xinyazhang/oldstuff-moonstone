#ifndef LAIN_GUI_SERACH_HANDLER_H
#define LAIN_GUI_SERACH_HANDLER_H

#include <QtCore/QString>
#include <QtGui/QTreeView>

class Database;

class SearchHandler
{
public:
	SearchHandler(Database* db):db_(db) {}
	virtual ~SearchHandler() {}
	void install_view(QTreeView* tw) { view_ = tw; }
	virtual void launch_search(const QString& ) = 0;
private:
	SearchHandler();
protected:
	Database* db_;
	QTreeView* view_;
};

#endif
