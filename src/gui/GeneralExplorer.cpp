#include "GeneralExplorer.h"
#include "ui_general_explorer.h"

GeneralExplorer::GeneralExplorer()
	:ui_(new Ui::GeneralExplorer), sh_(NULL)
{
	ui_->setupUi(this);
	connect(ui_->search_text, SIGNAL(returnPressed()),
			this, SLOT(launch_search()));
}

void GeneralExplorer::install_sh(SearchHandler* sh)
{
	sh_ = sh;
}

GeneralExplorer::~GeneralExplorer()
{
	delete ui_;
	delete sh_;
}

void GeneralExplorer::launch_search()
{
}
