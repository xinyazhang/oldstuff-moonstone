#include "KBView-tag.h"
#include "KBView-tag-file-inter.h"
#include <kernel/common.h>
#include "KBSearchResult.hpp"

KBViewTag::KBViewTag(Database* db, const tnode_t& tnode, KBViewItem* parent)
	:KBViewItem(parent), tnode_(tnode)
{
	KBViewTag::reload(db);
}


QVariant KBViewTag::col_data(Database* db, int col) const
{
	QString ret;
	if ( col == 0 )
	{
		ret = tnode_.mastername;
		// ret = db->tnodeman()->locate(idx_).mastername;
	}else
	{
		ret = cache_col1_;
	}
	return QVariant(ret);
}

struct tnode_tagrelbase_ks_meta
{
	typedef tnode_t ks_type;
	typedef tnode_t e_type;
	typedef KBViewTag cv_type;

	static bool empty_ks(const ks_type&)
	{
		return false; // No ``Search ALL''
	}

	static distinctlist_t<e_type> all(Database* db)
	{
		return distinctlist_t<e_type>(); // wanna "ALL"? Impossible!
	}

	// static distinctlist_t<e_type> select(Database* db, const ks_type& tnode)
	// It is left blank in the base class
};

struct tnode_taggeeks_meta
	:public tnode_tagrelbase_ks_meta
{
	// Only this one is OK
	static distinctlist_t<e_type> select(Database* db, const ks_type& tnode)
	{
		return db->relman()->taggee(tnode.idx);
	}
	static const char* header[2];
};
const char* tnode_taggeeks_meta::header[2] = {"Tags taged by this", ""};

struct tnode_taggerks_meta
	:public tnode_tagrelbase_ks_meta
{
	static distinctlist_t<e_type> select(Database* db, const ks_type& tnode)
	{
		return db->relman()->tagger(tnode.idx);
	}
	static const char* header[2];
};
const char* tnode_taggerks_meta::header[2] = {"Tags taged this", ""};

void KBViewTag::create_child(Database* db, int index)
{
	if (index == 0)
	{
		children_[index] = shared_ptr<KBViewItem>(
				KBSearchResult<tnode_taggeeks_meta>::Factory(db, tnode_, this));
	} else
	{
		children_[index] = shared_ptr<KBViewItem>(
				KBSearchResult<tnode_taggerks_meta>::Factory(db, tnode_, this));
	}
#if 0 //commented temporary
	if (index == 0)
	{
		/*
		 * The first child: files tagged by this tag
		 */
		children_[index] = shared_ptr<KBViewItem>(new KBViewTFI(db, idx_, this));
	} else
	{
		/*
		 * Tags tageed by this
		 */
		children_[index] = 
			shared_ptr<KBViewItem>(new KBViewTag(db, taggees_[index].idx, this));
	}
#endif
}

int KBViewTag::children_count(Database*) const
{
	return 2;
	// return (int)taggees_.size();
}

void KBViewTag::reload(Database* db)
{
	if ( TnodeMan::invalid(tnode_) )
		return ; // don't load placholder view
	names_ = db->tnodeman()->names(tnode_.idx);
	cache_col1_ = "";
	for(taglist_t::const_iterator iter = names_.begin();
			iter != names_.end();
			iter++)
	{
		cache_col1_ += iter->name;
		cache_col1_ += " ";
	}
	/*
	taggees_ = db->relman()->taggee(idx_);
	*/
	children_.clear();
	children_.resize(KBViewTag::children_count(db));
}

/*
 * It seems root factory is meaningless
 */

/*
KBViewTag* KBViewTag::RootFactory(Database* db, const unistr_list& ul)
{
	KBViewTag* ret = new KBViewTag(db, 0, NULL);
	if ( ul.empty() )
	{
		ret->taggees_ = db->tnodeman()->all();
	} else
	{
		taglist_t tl = db->tagman()->locate(ul);
		for(taglist_t::iterator iter = tl.begin();
				iter != tl.end();
				iter++)
		{
			ret->taggees_.push_back(db->tagman()->access_tnode(*iter));
		}
	}
	return ret;
}
*/
void KBViewTag::append_binary_layerout(QDataStream& ds) const
{
	ds << LOBLT_TNODE << tnode_.idx;
}
