#include "tag_dock.h"
#include "LainMain.h"
#include "ui_dock_tag_editor.h"
#include <kernel/common.h>

TagDock::TagDock(Database* db)
	:QWidget(/*LainMain::instance()*/), ui_(new Ui::TagEditor), db_(db)
{
	ui_->setupUi(this);
	//setWidget(content_);
	/* Don't setup model currently */
#if 0
	relations_ = new RelationModel;
	ui_->relations->setModel(relations_);
#endif 
	connect(ui_->conform, SIGNAL(released()), this, SLOT(commit()));
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

void TagDock::commit()
{
	tag_t tag = db_->tagman()->create(ui_->tag_name->text());
	tnode_t tnode = db_->tagman()->access_tnode(tag);
	if ( ui_->master_name->text().isEmpty() )
	{
		ui_->master_name->setText(tnode.mastername);
	} else
	{
		tnode.mastername = ui_->master_name->text();
		db_->tnodeman()->update(tnode);
	}
}
