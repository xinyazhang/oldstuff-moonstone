#ifndef LAIN_GUI_TAG_DOCK_WINDOW_H
#define LAIN_GUI_TAG_DOCK_WINDOW_H

#include <QtGui/QWidget>
#include <kernel/common.h>

namespace Ui
{
	class TagEditor;
};

class RelationModel;
class ConformWidget;
class DatabaseSession;

class TagDock
	:public QWidget
{
	Q_OBJECT;
public:
	TagDock(Database*);
	~TagDock();

public slots:
	void commit();
	void select_target(tag_t);
private:
	Ui::TagEditor* ui_;

	RelationModel* relations_;
	ConformWidget* conforms_;
	//QWidget* content_;
	Database* db_;
};

#endif
