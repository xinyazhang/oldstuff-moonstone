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
	void load_default();

	static LainMain* instance();
	static void release();

	void addsub(QWidget* );
private:
	Ui::LainMain* ui_;

	DatabaseSession* dbs_;
	static LainMain* inst;
};

#endif
