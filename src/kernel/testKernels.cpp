#include "testKernels.h"

#include "Tag.h"
#include "TagMan.h"
#include "Tnode.h"
#include "TnodeMan.h"

#include "Database.h"
#include "../dal/db_sqlite_impl.h"

void TestKernels::initTestCase()
{
	db_sqlite_impl* p;
	db_ = new Database(p = new db_sqlite_impl(":memory:", ""));
	p->connect();
	if ( !db_->initialized() )
		QVERIFY(db_->initialize());

	tagman_ = db_->tagman();
	tnodeman_ = db_->tnodeman();
}

void TestKernels::cleanupTestCase()
{
	system("pause");
	delete db_;
}

void TestKernels::test010_tnode_create()
{
	unistr str = "Test, just a test";
	idx_ = tnodeman_->create(NULL, str);
	tnode_t tnode = tnodeman_->locate(idx_);
	QVERIFY(tnode.comment == str);
}

void TestKernels::test020_tnode_update()
{
	tnode_t tnode = tnodeman_->locate(idx_);
	tnode.comment = "Updated test";
	tnodeman_->update(tnode);
	tnode = tnodeman_->locate(idx_);
	QVERIFY(tnode.comment == unistr("Updated test"));
}

void TestKernels::test030_tnode_refcount()
{
	tnode_t tnode_1 = tnodeman_->locate(idx_);

	tnodeman_->refcinc(idx_);
	tnode_t tnode_2 = tnodeman_->locate(idx_);
	QVERIFY(tnode_2.refc == (tnode_1.refc + 1));

	tnodeman_->refcdec(idx_);
	tnode_t tnode_3 = tnodeman_->locate(idx_);
	QVERIFY(tnode_3.refc == (tnode_1.refc));

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
	taglist_t taglist = tagman_->locate("tagme"); // unistr_list would construct from a single string
	QVERIFY(taglist.size() == 1);
	tag_t tag = taglist.front(); 
	tag_t tag2 = tagman_->hardlink(tag, "tagme2"); // note: TagMan would call TnodeMan to create a tnode
	tag = tagman_->locate("tagme").front(); // don't use the original tag, as the hardlink may trigger the tnode section.
	QVERIFY(tag.tnode == tag2.tnode);
}

void TestKernels::test115_tag_update()
{
	taglist_t taglist = tagman_->locate("tagme2"); // unistr_list would construct from a single string
	QVERIFY(taglist.size() == 1);
	tag_t tag = taglist.front();
	tag_t tagnew = tag;
	tagnew.name = "tagme_hardlink";
	tagman_->update(tag, tagnew);

	taglist = tagman_->locate("tagme_hardlink");
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
	 * delete the second tag "tagme_hardlink"
	 */
	taglist = tagman_->locate("tagme_hardlink");
	QVERIFY(taglist.size() == 1);
	tagman_->del(taglist.front());

	/*
	 * verify there the tnode is deleted
	 */
	tnode_t final = tnodeman_->locate(tag.tnode);
	QVERIFY(TnodeMan::invalid(final));
}

 QTEST_MAIN(TestKernels);
