#include "DatabaseSession.h"
#include "LainMain.h"
#include "ui_DatabaseSessionLayout.h"
#include <kernel/common.h>
//#include "TagSearcher.h"

DatabaseSession::DatabaseSession(LainMain* parent)
	:QWidget(parent), ui_(new Ui::DatabaseSession)
{
	db_ = new Database(sqlite_ = new db_sqlite_impl(":memory:", ""));

	ui_->setupUi(this);
	ui_->listWidget->addAction(ui_->actionTag);
	//ui_->tag_page->install_sh(new TagSearcher(db_));

	connect(ui_->actionTag, SIGNAL(triggered(bool)),
			this, SLOT(s2tag()));
}

DatabaseSession::~DatabaseSession()
{
	delete ui_;
	delete db_;
	delete sqlite_;
}

void DatabaseSession::s2tag()
{
	ui_->stacked->setCurrentIndex(0);
}
