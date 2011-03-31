#include <QtGui/QApplication>
#include "LainMain.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	LainMain* lm = LainMain::instance();
	lm->show();
	int ret = app.exec();
	LainMain::release();
	return ret;
}
