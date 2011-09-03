#include <windows.h>
#include "opened.h"

opened_t::opened_t()
	:volume_handle(INVALID_HANDLE_VALUE),
	handle_index(-1)
{
	overlap.hEvent = INVALID_HANDLE_VALUE;
	ZeroMemory(&overlap, sizeof(overlap));
}
