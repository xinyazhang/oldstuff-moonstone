#include <QtGui/QApplication>
#include "mainwindow.h"
#include "Preferences.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QEvent::registerEventType(EVENT_SEARCHDONE);
    MainWindow w(pref());
    w.show();

    return a.exec();
}
