#include <QtGui/QApplication>
#include "LainMain.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	LainMain* lm = new LainMain();
	lm->show();

	return app.exec();
}
