#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../pal/stdtype.h"
class DbSelModel;
class DbInfoModel;
class metadb_t;

namespace Ui
{
	class MainWindow;
};

class MainWindow
	:public QMainWindow
{
	Q_OBJECT;
public:
	MainWindow();
	~MainWindow();
private:
	Ui::MainWindow* ui;
	DbSelModel* sel_model_;
	DbInfoModel* info_model_;
	shared_ptr<metadb_t> metadb_;
private slots:
	void newConnection();
};

#endif
