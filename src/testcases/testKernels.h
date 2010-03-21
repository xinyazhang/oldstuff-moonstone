#ifndef _TESTCASE_KERNEL_H_
#define _TESTCASE_KERNEL_H_

#include <QtTest/QtTest>

class TagTest
	:public QObject
{
	Q_OBJECT
private slots:
	void test1();
};

class TnodeTest
	:public QObject
{
	Q_OBJECT
private slots:
	void test2();
};

#endif
