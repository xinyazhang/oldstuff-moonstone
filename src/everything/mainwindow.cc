#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchfiledbmodel.h"
#include "volumemodel.h"
#include "Preferences.h"
#include <QtGui/QDialogButtonBox>
#include "QFeedbackEvent.h"
#include <kernel/eventd.h>
#include <kernel/Database.h>
#include <kernel/volmgr.h>

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
	mac_model = new MachineModel;
	ui->machine_list->setModel(mac_model);

	procd().register_receiver(volprog, this);
}

MainWindow::~MainWindow()
{
	procd().supress_receiver(volprog, this);
	delete mac_model;
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
		double p = pref()->db_mgr->volmgr()->progress() * 10000.0;
		ui->index_prog->setValue((int)p);
		return true;
	} else {
		return QMainWindow::event(e);
	}
}

static int volprog(void* cookie, feedback_event ev)
{
	if (ev.evid != INDEX_PROGRESSED)
		return FBEV_IGNORE;
	MainWindow* mw = (MainWindow*)cookie;
	QCoreApplication::postEvent(mw, new QFeedbackEvent(ev));
	return FBEV_ACCEPT;
}

void MainWindow::on_buttonBox_2_clicked(QAbstractButton *button)
{
	QDialogButtonBox::StandardButton sb = ui->buttonBox_2->standardButton(button);
    if (sb == QDialogButtonBox::Save)
            mac_model->applyChanges();
	else if (sb == QDialogButtonBox::Discard)
            mac_model->discardChanges();
}

void MainWindow::on_m_newmac_released()
{
     QModelIndex index = ui->machine_list->selectionModel()->currentIndex();
     mac_model->insertRow(index.row()+1);
     for (int column = 0; column < mac_model->columnCount(index.parent()); ++column) {
         QModelIndex child = mac_model->index(index.row()+1, column, index.parent());
         mac_model->setData(child, QVariant("[No data]"), Qt::EditRole);
     }
}

void MainWindow::on_mv_newap_released()
{
     QModelIndex index = ui->machine_list->selectionModel()->currentIndex();
	 /*
	  * Index's parent is valid
	  * i.e AP is selected
	  */
	 if (index.parent().isValid())
		 index = index.parent();

	 mac_model->insertRow(0, index);
     for (int column = 0; column < mac_model->columnCount(index.parent()); ++column) {
         QModelIndex child = mac_model->index(0, column, index);
         mac_model->setData(child, QVariant("[No data]"), Qt::EditRole);
     }
}

void MainWindow::on_mv_del_released()
{
     QModelIndex index = ui->machine_list->selectionModel()->currentIndex();
     mac_model->removeRow(index.row(), index.parent());
}
