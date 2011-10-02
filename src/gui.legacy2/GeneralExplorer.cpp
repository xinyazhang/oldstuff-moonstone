#include "GeneralExplorer.h"
#include "ui_general_explorer.h"
#include "SearchHandler.h"

GeneralExplorer::GeneralExplorer()
	:ui_(new Ui::GeneralExplorer), sh_(NULL)
{
	ui_->setupUi(this);
	connect(ui_->search_text, SIGNAL(returnPressed()),
			this, SLOT(launch_search()));
	ui_->result_tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui_->result_tree->setDragEnabled(true);
	/* used for debug */
	ui_->result_tree->setAcceptDrops(true);
	ui_->result_tree->setDropIndicatorShown(true);
}

void GeneralExplorer::install_sh(SearchHandler* sh)
{
	sh_ = sh;
	sh_->install_view(ui_->result_tree);
}

GeneralExplorer::~GeneralExplorer()
{
	delete ui_;
	delete sh_;
}

void GeneralExplorer::launch_search()
{
	sh_->launch_search(ui_->search_text->text().trimmed());
}

void GeneralExplorer::install_prop(QWidget* widget)
{
	ui_->horizontalLayout->addWidget(widget);
	prop_ = widget;
}
