#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "searchfiledbmodel.h"
#include "volumemodel.h"
#include <QtCore/QEvent>
#include <QtGui/QAbstractButton>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	bool event ( QEvent * e );
private slots:
	void on_searching_textChanged(const QString &arg1);
	void on_buttonBox_clicked(QAbstractButton *button);
private:
	SearchFileDBModel* search_model;
        VolumeModel* vol_model;
	Ui::MainWindow *ui;
	class Preferences* pref_;
};

static const int EVENT_SEARCHDONE = QEvent::User+1;

#define CVT_QEVENT(x) (QEvent::User + x)

#endif // MAINWINDOW_H
