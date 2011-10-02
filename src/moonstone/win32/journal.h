#ifndef JOURNAL_WIN32_H
#define JOURNAL_WIN32_H

#include <windows.h>

struct journal_t
{
	DWORDLONG journal_id_last_seen;
	USN last_recorded_usn;
};

namespace boost {
namespace serialization {

template<class archive>
void serialize(archive& ar, journal_t& journal, const unsigned int)
{
	ar & make_nvp("ntfs_journal_id", journal.journal_id_last_seen);
	ar & make_nvp("ntfs_last_accessed_uid", journal.last_recorded_usn);
}

} // namespace serialization
} // namespace boost

#endif
