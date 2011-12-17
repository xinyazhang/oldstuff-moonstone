#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchfiledbmodel.h"
#include "volumemodel.h"
#include "Preferences.h"
#include <QtGui/QDialogButtonBox>
#include <kernel/eventd.h>
#include "QFeedbackEvent.h"

static int volprog(void* cookie, feedback_event ev);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	search_model = new SearchFileDBModel(pref());
	ui->result->setModel(search_model);
	vol_model = new VolumeModel(pref());
	ui->volume_list->setModel(vol_model);
	setWindowState(Qt::WindowMaximized);

	procd().register_receiver(volprog, this);
}

MainWindow::~MainWindow()
{
	delete vol_model;
	delete search_model;
	delete ui;
}

void MainWindow::on_searching_textChanged(const QString &arg1)
{
	search_model->change_searching_text(arg1);
}

void MainWindow::on_buttonBox_clicked(QAbstractButton *button)
{
	QDialogButtonBox::StandardButton sb = ui->buttonBox->standardButton(button);
	if (sb == QDialogButtonBox::Apply)
		vol_model->apply_changes();
	else if (sb == QDialogButtonBox::Reset)
		vol_model->clear_changes();
}

bool MainWindow::event(QEvent* e)
{
	if (e->type()== CVT_QEVENT(INDEX_PROGRESSED)) {
		double p = pref()->db_mgr->volmgr()->progress() * 100.0;
		ui->index_prog->setValue((int)p);
		return true;
	} else {
		return false;
	}
}

static int volprog(void* cookie, feedback_event ev)
{
	if (ev.evid != INDEX_PROGRESSED)
		return FBEV_IGNORE;
	MainWindow* mw = (MainWindow*)cookie;
	QCoreApplication::postEvent(mw, new QFeedbackEvent(ev));
}
