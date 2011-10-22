#include <QtGui/QApplication>
#include "mainwindow.h"
#include "Preferences.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QEvent::registerEventType(EVENT_SEARCHDONE);
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