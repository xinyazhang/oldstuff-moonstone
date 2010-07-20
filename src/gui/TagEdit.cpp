#include "TagEdit.h"
#include "ui_tag.h"
#include "TagSearchResultModel.h"
#include "../kernel/TagManContext.h"
#include "../kernel/common.h"

TagEdit::TagEdit(QWidget* parent)
	:ui(new Ui::TagEdit), 
	QWidget(parent), 
	context_(NULL)
{
	ui->setupUi(this);
	ui->retranslateUi(this);
	//ui->tag_search_result->setModel(search_model_);

	ui->toolbar->addAction(ui->actionNewTag);
	ui->toolbar->addAction(ui->actionSaveTag);
	ui->toolbar->addAction(ui->actionRevert);
	ui->toolbar->addAction(ui->actionGlobalUndo);
	ui->toolbar->addAction(ui->actionGlobalRedo);


	makeSigSlotConnections();
}

TagEdit::~TagEdit()
{
	delete ui;
	delete search_model_;
	delete context_;
}

void TagEdit::setup(Database* db)
{
	db_ = db;
	delete context_;
	search_model_ = new TagSearchResultModel(db->tagman());
	ui->tag_search_result->setModel(search_model_);
	//connect(search_model_, SIGNAL(selectionChanged()), this, SLOT(edit_another()));
	context_ = new TagManContext(db);
}

void TagEdit::new_guitag()
{
	saveguard();
	context_->start_context(tag_t());
#if 0
	if (switch_state(NEW_TAG))
	{
		load_data_to_gui();
		edited_ = false;
	}
#endif
}

void TagEdit::edit_another()
{
	saveguard();
	context_->start_context(search_model_->load_tag());
#if 0
	if (switch_state(EDIT_TAG))
	{
		load_data_to_gui();
		edited_ = false;
	}
#endif
}

void TagEdit::search()
{
	search_model_->search(ui->searchline->text());
}

void TagEdit::clear_focus()
{
	clearFocus();
}

void TagEdit::makeSigSlotConnections()
{
	connect(ui->actionNewTag, SIGNAL(triggered()), this, SLOT(new_guitag()));
	connect(ui->actionSaveTag, SIGNAL(triggered()), this, SLOT(save_to_dbtag()));
	connect(ui->actionRevert, SIGNAL(triggered()), this, SLOT(revert()));
	connect(ui->actionGlobalUndo, SIGNAL(triggered()), this, SLOT(undo()));
	connect(ui->actionGlobalRedo, SIGNAL(triggered()), this, SLOT(redo()));
	connect(ui->name, SIGNAL(editingFinished()), this, SLOT(change_tag_name()));
	connect(ui->name, SIGNAL(editingFinished()), this, SLOT(clear_focus()));
	connect(ui->searchbutton, SIGNAL(released()), this, SLOT(search()));
#if 0
	connect(ui->pname, SIGNAL(currentIndexChanged(const QString& text )),
			this, SLOT(change_tag_pname(const QString&)));
#endif
	// notice the undo and redo have been provided by QLineEdit and QTextEdit
	// See QtDesigner, don't conflict in gui design
}

#if 0
bool TagEdit::switch_state(TagEdit::State state)
{
	if ( edited_ )
	{
		if (!require_ack(UNSAVED))
		{
			state_ = old_state;
			return false;
		}
	}
	State old_state = state_;
	state_ = state;
	if ( NEW_TAG == state_ )
	{
		editing_tag_ = tag_t();
		editing_tnode_ = tnode_t();
	} else if ( EDIT_TAG == state_ )
	{
		editing_tag_ = search_model_->load_tag();
		editing_tnode_ = search_model_->load_tnode_if_exists();
	}
	return true;
}
#endif

void TagEdit::load_data_to_gui()
{
	edited_ = false;
	ui->name->setText(context_->name());
	ui->comment->setText(context_->comment());
}

void TagEdit::change_tag_name()
{
	context_->chname(ui->name->text().trimmed());
}

#if 0
void TagEdit::change_tag_pname(const QString& pname)
{
	context_->chpname(pname);
}
#endif

void TagEdit::revert()
{
	context_->start_context(editing_tag_);
	load_data_to_gui();
}

void TagEdit::undo()
{
	context_->undo();
	update_unredo_actions();
	load_data_to_gui();
}

void TagEdit::redo()
{
	context_->redo();
	update_unredo_actions();
	load_data_to_gui();
}

void TagEdit::update_unredo_actions()
{
	ui->actionGlobalUndo->setEnabled(context_->undoable());
	ui->actionGlobalRedo->setEnabled(context_->redoable());
}

void TagEdit::saveguard()
{
}

void TagEdit::save_to_dbtag()
{
	context_->commit_changes();
}
