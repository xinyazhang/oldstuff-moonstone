#include <QtCore/QObject>
#include <kernel/machine.h>
#include <kernel/access_point.h>
#include <kernel/macmgr.h>
#include <pal/volume.h>
#include "machinemodelitem.h"
#include "Preferences.h"

const int machine_display_items = 2;
const int ap_display_items = 2;
static const int display_items = 2;
static const unichar* TR[] = {UT("Name"), UT("Comment")};

#define MACMGR (perf()->db_mgr->macmgr())

MachineRoot::MachineRoot()
{
	/* Load Machines */
	macmgr_t* macmgr = MACMGR
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
	return new MachineItem(macmgr_t());
}

MachineItem::MachineItem(MachineRoot* root, const macmgr_t& mac)
	:TreeItem(root), mac_(mac)
{
	itemData.resize(display_items);
	itemData[0] = mac_.name;
	itemData[1] = mac_.comment;

	access_point ap = macmgr->apfirst();
	while (ap.valid()) {
		ap.phost = &mac_;
		APItem *child = new APItem(this, ap);
		childItems.append(child);
		ap = macmgr->apnext(ap);
	}
}

TreeItem* MachineItem::spawnChild(int, int columns)
{
	return new APItem(this, access_point(&mac));
}

int MachineItem::removeAtBackend()
{
	MACMGR->rm_machine(mac_);
}

int MachineItem::addAtBackend()
{
	MACMGR->add_machine(mac_);
}

int MachineItem::updateAtBackend()
{
	MACMGR->update_machine(mac_);
}

APItem::APItem(MachineItem* root, const access_point& ap)
	:TreeItem(root), ap_(ap)
{
	itemData.resize(display_items);
	itemData[0] = ap_.url;
	itemData[1] = ap_.comment;
}

int APItem::removeAtBackend()
{
	MACMGR->rm_ap(ap_);
}

int APItem::addAtBackend()
{
	MACMGR->add_ap(ap_);
}

int APItem::updateAtBackend()
{
	MACMGR->update_ap(ap_);
}
