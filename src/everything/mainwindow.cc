#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(class Preferences* pref, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	pref_(pref)
{
	ui->setupUi(this);
	ui->result->setModel();
	search_model = new SearchFileDBModel(pref());
	setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
	delete search_model;
	delete ui;
}

void MainWindow::on_searching_textChanged(const QString &arg1)
{
	search_model->change_searching_text(arg1);
}
