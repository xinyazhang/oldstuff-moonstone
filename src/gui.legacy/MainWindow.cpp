#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QXmlStreamReader>
#include "DbSelModel.h"
#include "DbInfoModel.h"
#include "../kernel/metadb.h"
#include "../dal/dbmeta.h"
#include "../dal/DatabaseInterface.h"


MainWindow::MainWindow()
	:ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	showMaximized();
	ui->retranslateUi(this);

	ui->wi_dock_dblist->setVisible(true);
	ui->wi_dbinfo->setVisible(true);
	ui->wi_dblist->setVisible(true);

	QXmlStreamReader reader("<MetaDB><Database Type=\"SQLite\"><Name>MemoryDB</Name><File>:memory:</File></Database><Database Type=\"SQLite\"><Name>MemoryAlias</Name><File>:memory:</File></Database><Database Type=\"SQLite\"><Name>Imperishable</Name><File>Imperishable</File></Database></MetaDB>");
	metadb_ = shared_ptr<metadb_t>(new metadb_t(reader));
	sel_model_ = new DbSelModel(metadb_);
	info_model_ = new DbInfoModel(metadb_);
	ui->wi_dblist->setModel(sel_model_);
	ui->wi_dbinfo->setModel(info_model_);

	connect(ui->wi_dblist, SIGNAL(clicked(const QModelIndex&)), sel_model_, SLOT(selectionChange(const QModelIndex&)));
	connect(ui->wi_btn_connect, SIGNAL(released()), this, SLOT(newConnection()));
	connect(sel_model_, SIGNAL(selectionChanged(int)), info_model_, SLOT(set_dbidx(int)));
//	connect(sel_model_, SIGNAL(selectionChanged(int )), ui->wi_dbinfo, SLOT(updateGeometries()));
}

MainWindow::~MainWindow()
{
	delete ui;
	delete sel_model_;
	delete info_model_;
}

void MainWindow::newConnection()
{
	shared_ptr<dbmeta_t> dbm = metadb_->dbmeta(sel_model_->selected());
	ui->opened->opendb(dbm);
}
