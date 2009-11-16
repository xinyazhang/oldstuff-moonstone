#include "Tag.h"

TagParameters::TagParameters()
{
}

TagParameters& TagParameters::arg(const UniStr& name)
{
	push(UniStr("name"), name);
	return *this;
}

TagParameters& TagParameters::arg_family(const UniStr& family)
{
	push(UniStr("family"), family);
	return *this;
}
