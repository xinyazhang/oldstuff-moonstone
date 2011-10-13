#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QEvent::registerEventType(EVENT_SEARCHDONE);
    MainWindow w;
    w.show();

    return a.exec();
}
