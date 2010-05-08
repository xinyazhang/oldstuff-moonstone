#ifndef KERNEL_TAGMAN_CONTEXT_H
#define KERNEL_TAGMAN_CONTEXT_H

#include "common_declare.h"
#include "tag.h"
#include "tnode.h"
#include "tag_edit_action.h"
#include "kstack.h"

/*
 * Redo/Undo layer for tag managerment
 * Underlaying classes:
 *      - TagMan
 *      - TnodeMan
 */
class EXPORT_TO_DLL TagManContext
{
public:
	TagManContext(Database* );

	bool edited() const;
	/* 
	 * this is an new start for context, for editing some tag
	 *  note: tag_t() for new tag
	 */
	void start_context(const tag_t& = tag_t()); 
	void undo();
	void redo();
	bool undoable() const;
	bool redoable() const;
	/*
	 * DOs for higer level of tag modification, rather than tnode&tag mixed manipulate
	 */
	/*
	 * 1. name
	 */
	unistr name() const; // note: this is current name, contains modification haven't been committed yes.
	void chname(const unistr&); // note: this should check whether the "new" name is identical with the current one.
	/*
	 * 2. primary name
	 */
	unistr pname() const;
	void chpname(const unistr&);
	/*
	 * 3. alias
	 */
	taglist_t alias() const; // show
	void merge(const tag_t&); // add
	//void mkindep(); // sepearte, "indep" stands for indepedent, don't impt temporary.
	/*
	 * 4. tag
	 */
	void addtag(const tag_t&);
	void removetag(const tag_t&);

	/*
	 * 5. comments
	 */
	unistr comment() const;
	void editcomment(const unistr&);

	bool commit_changes();
	bool discard_changes();
private:
	Database* db_;

	tag_t original_tag_;
	tnode_t original_tnode_;

	bool edited_;

	tag_t edited_tag_;
	tnode_t edited_tnode_;

	/*
	 * We need a pool to save them
	 * leave as todo
	 */
	kstack<tag_edit_action> undo_stack_;
	kstack<tag_edit_action> redo_stack_;
	std::vector<idx_t> merging_tnodes_;
};

#endif
