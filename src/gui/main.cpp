#include <QtGui/QApplication>
#include "LainMain.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	LainMain* lm = LainMain::instance();
	lm->load_default();
	lm->show();
	int ret = app.exec();
	LainMain::release();
	return ret;
}

#ifdef Q_WS_WIN
#include "windows.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int )
{
	return main(__argc, __argv);
}
#endif
