#include <pal/daemon.h>
#include "nexus.h"

int main(int argc, char *argv[])
{
	int err = daemonize();
	if (err)
		return err;
	if (argc > 1)
		nexus_init(argv[1]);
	else
		nexus_init("nexus");
	return nexus_wait();
}

#ifdef _MSC_VER // if defined, must be windows
#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int )
{
	return main(__argc, __argv);
}
#endif
