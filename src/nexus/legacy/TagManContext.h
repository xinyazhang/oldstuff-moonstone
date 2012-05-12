#ifndef KERNEL_TAGMAN_CONTEXT_H
#define KERNEL_TAGMAN_CONTEXT_H

#include "common_declare.h"
#include "tag.h"
#include "tnode.h"
#include "tag_edit_action.h"
#include "kstack.h"
#include <hash_map>

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
#if 0 // removed as we use mkprimary
	void chpname(const unistr&);
#endif
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
	 * 	note: this won't lead to a tagedit creation -- 
	 * 	it has been supported by the edit controller of gui.
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
	//std::vector<idx_t> merging_tnodes_; // removed because of its complexity

public: // Modification exchange protocol's interface
	enum TNODEMODBIT
	{
		MOD_NAME = 0,
		MOD_ALIAS = 1,
		MOD_TAG = 2,
		MOD_PNAME = 4,
		MOD_COMMENT = 3,
		MOD_ROOT = 5 // make tnode for these tags
	};

	int last_mod() const;

	enum ALIASMOD
	{
		ALIAS_ADD = 1,
		ALIAS_RENAME = 2,
		ALIAS_REMOVE = 3,
		ALIAS_TEARDOWN = 4
	};

	ALIASMOD aliasmod() const;
	int aliasmod_place() const;
	unistr aliasmod_name() const;

	enum TAGGINGMOD
	{
		TAGGING = 1,
		UNTAGGING = 2
	};
	TAGGINGMOD taggingmod() const;
	idx_t taggingmod_tnode() const;
	// that's sufficient, other data can be obtained from attributes.
private:
	int last_; // last modification, use in kernel-UI protocol
	int accu_; // accumulating modifications, used in commit changes to db
	ALIASMOD aliasmod_type_;
	TAGGINGMOD taggingmod_type_;
	// TODO: use a hash map to store every tag's modifications
	struct TagMod
	{
		ALIASMOD type;
		unistr new_value;
	};
	//std::hash_map<unistr, TagMod> modmap_;
	std::vector<idx_t> merging_tnodes_;
};

#endif
