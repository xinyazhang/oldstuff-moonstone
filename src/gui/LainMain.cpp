#include "LainMain.h"
#include "ui_lain.h"

LainMain* LainMain::inst = NULL;

LainMain::LainMain()
	:ui_(new Ui::LainMain)
{
	ui_->setupUi(this);
	/* Automaticlly Load the default db */
	dbs_ = new DatabaseSession;
}

LainMain::~LainMain()
{
	delete dbs_;
	delete ui_;
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
