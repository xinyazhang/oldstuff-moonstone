#include "tag_dock.h"
#include "LainMain.h"
#include "ui_dock_tag_editor.h"
#include <kernel/common.h>

TagDock::TagDock()
	:QDockWidget(LainMain::instance()), ui_(new Ui::TagEditor)
{
	ui_->setupUi(this);
	/* Don't setup model currently */
#if 0
	relations_ = new RelationModel;
	ui_->relations->setModel(relations_);
#endif 
}

TagDock::~TagDock()
{
	delete ui_;
#if 0
	delete relations_;
#endif
}

void TagDock::select_target(tag_t tag)
{
#if 0
	relations_->select_target(tag);
#endif
}
