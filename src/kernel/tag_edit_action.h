#ifndef KERNEL_TAG_EDIT_ACTION_H
#define KERNEL_TAG_EDIT_ACTION_H

#include "../pal/stdtype.h"
#include "common_declare.h"
#include "sql_stmt.h"

class tag_edit_action_impl
{
public:
	virtual ~tag_edit_action_impl() {};
	virtual void redo() = 0;
	virtual void undo() = 0;
	//virtual sql_stmt sql(Database* ) = 0;
	virtual bool execute(Database* ) = 0; // return false on failed
};

class tag_edit_action
{
public:
	/*
	 * chname_action:
	 * 	if tag is invalid, give an insert sql, or give an update sql.
	 */
	static tag_edit_action chname_action(tag_t*, const unistr&);
	/*
	 * chpname_action:
	 * 	if tnode_t is invalid, give an insert sql, or give an update sql.
	 */
	static tag_edit_action chpname_action(tnode_t*, tag_t* );
	/*
	 * put idx into merge_list
	 * take care: dumplicated value
	 */
	static tag_edit_action merge_action(std::vector<idx_t>* , idx_t );

	void redo() { p_->redo(); }
	void undo() { p_->undo(); }
	//sql_stmt sql(Database* db) { return p_->sql(db); }
	bool execute(Database* db) { return p_->execute(db); }
private:
	tag_edit_action(tag_edit_action_impl* impl):p_(impl) {}
	shared_ptr<tag_edit_action_impl> p_;
};

#endif
