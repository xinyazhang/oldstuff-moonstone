#include "TagManContext.h"
#include "common.h"

using std::vector;

TagManContext::TagManContext(Database* db)
	:db_(db),
	edited_(false)
{
}

void TagManContext::start_context(const tag_t& tag)
{
	edited_tag_ = original_tag_ = tag;
	if ( tag.tnode != 0 )
		original_tnode_ = edited_tnode_ = db_->tagman()->access_tnode(edited_tag_);
	else
		original_tnode_ = edited_tnode_ = TnodeMan::invalid_tnode();
	edited_ = false;
	merging_tnodes_.clear();
	merging_tnodes_.push_back(original_tnode_.idx);
}

bool TagManContext::edited() const
{
	return edited_;
}

void TagManContext::undo()
{
	if ( undoable() )
	{
		tag_edit_action act = undo_stack_.pop();
		act.undo();
		redo_stack_.push(act);
	}
}

void TagManContext::redo()
{
	if ( redoable() )
	{
		tag_edit_action act = redo_stack_.pop();
		act.redo();
		undo_stack_.push(act);
	}
}

bool TagManContext::undoable() const
{
	return !undo_stack_.empty();
}

bool TagManContext::redoable() const
{
	return !redo_stack_.empty();
}

unistr TagManContext::name() const
{
	return edited_tag_.name;
}

void TagManContext::chname(const unistr& name)
{
	if ( this->name() == name )
		return ;
	tag_edit_action act = tag_edit_action::
		chname_action(&edited_tag_, name);
	act.redo();
	undo_stack_.push(act);
}

unistr TagManContext::pname() const
{
	if ( TnodeMan::invalid(edited_tnode_) )
	{
		return edited_tag_.name;
	} else
		return edited_tnode_.mastername;
}

#if 0 // mkpname is better
void TagManContext::chpname(const unistr& name)
{
	if ( pname() == name )
		return ;
	tag_edit_action act = tag_edit_action::
		chpname_action(&edited_tnode_, &edited_tag_);
	act.redo();
	undo_stack_.push(act);
}
#endif

taglist_t TagManContext::alias() const
{
	taglist_t ret;
	for(vector<idx_t>::const_iterator iter = merging_tnodes_.begin();
			iter != merging_tnodes_.end();
			iter++)
	{
		taglist_t i = db_->tnodeman()->names(*iter);
		ret.insert(ret.end(), i.begin(), i.end());
	}
	return ret;
}

#if 0
void TagManContext::merge(const tag_t& tag)
{
	tag_edit_action act = tag_edit_action::
		merge_action(&merging_tnodes_, tag);
	act.redo();
	undo_stack_.push(act);
}
#endif

void TagManContext::addtag(const tag_t&)
{
	// no impl.
}

unistr TagManContext::comment() const
{
	return edited_tnode_.comment;
}

void TagManContext::editcomment(const unistr& comment)
{
	edited_tnode_.comment = comment;
}

bool TagManContext::commit_changes()
{
	db_->begin_transaction();
	/*
	 * this is necessary
	 * 	tag_edit_actions sometimes contains pointers to edited_*_ for make decision
	 */
	unistr edited_comment = edited_tnode_.comment; // backup edited comment
	edited_tag_ = original_tag_;
	edited_tnode_ = original_tnode_;

	while(!undo_stack_.empty())
	{
		tag_edit_action act = undo_stack_.front();
		if ( !act.execute(db_) )
		{
			db_->abort_transaction();
			return false;
		}
		undo_stack_.pop_front();
	}
	if ( edited_comment != original_tnode_.comment ) // commit comment's editing
	{
		if ( edited_tnode_.idx != 0 )
		{
			edited_tnode_.comment = edited_comment;
			db_->tnodeman()->update(edited_tnode_);
		} else if ( !TagMan::invalid(edited_tag_) ) // invalid tag has no name, so it has no comment.
		{
			db_->tnodeman()->create(&edited_tag_, edited_comment);
		}
	}
	db_->final_transaction();

	undo_stack_.clear();
	redo_stack_.clear();
	original_tag_ = edited_tag_;
	original_tnode_ = edited_tnode_;
	merging_tnodes_.clear();
	merging_tnodes_.push_back(original_tag_.tnode);

	return true;
}

bool TagManContext::discard_changes()
{
	edited_tag_ = original_tag_;
	edited_tnode_ = original_tnode_;
	undo_stack_.clear();
	redo_stack_.clear();
	merging_tnodes_.clear();
	merging_tnodes_.push_back(original_tag_.tnode);

	return true;
}
