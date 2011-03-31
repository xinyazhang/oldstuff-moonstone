#ifndef LAIN_GUI_TAG_DOCK_WINDOW_H
#define LAIN_GUI_TAG_DOCK_WINDOW_H

#include <QtGui/QDockWidget>

namespace Ui
{
	class TagEditor;
};

class RelationModel;
class ConformWidget;

class TagDock
	:public QDockWidget
{
	Q_OBJECT;
public:
	TagDock();
	~TagDock();

public slots:
	void select_target(tag_t);

private:
	Ui::TagEditor* ui_;

	RelationModel* relations_;
	ConformWidget* conforms_;
};

#endif
