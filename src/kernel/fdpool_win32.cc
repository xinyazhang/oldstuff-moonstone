#include "fdpool.h"
#include "threadpool.h"
#include <pal/arch_march.h>
#include <pal/ioinfo.h>
#include <windows.h>

fdpool_win32::fdpool_win32()
{
	iocp_ = CreateIoComletionPort(INVALID_HANDLE_VALUE,
			NULL, 0, 0);
	tg_ = globaltp()->allocate_threads(cpu_number(), this);
}

fdpool_win32::~fdpool_win32()
{
	for(int i = 0; i < tg_->size(); i++) {
		PostQueudCompletionStatus(iocp_, 0, 0, NULL);
	}
	delete tg_;
	CloseHandle(iocp_);
}

bool fdpool_win32::async_attach(pollablefd_t* pfd)
{
	return CreateIoComletionPort(pfd->fd(), iocp_, pfd, 0) == pfd->fd();
}

int fdpool_win32::tp_working()
{
	pollablefd_t* pfd;
	ioinfo_t ioi;

	while (true) {
		BOOL succ = GetQueuedCompletionStatus(iocp_,
				&ioi.size,
				&pfd,
				&ioi.poverlapped,
				INFINITE);
		if (!pfd) {
			return 0;
		}
		pfd->process(ioi);
		if (pfd->status() == pollablefd_t::terminated)
			delete pfd;
	}
	return 0;
}
