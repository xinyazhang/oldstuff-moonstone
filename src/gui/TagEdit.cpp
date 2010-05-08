#include "TagEdit.h"
#include "ui_tag.h"
#include "TagSearchResultModel.h"

TagEdit::TagEdit(QWidget* parent)
	:ui(new Ui::TagEdit), 
	QWidget(parent), 
	search_model_(new TagSearchModel(tagman)),
	context_(NULL)
{
	ui->setupUi(this);
	ui->retranslateUi(this);
	ui->tag_search_result->setModel(search_model_);

	ui->toolbar->addAction(ui->actionNewTag);
	ui->toolbar->addAction(ui->actionSaveTag);

	connect(search_model_, SIGNAL(selectionChanged()), this, SLOT(edit_another()));

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

void TagEdit::makeSigSlotConnections()
{
	connect(ui->actionNewTag, SIGNAL(triggered()), this, SLOT(new_guitag()));
	connect(ui->actionSaveTag, SIGNAL(triggered()), this, SLOT(save_to_dbtag()));
	connect(ui->actionGlobalUndo, SIGNAL(triggered()), this, SLOT(undo()));
	connect(ui->actionGlobalRedo, SIGNAL(triggered()), this, SLOT(redo()));
	connect(ui->name, SIGNAL(editingFinished()), this, SLOT(change_tag_name()));
	connect(ui->pname, SIGNAL(currentIndexChanged(const QString& text )),
			this, SLOT(change_tag_pname(const QString&)));
	// notice the undo and redo have been provided by QLineEdit and QTextEdit
	// See QtDesigner, don't conflict in gui design
}

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

void TagEdit::load_data_to_gui()
{
	edited_ = false;
	ui->name->setText(editing_tag_.name);
	ui->comment->setText(editing_tnode_.comment);
}

void TagEdit::change_tag_name()
{
	context_->chname(ui->name->text());
}

void TagEdit::change_tag_pname(const QString& pname)
{
	context_->chpname(pname);
}

void TagEdit::undo()
{
	context_->undo();
	update_unredo_actions();
}

void TagEdit::redo()
{
	context_->redo();
	update_unredo_actions();
}

void TagEdit::update_unredo_actions()
{
	ui->actionGlobalUndo->setEnable(context_->undoable());
	ui->actionGlobalRedo->setEnable(context_->redoable());
}
