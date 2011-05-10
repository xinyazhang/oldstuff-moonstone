#include <QtGui/QMdiSubWindow>
#include <QtGui/QDockWidget>
#include "LainMain.h"
#include "ui_lain.h"
#include "DatabaseSession.h"
#include "tag_dock.h"

LainMain* LainMain::inst = NULL;

LainMain::LainMain()
	:ui_(new Ui::LainMain)
{
	ui_->setupUi(this);
	setWindowState(Qt::WindowMaximized);
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
	QMdiSubWindow *mdis = new QMdiSubWindow(this);
	mdis->setWidget(dbs_);
	mdis->setWindowState(Qt::WindowMaximized);

	ui_->mdiArea->addSubWindow(mdis);
	/* Test Dock */
	//TagDock *ge = new TagDock(dbs_;
	//addDockWidget(Qt::RightDockWidgetArea, ge);
	/* Add Dock trigger */
	connect(ui_->actionTag, SIGNAL(triggered()),
			dbs_, SLOT(newTagDialog()));
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

void LainMain::addsub(QWidget* widget)
{
	//addDockWidget(Qt::RightDockWidgetArea, widget);
	QDockWidget* dw = new QDockWidget(this);
	dw->setWidget(widget);
	addDockWidget(Qt::AllDockWidgetAreas, dw);
}
