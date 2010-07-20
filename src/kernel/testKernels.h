#ifndef KERNEL_TEST_H
#define KERNEL_TEST_H

#include <QtTest/QtTest>
#include "../pal/stdtype.h"
#include "common_declare.h"
#include "common.h"

class TagMan;
class TnodeMan;
//class TagManContext;

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
	//void test200_tmc_clean_start();

	void test300_ttr_create(); // tag tag relation
	void test310_ttr_probe();
	void test320_ttr_remove();

	void cleanupTestCase();
private:
	Database* db_;
	TagMan* tagman_;
	TnodeMan* tnodeman_;
	RelationMan* relman_;
	//TagManContext* tmc_;
	idx_t idx_;

	tag_t tmptag1, tmptag2;
};

#endif
