#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchfiledbmodel.h"
#include "volumemodel.h"
#include "Preferences.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	search_model = new SearchFileDBModel(pref());
	ui->result->setModel(search_model);
	vol_model = new VolumeModel(pref());
	ui->volume_list->setModel(vol_model);
	setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
        delete vol_model;
	delete search_model;
	delete ui;
}

void MainWindow::on_searching_textChanged(const QString &arg1)
{
	search_model->change_searching_text(arg1);
}

void MainWindow::on_buttonBox_accepted()
{
        vol_model->apply_changes();
}

void MainWindow::on_buttonBox_rejected()
{
        vol_model->clear_changes();
}
