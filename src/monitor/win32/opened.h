#ifndef OPENDE_DATA_H
#define OPENDE_DATA_H

#include <boost/scoped_ptr.hpp>

static struct opened_t
{
	HANDLE volume_handle;
	USN_JOURNAL_DATA cur_usn_meta,
	READ_USN_JOURNAL_DATA usn_selecting,
	OVERLAPPED overlap;
	boost::scoped_ptr<char> usn_buffer,
	DWORD usn_read_bytes,
	int handle_index;
};

#endif
