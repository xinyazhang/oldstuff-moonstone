/****************************************************************************
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

/*
   treeitem.cpp

   Base class of all tree item
   */

#include <QStringList>
#include "treeitem.h"

TreeItem::TreeItem(TreeItem *parent)
	:parentItem(parent), status(NoChange)
{
}

TreeItem::~TreeItem()
{
	qDeleteAll(childItems);
	qDeleteAll(removedItems);
}

TreeItem *TreeItem::child(int number)
{
	return childItems.value(number);
}

int TreeItem::childCount() const
{
	return childItems.count();
}

int TreeItem::childNumber() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}

int TreeItem::columnCount() const
{
	return itemData.count();
}

QVariant TreeItem::data(int column) const
{
	return itemData.value(column);
}

QVariant TreeItem::data(int column, int role) const
{
	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();
	return itemData.value(column);
}

bool TreeItem::insertChildren(int position, int count, int columns)
{
	if (position < 0 || position > childItems.size())
		return false;

	for (int row = 0; row < count; ++row) {
		TreeItem *item = spawnChild(position, columns);
		if (item) {
			childItems.insert(position, item);
			item->currentChangeStatus = Added;
		}
	}

	return true;
}

virtual TreeItem* TreeItem::spawnChild(int, int columns)
{
	QVector<QVariant> data(columns);
	TreeItem *item = new TreeItem(data, this);
	return item;
}

bool TreeItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > childItems.size())
		return false;

	for (int row = 0; row < count; ++row) {
		TreeItem *takenItem = childItems.takeAt(position);
		if (takenItem && takenItem->changeStatus() != Added) {
			removedItems.append(takenItem);
			takenItem->currentChangeStatus = Removed;
			/* Cascade */
			takeItem->removeChildren(0, takenItem->childCount());
		}
	}

	return true;
}

TreeItem *TreeItem::parent()
{
	return parentItem;
}

bool TreeItem::insertColumns(int position, int columns)
{
	if (position < 0 || position > itemData.size())
		return false;

	for (int column = 0; column < columns; ++column)
		itemData.insert(position, QVariant());

	foreach (TreeItem *child, childItems)
		child->insertColumns(position, columns);

	return true;
}

bool TreeItem::removeColumns(int position, int columns)
{
	if (position < 0 || position + columns > itemData.size())
		return false;

	for (int column = 0; column < columns; ++column)
		itemData.remove(position);

	foreach (TreeItem *child, childItems)
		child->removeColumns(position, columns);

	return true;
}

bool TreeItem::setData(int column, const QVariant &value)
{
	if (column < 0 || column >= itemData.size())
		return false;

	itemData[column] = value;

	if (status != Added)
		status = Updated;
	return true;
}

int TreeItem::changeStatus() const
{
	return status;
}

int TreeItem::discardChanges() const
{
	itemData = initData;
	foreach(TreeItem* item, childItems) {
		if (item->changeStatus() == Added)
			delete item;
	}
	childItems = initItems;
	foreach(TreeItem* item, childItems) {
		item->discardChanges();
	}
	removedItems.clear();
	status = NoChange;
}

int TreeItem::applyChanges() const
{
	initData = itemData;
	foreach(TreeItem* item, childItems) {
		switch (item->changeStatus()) {
			case Added:
				item->addAtBackend();
				break;
			case Updated:
				item->updateAtBackend();
				break;
		}
		item->applyChanges();
	}
	foreach(TreeItem* item, removedItems) {
		item->applyChanges();
		item->removeAtBackend();
	}
}

virtual int TreeItem::removeAtBackend();
virtual int TreeItem::addAtBackend();
virtual int TreeItem::updateAtBackend():
