#include "LainMain.h"
#include "ui_lain.h"
#include "DatabaseSession.h"
#include "tag_dock.h"

LainMain* LainMain::inst = NULL;

LainMain::LainMain()
	:ui_(new Ui::LainMain)
{
	ui_->setupUi(this);
}

LainMain::~LainMain()
{
	delete dbs_;
	delete ui_;
}

void LainMain::load_default()
{
	/* Automaticlly Load the default db */
	dbs_ = new DatabaseSession(this);
	ui_->mdiArea->addSubWindow(dbs_);
	/* Test Dock */
	TagDock *ge = new TagDock;
	addDockWidget(Qt::RightDockWidgetArea, ge);
}

LainMain* LainMain::instance()
{
	if ( inst == NULL )
		inst = new LainMain;
	return inst;
}

void LainMain::release()
{
	delete inst;
	inst = NULL;
}
