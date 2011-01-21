#include "sessionwindow.h"
#include "ui_Session.h"
#include "../kernel/common.h"
#include "TagEdit.h"

SessionWindow::SessionWindow(boost::shared_ptr<DatabaseInterface> dbi, QWidget* parent)
	:ui(new Ui::SessionWindow), 
	QWidget(parent), 
	dbi_(dbi), 
	db_(new Database(dbi.get()))
{
	ui->setupUi(this);
	ui->retranslateUi(this);

	ui->widget->setup(db_);
	//ui->widget_2->setup(db_);

	if (!db_->initialized())
		db_->initialize();
}

SessionWindow::~SessionWindow()
{
	delete ui;
	delete db_;
}

