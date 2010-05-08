#ifndef SESSIONWINDOW_H 
#define SESSIONWINDOW_H 

#include <QMainWindow>
#include "../pal/stdtype.h"

class Database;
class DatabaseInterface;

namespace Ui
{
	class SessionWindow;
};

class SessionWindow
	:public QWidget
{
	Q_OBJECT;
public:
	SessionWindow(boost::shared_ptr<DatabaseInterface> dbi, QWidget* parent = NULL);
	~SessionWindow();
private:
	Ui::SessionWindow* ui;
	boost::shared_ptr<DatabaseInterface> dbi_;
	Database* db_;
};

#endif

