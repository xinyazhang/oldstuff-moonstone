#ifndef _KERNEL_DECLARES_
#define _KERNEL_DECLARES_

#include "../core/UniStr.h"
#include <boost/shared_ptr.hpp>

class Tag;
typedef boost::shared_ptr<Tag> TagRef;

class TagNode;
typedef boost::shared_ptr<TagNode> TagNodeRef;

class TagIterator;
//typedef boost::shared_ptr<TagIterator> TagIteratorRef;

class Family;
typedef boost::shared_ptr<Family> FamilyRef;

class TagAliasGroup;
typedef boost::shared_ptr<TagAliasGroup> TagAliasGroupRef;

class AttrFamily;
typedef boost::shared_ptr<AttrFamily> AttrFamilyRef;

class Resource;
typedef boost::shared_ptr<Resource> ResourceRef;

class Container;
typedef boost::shared_ptr<Container> ContainerRef;

class TagBind;
typedef boost::shared_ptr<TagBind> TagBindRef;

class Attr;
typedef boost::shared_ptr<Attr> AttrRef;

class Database;
class Category;
class Parameters;

#endif
