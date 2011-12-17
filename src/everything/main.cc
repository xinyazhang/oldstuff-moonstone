#include <QtGui/QApplication>
#include "mainwindow.h"
#include "Preferences.h"
#include <kernel/evid.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QEvent::registerEventType(EVENT_SEARCHDONE);
	QEvent::registerEventType(CVT_QEVENT(INDEX_PROGRESSED));
    MainWindow w;
    w.show();

    return a.exec();
}

#ifdef Q_WS_WIN
#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int )
{
	return main(__argc, __argv);
}
#endif
