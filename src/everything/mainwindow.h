#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "searchfiledbmodel.h"
#include <QtCore/QEvent>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_searching_textChanged(const QString &arg1);

private:
    SearchFileDBModel* search_model;
    Ui::MainWindow *ui;
};

static const int EVENT_SEARCHDONE = QEvent::User+1;

#endif // MAINWINDOW_H
