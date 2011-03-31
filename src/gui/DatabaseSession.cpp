#include "DatabaseSession.h"
#include "LainMain.h"
#include "ui_databasesessionlayerout.h"
//#include "TagSearcher.h"

DatabaseSession::DatabaseSession()
	:QWidget(LainMain::instance())
{
	db_ = new Database(sqlite_ = new db_sqlite_impl(":memory:", ""));

	ui_->setupUi(this);
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
