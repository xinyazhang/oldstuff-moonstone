#include "dbmeta.h"
#include <QtCore/QXmlStreamReader>

bool dbmeta_t::read(QXmlStreamReader& reader, shared_ptr<dbmeta_t>& retv)
{
	if ( !reader.isStartElement() )
		return false;
	if ( reader.name() != "Database" )
	{
		QString m = reader.name().toString();
		return false;
	}
	if ( reader.attributes().value("Type") != database_typename() )
		return false;
	//reader.readNextStartElement();
	return parse(reader, retv);
}
