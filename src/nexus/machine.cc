#include "machine.h"

bool machine_t::valid() const
{
	if (id < 0)
		return false;
	return true;
}

machine_t::machine_t()
	:id(-1)
{
}
