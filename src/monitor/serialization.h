#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "partition.h"
#include "tracing.h"

#include <boost_arctype.h> //should include any archive files before serialization 
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

#include <string>

#include <journal.h>

namespace boost {
namespace serialization {

template<class archive>
void load(archive & ar, unistr& str, const unsigned int version)
{
	std::wstring wstr;
	ar & make_nvp("value", wstr);
	str = wstr;
}

template<class archive>
void save(archive & ar, const unistr& str, const unsigned int version)
{
	std::wstring wstr = str.c_str();
	ar & make_nvp("value", wstr);
}

template<class archive>
void serialize(archive & ar, unistr& str, const unsigned int version)
{
	boost::serialization::split_free(ar, str, version);
	//ar & make_nvp("value", str);
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
void load(archive& ar, partition_list& plist, const unsigned int /* file_version */)
{
	plist.clear();
	size_t count;
	ar >> BOOST_SERIALIZATION_NVP(count);
	for(size_t i = 0; i < count; i++)
	{
		partition* part = new partition;
		ar >> make_nvp("partition", *part);
		plist.push_back(boost::shared_ptr<partition>(part));// for
	}
}

template<class archive>
void save(archive& ar, const partition_list& plist, const unsigned int /* file_version */)
{
	size_t count = plist.size();
	ar << BOOST_SERIALIZATION_NVP(count);
	for(size_t i = 0; i < count; i++)
	{
		ar << make_nvp("partition", *plist[i]);
	}
}

template <class archive>
void serialize(archive & ar, partition_list& c, const unsigned int version)
{
	boost::serialization::split_free(ar, c, version);
} 

template<class archive>
void serialize(archive& ar, path_internal& path, const unsigned int)
{
	uuid_t uuid;
	if (path.partition)
		uuid = path.partition->uuid;
	ar & make_nvp("uuid", uuid);
	ar & make_nvp("path", path.paths);
	ar & make_nvp("journal_status", path.journal_status);
	if (path.journal_status == JOURNAL_ENABLED)
		ar & make_nvp("journal", path.journal);
	path.partition = locate_partition(known_partitions, uuid);
}

} // namespace serialization
} // namespace boost
#endif
