#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "searchfiledbmodel.h"
#include "volumemodel.h"
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
	void on_searching_textChanged(const QString &arg1);
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
private:
	SearchFileDBModel* search_model;
        VolumeModel* vol_model;
	Ui::MainWindow *ui;
	class Preferences* pref_;
};

static const int EVENT_SEARCHDONE = QEvent::User+1;

#endif // MAINWINDOW_H
