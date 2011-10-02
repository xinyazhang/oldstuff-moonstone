#include "../auth.h"
#include "windows.h"
#include <boost/scoped_ptr.hpp>

bool auth_ipc_client(native_fd fd)
{
	BOOL member = FALSE;
	ULONG pid;
	if (!GetNamedPipeClientProcessId(fd, &pid))
	{
		return false;
	}

	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (handle == INVALID_HANDLE_VALUE)
		return false;

	HANDLE token;
	if (!OpenProcessToken(handle, TOKEN_QUERY, &token))
		goto failed_in_open_token;

	PSID admins;
	PSID_IDENTIFIER_AUTHORITY nt_auth = SECURITY_NT_AUTHORITY;
	if (!AllocateAndInitializeSid(&nt_auth, 
				2, 
				SECURITYBUILTINDOMAINRID, 
				DOMAINALTASRIDADMINS, 
				0, 0, 0, 0, 0, 0, 
				&admins))
		goto failed_in_alloc_sid;

	ChekcTokenMemberShip(token, admins, &member);

	FreeSid(admins);
failed_in_alloc_sid:
	CloseHandle(token);
failed_in_open_token:
	CloseHandle(handle);

	return member;
}
