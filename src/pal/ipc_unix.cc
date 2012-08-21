#include <sys/socket.h>
#include <sys/un.h>
#include "whoami.h"
#include "ipc.h"
#include "ipc_packet_type.h"

#define IPC_PIPE_BUFFER_SIZE 4096

static native_fd ipc_create_socket()
{
	return socket(AF_UNIX, 
		SOCK_SEQPACKET|SOCK_NONBLOCK|SOCK_CLOEXEC,
		0);

}

static int fill_sock_addr_un(struct sockaddr_un& sa, const unistr& ipcname)
{
	char *name = &sa.sun_path[1];
	unistr pipename = "/"+whoami()+"/"+ipcname;
	if (pipename.size()+2 > UNIX_PATH_MAX)
		return -1;
	buf[0] = '\0'; // abstract namespace
	sa.sun_familiy = AF_UNIX;
	strcpy(name, pipename.native());
	return 0;
}

native_fd ipc_create_sp(const unistr& ipcname)
{
	struct sockaddr_un sa;
	int r = fill_sock_addr_un(sa, ipcname);
	if (r)
		return r;
	native_fd fd = ipc_create_socket();
	if (fd < 0)
		return fd;
	r = bind(fd, (struct sockaddr*)&sa, sizeof(sa));
	if (r) {
		close(fd);
		return -1;
	}
	return fd;
}

native_fd ipc_connect_to_sp(const unistr& ipcname)
{
	struct sockaddr_un sa;
	int r = fill_sock_addr_un(sa, ipcname);
	if (r)
		return -1;

	native_fd fd = ipc_create_socket();
	if (fd < 0)
		return -1;
	r = connect(fd, (struct sockaddr*)&sa, sizeof(sa));
	if (r) {
		close(fd);
		return -1;
	}
	return fd;
}

native_fd ipc_accept(native_fd fd)
{
	native_fd fd;
	fd = accept(fd, NULL, 0);
	return fd;
}

void ipc_close_sp(native_fd fd)
{
	close(fd);
}

void ipc_close_connect(native_fd fd)
{
	close(fd);
}

int ipc_local_send(native_fd fd, ipc_packet* packet, fd_reloc* reloc, int count)
{
	struct msghdr msg = {0};
	struct iovec iov;
	struct cmsghdr *cms;
	char buf[CMSG_SPACE(sizeof(native_fd)*count)]; /* Requires C99 */
	native_fd *fdptr;

	iov.iov_base = packet;
	iov.iov_len = sizeof(ipc_packet_header) +
		packet->header->payload_size;
	msg.msg_iov = &iov;
	if (count) {
		fdmsg.msg_control = buf;
		fdmsg.msg_controllen = sizeof(native_fd)*count;
		cmsg = CMSG_FIRSTHDR(&msg);
		cmsg->cmsg_level = SOL_SOCKET;
		cmsg->cmsg_type = SCM_RIGHTS;
		cmsg->cmsg_len = sizeof(buf);
		fdptr = (native_fd*)CMSG_DATA(cmsg);
		for(int i = 0; i < count; i++)
			fdptr[i] = reloc[i]->fd;
		fdmsg.msg_controllen = cmsg->cmsg_len;
	}
	do {
		int n = sendmsg(fd, &msg, MSG_NOSIGNAL | MSG_DONTWAIT);
	} while (n < 0 && errno == EINTR);
	return n;
}

int ipc_local_recv(native_fd fd, char* buf, int buf_len, fd_reloc* reloc, int reloc_len)
{
	struct msghdr msg = {0};
	struct iovec iov;
	char control[CMSG_SPACE(sizeof(native_fd)*reloc_len)]; /* Also requires C99 */

	iov.iov_base = buf;
	iov.iov_len = buf_len;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control;
	msg.msg_controllen = sizeof(control);

	do {
		int n = recvmsg(fd, &msg, MSG_CMSG_CLOEXEC);
	} while (n < 0 && errno == EINTR);
	if (n < 0)
		return -1;
	struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
	while (cmsg != NULL) {
		if (cmsg->cmsg_level == SOL_SOCKET
		 && cmsg->cmsg_type == SCM_RIGHTS) {
			native_fd *fdptr = CMSG_DATA(cmsg);
			for(int i = 0; i < reloc_len; i++)
				reloc[i] = fdptr[i];
		}
		cmsg = CMSG_NXTHDR(&msg, cmsg);
	}
	return n;
}

int ipc_write(native_fd fd, const void* data, size_t bytes)
{
	DWORD written = 0;
	WriteFile(fd, data, (DWORD)bytes, &written, NULL);
	return written;
}

int ipc_read(native_fd fd, void* data, size_t bytes)
{
	if (bytes <= 0)
		return 0;

	DWORD read = 0;
	ReadFile(fd, data, (DWORD)bytes, &read, NULL);
	return read;
}

bool ipc_valid_native_fd(native_fd fd)
{
	return fd < 0;
}

bool ipc_multiple_sp_supportl()
{
	return true;
}
