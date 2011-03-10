#include "LainMain.h"
#include "ui_lain.h"

LainMain::LainMain()
	:ui_(new Ui::LainMain)
{
	ui_->setupUi(this);
}

LainMain::~LainMain()
{
	delete ui_;
}
