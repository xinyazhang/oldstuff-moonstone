#include "logusn.h"
#include "../serialization.h"
#include <stdio.h>
#include "../packet_handler.h"
#include <pal/ipc.h>
#include <windows.h>

#define LOGUSN_FILE_SIZE_LIMIT 10485760

using boost::serialization::make_nvp;

int lastfid;
#if 0
static int serv_journal_req(ipc_packet*, ph_cookie);
static int serv_journal_ack(ipc_packet*, ph_cookie);
#endif

void logusn_init()
{
#if 0
	lastfid = 0;
	ph_register(PT_PRIV_JOURNAL_REQUEST, serv_journal_req);
	ph_register(PT_PRIV_JOURNAL_ACK, serv_journal_ack);
#endif
}

void logusn_load(barc_i& ar)
{
	ar >> make_nvp("LATEST_USN_JOURNAL_DUMP_FILE", lastfid);
}

void logusn_save(barc_o& ar)
{
	ar << make_nvp("LATEST_USN_JOURNAL_DUMP_FILE", lastfid);
}

static unistr fid2filename(const unistr& varpath, int fid)
{
	unistr filename(varpath);
	filename += L"/UJDUMP";
	filename += unistr::number(fid);
	filename += L".journal";

	return filename;
}


class logusn_write_context_t
	:public logusn_context_t
{
public:
	logusn_write_context_t(const unistr& varpath, int fid)
		:vp(varpath)
	{
		unistr fn = fid2filename(varpath, fid);
		fd_ = _wfopen(fn.native(), L"ab");
	}
	~logusn_write_context_t()
	{
		fclose(fd_);
	}

	virtual void ev_uuid(uuid_t& uuid)
	{
		if (uuid != cur_uuid_)
		{
			cur_uuid_ = uuid;
			write_ev(SWITCH_VOLUME);
			fwrite(uuid.data, sizeof(uuid.data), 1, fd_);
			checksize();
		}
	}

	virtual void ev_jid(uint64_t& jid)
	{
		write_ev(JOURNALRUPT);
		fwrite(&jid, sizeof(jid), 1, fd_);

		checksize();
	}

	virtual void ev_usn_records(USN& usn, char* buf, size_t& bytes)
	{
		write_ev(USN_AND_RECORD);
		fwrite(&usn, sizeof(usn), 1, fd_);

		uint32_t size_in_u32 = bytes;
		fwrite(&size_in_u32, sizeof(size_in_u32), 1, fd_);
		fwrite(buf, 1, bytes, fd_);

		checksize();
	}
private:
	FILE* fd_;
	uuid_t cur_uuid_;
	unistr vp;

	void write_ev(SQLUSN_EVENT_TYPE event)
	{
		uint32_t ev = event;
		fwrite(&ev, sizeof(uint32_t), 1, fd_);
	}

	void checksize()
	{
		int64_t pos = _ftelli64(fd_);
		if (pos >= LOGUSN_FILE_SIZE_LIMIT)
		{
			fclose(fd_);
			unistr fn = fid2filename(vp, lastfid++);
			fd_ = _wfopen(fn.native(), L"ab");
		}
	}
};

logusn_context_t* logusn_create_for_write(const unistr& varpath)
{
	return new logusn_write_context_t(varpath, lastfid);
}

class logusn_read_context_t
	:public logusn_context_t
{
public:
	logusn_read_context_t(const unistr& fn)
	{
		fd_ = _wfopen(fn.native(), L"rb");
	}
	~logusn_read_context_t()
	{
		fclose(fd_);
	}

	virtual void seekg(int64_t offset)
	{
		_fseeki64(fd_, offset, SEEK_SET);
	}

	virtual SQLUSN_EVENT_TYPE cur_ev()
	{
		uint32_t ev;
		fread(&ev, sizeof(ev), 1, fd_);
		return (SQLUSN_EVENT_TYPE)ev;
	}

	virtual void ev_uuid(uuid_t& uuid)
	{
		fread(uuid.data, sizeof(uuid.data), 1, fd_);
	}

	virtual void ev_jid(uint64_t& jid)
	{
		fread(&jid, sizeof(jid), 1, fd_);
	}

	virtual void ev_usn_records(USN& usn, char* buf, size_t& bytes)
	{
		fread(&usn, sizeof(usn), 1, fd_);

		uint32_t size_in_u32;
		fread(&size_in_u32, sizeof(size_in_u32), 1, fd_);
		if (bytes < size_in_u32)
		{
			bytes = 0;
			fseek(fd_, - (sizeof(usn)+sizeof(size_in_u32)), SEEK_CUR);
			return ;
		}
		bytes = size_in_u32;
		fread(buf, 1, bytes, fd_);
	}
private:
	FILE* fd_;
};

logusn_context_t* logusn_create_for_read(const unistr& varpath, int fid, int64_t initseek)
{
	logusn_read_context_t* ctx = new logusn_read_context_t(fid2filename(varpath, lastfid));
	ctx->seekg(initseek);
	return ctx;
}

#if 0
struct journal_req_t
{
	ipc_packet_header header;
	int32_t fid;
};

static int serv_journal_req(ipc_packet* packet, ph_cookie cookie)
{
	native_fd fd = (native_fd)cookie;
	journal_req_t* req = (journal_req_t*)packet;
	unistr fn = fid2filename(varpath, req->fid);
	HANDLE file = CreateFileW(fn.native(),
			GENERIC_READ,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
	ipc_send_fd(fd, file);
	CloseHandle(file);
	return 0;
}

struct journal_ack_t
{
	ipc_packet_header header;
	int32_t fid;
};

static int serv_journal_ack(ipc_packet*, ph_cookie)
{
	journal_req_t* req = (journal_req_t*)packet;
	unistr fn = fid2filename(varpath, req->fid);
	DeleteFileW(fn.native());
}
#endif