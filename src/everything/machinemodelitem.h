#ifndef MACHINE_MODEL_ITEM_H
#define MACHINE_MODEL_ITEM_H

#include "treeitem.h"

struct machine_t;
struct access_point;
struct volume;

class MachineRoot
	:public TreeItem
{
public:
	MachineRoot();
	~MachineRoot();
protected:
	/* To create new MachineItem */
	virtual TreeItem* spawnChild(int position, int columns);
	/* MachineRoot doesn't have a backend */
};

class MachineItem
	:public TreeItem
{
public:
	MachineItem(const machine_t&);
	~MachineItem();
protected:
	/* To create new APItem */
	virtual TreeItem* spawnChild(int position, int columns);
	virtual int removeAtBackend();
	virtual int addAtBackend();
	virtual int updateAtBackend():
};

class APItem
	:public TreeItem
{
public:
	APItem(MachineItem*, const access_point&);
	~APItem();
protected:
	/* return NULL as no children for APs */
	virtual TreeItem* spawnChild(int position, int columns) { return NULL; }
	virtual int removeAtBackend();
	virtual int addAtBackend();
	virtual int updateAtBackend():
};

#endif
