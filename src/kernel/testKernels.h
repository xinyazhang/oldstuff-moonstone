#ifndef KERNEL_TEST_H
#define KERNEL_TEST_H

#include <QtTest/QtTest>
#include "../pal/stdtype.h"
#include "common_declare.h"

class TagMan;
class TnodeMan;

/*
 * Assuming the database has been tested.
 */
class TestKernels
	:public QObject
{
	Q_OBJECT;
public:
	TestKernels()
	{
		//initTestCase();
	}
private slots:
	void initTestCase();
	void test010_tnode_create();
	void test020_tnode_update();
	void test030_tnode_refcount();
	void test100_tag_create();
	void test110_tag_hardlink();
	void test115_tag_update();
	void test120_tag_delete();
	void cleanupTestCase();
private:
	Database* db_;
	TagMan* tagman_;
	TnodeMan* tnodeman_;
	idx_t idx_;
};

#endif
