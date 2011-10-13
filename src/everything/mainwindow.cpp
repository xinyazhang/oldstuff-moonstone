#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    search_model = new SearchFileDBModel(pref());
    ui->result->setModel();
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
