#include <QtCore/QObject>
#include <kernel/machine.h>
#include <kernel/access_point.h>
#include <kernel/macmgr.h>
#include <pal/volume.h>
#include "machinemodelitem.h"
#include "Preferences.h"
#include <kernel/Database.h>

const int machine_display_items = 2;
const int ap_display_items = 2;
static const int display_items = 2;
static const char* TR[] = {"Name", "Comment"};

#define MACMGR (pref()->db_mgr->macmgr())

MachineRoot::MachineRoot()
{
	/* Load Machines */
	macmgr_t* macmgr = MACMGR;
	machine_t mac = macmgr->macfirst();
	while (mac.valid()) {
		MachineItem *child = new MachineItem(this, mac);
		childItems.append(child);
		mac = macmgr->macnext(mac);
	}
	initItems = childItems;

	itemData.resize(display_items);
	itemData[0] = QObject::tr(TR[0]);
	itemData[1] = QObject::tr(TR[1]);
}

MachineRoot::~MachineRoot()
{
}

TreeItem* MachineRoot::spawnChild(int, int columns)
{
	return new MachineItem(this, machine_t());
}

MachineItem::MachineItem(MachineRoot* root, const machine_t& mac)
	:TreeItem(root), mac_(mac)
{
	itemData.resize(display_items);
	itemData[0] = QString(mac_.name);
	itemData[1] = QString(mac_.comment);

	access_point ap = MACMGR->apfirst();
	while (ap.valid()) {
		ap.phost = &mac_;
		APItem *child = new APItem(this, ap);
		childItems.append(child);
		ap = MACMGR->apnext(ap);
	}
}

MachineItem::~MachineItem()
{
}

TreeItem* MachineItem::spawnChild(int, int columns)
{
	return new APItem(this, access_point(&mac_));
}

int MachineItem::removeAtBackend()
{
	MACMGR->rm_machine(mac_);
	return 0;
}

int MachineItem::addAtBackend()
{
	MACMGR->add_machine(mac_);
	return 0;
}

int MachineItem::updateAtBackend()
{
	MACMGR->update_machine(mac_);
	return 0;
}

APItem::APItem(MachineItem* root, const access_point& ap)
	:TreeItem(root), ap_(ap)
{
	itemData.resize(display_items);
	itemData[0] = QString(ap_.url);
	itemData[1] = QString(ap_.comment);
}

APItem::~APItem() { }

int APItem::removeAtBackend()
{
	MACMGR->rm_ap(ap_);
	return 0;
}

int APItem::addAtBackend()
{
	MACMGR->add_ap(ap_);
	return 0;
}

int APItem::updateAtBackend()
{
	MACMGR->update_ap(ap_);
	return 0;
}
