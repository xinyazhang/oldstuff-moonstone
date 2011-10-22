#include <stdint.h>
#include "binlayerout.h"
#include "common.h"
#include <QtCore/QDataStream>

static bool scan(QDataStream& stream, uint8_t type)
{
	uint8_t peek;
	while (!stream.atEnd())
	{
		stream >> peek;
		if (peek == type)
		{
			stream.skipRawData(sizeof(uint32_t));
			return true;
		}
		uint32_t skip;
		stream >> skip;
		stream.skipRawData(skip);
	}
	return false;
}

QDataStream& operator<< (QDataStream& stream, const tnode_t& tnode)
{
	stream << LOBLT_TNODE;
	uint32_t pll = sizeof(idx_t);
	stream << pll;
	stream << tnode.idx;

	return stream;
}

QDataStream& operator>> (QDataStream& stream, tnode_t& tnode)
{
	if (scan(stream, LOBLT_TNODE))
		stream >> tnode.idx;
	return stream;
}

QDataStream& operator<< (QDataStream& stream, const fso_t& fso)
{
	stream << LOBLT_FILE;
	uint32_t pll = sizeof(idx_t) * 2;
	stream << pll;
	stream << fso.fsoid() << fso.parentid();

	return stream;
}

QDataStream& operator>> (QDataStream& stream, fso_t& fso)
{
	if (scan(stream, LOBLT_TNODE))
		stream >> fso.fsoid_ >> fso.parent_;
	return stream;
}
