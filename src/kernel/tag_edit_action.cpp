#include "tag_edit_action.h"
#include "common.h"
#include <algorithm>

using std::vector;

class tag_chname_action
	:public tag_edit_action_impl
{
public:
	tag_chname_action(tag_t* tag, const unistr& name)
		:o_(tag->name), 
		tag_(tag), 
		n_(name), 
		invalid_(TagMan::invalid(*tag))
	{
	}

	void redo()
	{
		tag_->name = n_;
	}

	void undo()
	{
		tag_->name = o_;
	}

	bool execute(Database* db)
	{
		if ( invalid_ )
		{
			*tag_ = db->tagman()->create(n_);
			return !TagMan::invalid(*tag_);
		} else 
		{
			tag_t nt = *tag_;
			nt.name = n_;
			if (db->tagman()->update(*tag_, nt) )
			{
				tag_->name = n_;
				return true;
			} else
				return false;
		}
	}
private:
	tag_t* tag_;
	unistr o_; //old name
	unistr n_; //new name
	bool invalid_;
};

tag_edit_action tag_edit_action::chname_action(tag_t* tag, const unistr& name)
{
	return tag_edit_action(new tag_chname_action(tag, name));
}

class tag_chpname_action
	:public tag_edit_action_impl
{
public:
	tag_chpname_action(tnode_t* tnode, tag_t* tag)
		:tnode_(tnode),
		tag_(tag),
		o_(tnode->mastername),
		n_(tag->name)
	{
	}
	
	void undo()
	{
		tnode_->mastername = o_;
	}
	
	void redo()
	{
		tnode_->mastername = n_;
	}

	bool execute(Database* db)
	{
		if ( TnodeMan::invalid(*tnode_) )
		{
			idx_t idx = db->tnodeman()->create(tag_, unistr());
			if ( 0 == idx )
				return false;
			tnode_->idx = idx;
			tag_->tnode = idx;
		}
		tnode_->mastername = n_;
		return db->tnodeman()->update(*tnode_);
	}
private:
	tnode_t *tnode_;
	tag_t* tag_;
	unistr o_;
	unistr n_;
};

tag_edit_action tag_edit_action::chpname_action(tnode_t* tnode, tag_t* tag)
{
	return tag_edit_action(new tag_chpname_action(tnode, tag));
}

#if 0
class tag_merge_action
	:public tag_edit_action_impl
{
public:
	tag_merge_action(vector<idx_t>* vec, idx_t tnode)
		:vec_(vec),
		i_(tnode)
	{
		tagnoroot_ = (i_ == 0);
		duplicated_ = (std::find(vec_->begin(), vec_->end(), i_) != vec_->end());
	}

	void undo()
	{
		if ( !duplicated_ )
		{
			vec_->erase(std::find(
						vec_->begin(),
						vec_->end(),
						i_));
		}
	}

	void redo()
	{
		if ( !duplicated_)
			vec_->push_back(i_);
	}

	bool execute(Database* db)
	{
		if ( vec_->size() <= 1 )
			return true;
		/*
		 * TODO
		 */
#if 0
		sql_stmt stmt = db_->create_stmt_ex("UPDATE "+
				db_->table(Database::TagTable)+
				" set name = $1, tnode = $2 where name = $3 and tnode = $4");
#endif
		return true;
	}
private:
	vector<idx_t>* vec_;
	idx_t i_;
	bool duplicated_;
	bool tagnoroot_;
};

tag_edit_action tag_edit_action::merge_action(vector<idx_t>* vec, idx_t idx)
{
	return tag_edit_action(new tag_merge_action(vec, idx));
}
#endif
