#include "testKernels.h"

#include "Tag.h"
#include "TagMan.h"
#include "Tnode.h"
#include "TnodeMan.h"

#include "../dal/Database.h"

void TestKernels::initTestCase()
{
	db_ = new Database();
	db_->initialize(Database::Temporary);
	db_->buildTables();

	tagman_ = db_->tagman();
	tnodeman_ = db_->tnodeman();
}

void TestKernels::cleanupTestCase()
{
	delete db_;
}

void TestKernels::test010_tnode_create()
{
	unistr str = "Test, just a test";
	idx_ = tnodeman_->create(NULL, str);
	tnode_t tnode = tnodeman_->locate(idx);
	QVERIFY(tnode.comment == str);
}

void TestKernels::test020_tnode_update()
{
	tnode_t tnode = tnodeman_->locate(idx_);
	tnode.comment = "Updated test";
	tnodeman_->update(tnode);
	tnode = tnodeman_->locate(idx_);
	QCOMPARE(tnode.comment, "Updated test");
}

void TestKernels::test030_tnode_refcount()
{
	tnode_t tnode_1 = tnodeman_->locate(idx_);

	tnodeman_->refcinc(idx_);
	tnode_t tnode_2 = tnodeman_->locate(idx_);
	QVERIFY(tnode_2.refc == (tnode_1.refc + 1));

	tnodeman_->refcdec(idx_);
	tnode_t tnode_3 = tnodeman_->locate(idx_);
	QVERIFY(tnode_3.refc == (tnode_1.refc + 1));

	while ( tnode_3.refc > 1 )
	{
		tnodeman_->refcdec(idx_);
		tnode_3 = tnodeman_->locate(idx_);
	}
	tnodeman_->refcdec(idx_);
	tnode_t final = tnodeman_->locate(idx_);
	QVERIFY(TnodeMan::invalid(final));
}

void TestKernels::test100_tag_create()
{
	tag_t tag = tagman_->create("tagme");
	QVERIFY(!TagMan::invalid(tag));
	bool found = false;
	taglist_t taglist = tagman_->locate("tagme");
	for(taglist_t::const_iterator iter = taglist.begin();
			iter != taglist.end();
			iter++)
	{
		if (*iter == tag)
		{
			found = true;
			break;
		}
	}
	QVERIFY(found);
}

void TestKernels::test110_tag_hardlink()
{
	taglist_t taglist = tagman_->locate("tagne"); // unistr_list would construct from a single string
	QVERIFY(taglist.size() == 1);
	tag_t tag = taglist.front();
	tag_t tag2 = tagman_->hardlink(tag, "tagme2"); // note: TagMan would call TnodeMan to create a tnode
	QVERIFY(tag.tnode == tag2.tnode);
}

void TestKernels::test115_tag_update()
{
	taglist_t taglist = tagman_->locate("tagne"); // unistr_list would construct from a single string
	QVERIFY(taglist.size() == 1);
	tag_t tag = taglist.front();
	tag_t tagnew = tag;
	tagnew.name = "tagme";
	tagman_->update(tag, tagnew);

	taglist = tagman_->locate("tagme");
	QVERIFY( taglist.size() == 1 && tag.tnode == taglist.front().tnode );
}

void TestKernels::test120_tag_delete()
{
	/* 
	 * delete the first tag "tagme"
	 */
	taglist_t taglist = tagman_->locate("tagme");
	QVERIFY(taglist.size() == 1);
	tag_t tag = taglist.front();
	tagman_->del(tag);

	/* 
	 * delete the second tag "tagme2"
	 */
	taglist = tagman_->locate("tagme2");
	QVERIFY(taglist.size() == 1);
	tagman_->del(taglist.front());

	/*
	 * verify there the tnode is deleted
	 */
	tnode_t final = tnodeman_->locate(tag.idx);
	QVERIFY(TnodeMan::invalid(final));
}
