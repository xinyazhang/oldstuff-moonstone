#ifndef LAIN_GUI_LAINMAIN_H
#define LAIN_GUI_LAINMAIN_H

#include <QtGui/QMainWindow>
class DatabaseSession;

namespace Ui
{
	class LainMain;
}

class LainMain
	:public QMainWindow
{
	Q_OBJECT;
public:
	LainMain();
	~LainMain();

	static LainMain* instance();
	static void release();
private:
	Ui::LainMain* ui_;

	DatabaseSession* dbs_;
	static LainMain* inst;
};

#endif
