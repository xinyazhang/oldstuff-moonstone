#ifndef LAIN_GUI_GENERAL_EXPLORER_H
#define LAIN_GUI_GENERAL_EXPLORER_H

#include <QtGui/QWidget>
#include "SearchHandler.h"

namespace Ui
{
	class GeneralExplorer;
};

class SearchHandler;

class GeneralExplorer
	:public QWidget
{
	Q_OBJECT;
public:
	GeneralExplorer();
	void install_sh(SearchHandler*);
	~GeneralExplorer();
public slots:
	void launch_search();
private:
	SearchHandler* sh_;
	Ui::GeneralExplorer* ui_;
};

#endif
