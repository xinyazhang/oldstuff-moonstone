#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "partition.h"
#include "tracing.h"

#include <boost_arctype.h> //should include any archive files before serialization 

#include <boost/uuid/uuid_serialize.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

#include <string>

namespace boost {
namespace serialization {

template<class archive>
void serialize(archive & ar, unistr& str, const unsigned int)
{
	std::wstring wstr;
	ar & wstr;

	unistr = wstr.c_str();
}

template<class archive>
void serialize(archive & ar, partition& part, const unsigned int)
{
    ar & make_nvp("KPI", part.kpi);
    ar & make_nvp("UUID", part.uuid);
	ar & make_nvp("LOCATORS", part.locators); // it seems a std::map didn't need it ..
	/* device is NOT stored as its a runtime value */
}


template<class archive>
void serialize(archive& ar, partition_list& plist, const unsigned int /* file_version */)
{
	clear();
	size_t count;
	ar & BOOST_SERIALIZATION_NVP(count);
	ar & BOOST_SERIALIZATION_NVP(kpi_last);
	for(size_t i = 0; i < count; i++)
	{
		partition* part = new partition;
		ar & make_nvp("partition", *part);
		plist.push_back(boost::shared_ptr<partition>(part));
	}
}

template<class archive>
void serialize(archive& ar, path_internal& path, const unsigned int)
{
	uuid_t uuid;
	ar & uuid;
	ar & path.path;
	path.partition = locate_partition(known_partitions, uuid);
}

} // namespace serialization
} // namespace boost
#endif
