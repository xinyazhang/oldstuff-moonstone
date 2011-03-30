#ifndef LAIN_GUI_LAINMAIN_H
#define LAIN_GUI_LAINMAIN_H

#include <QtGui/QMainWindow>
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

private:
	Ui::LainMain* ui_;
};

#endif
