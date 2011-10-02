#ifndef LAIN_GUI_GENERAL_EXPLORER_H
#define LAIN_GUI_GENERAL_EXPLORER_H

#include <QtGui/QWidget>

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
	void install_prop(QWidget*);
	~GeneralExplorer();
public slots:
	void launch_search();
private:
	SearchHandler* sh_;
	Ui::GeneralExplorer* ui_;
	QWidget* prop_;
};

#endif
