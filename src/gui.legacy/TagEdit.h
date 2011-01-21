#ifndef TAGEDIT_H
#define TAGEDIT_H

#include <QtGui/QWidget>
#include "../kernel/tag.h"
#include "../kernel/tnode.h"

namespace Ui
{
	class TagEdit;
};

class Database;
class TagSearchResultModel;
class TagManContext;
/*
 * This class receive request from user, and manipulate TagMan object for work
 */

class TagEdit
	:public QWidget
{
	Q_OBJECT;
public:
	TagEdit(QWidget* parent = NULL);
	~TagEdit();

	void setup(Database* db); // should be call only once!

public slots:
	/*
	 * Carefully apart guitag from tag in db
	 * As we can see, console command can add a tag in one time,
	 * but GUI can't, like the difference between "mkdir" and "New Folder" in exploere
	 *
	 */
	void new_guitag();
	void save_to_dbtag();
	void edit_another();
	void search();
	void clear_focus();
private:
	Ui::TagEdit* ui;
	Database* db_;
	TagSearchResultModel* search_model_;
	TagManContext* context_;

	void makeSigSlotConnections();

#if 0
	enum State
	{
		NEW_TAG,
		EDIT_TAG
	} state_;
	bool switch_state(State state);
#endif

	enum Info
	{
		UNSAVED
	};

	bool edited_;
	tag_t editing_tag_;
	tnode_t editing_tnode_;

	void load_data_to_gui();
	void saveguard();

private slots:
	void change_tag_name();
	//void change_tag_pname(const QString&);
	void revert();
	//void make_primary();
	void undo();
	void redo();
private:
	void update_unredo_actions();
};


#endif
