#include <stdio.h>

const wchar_t* value = 
L"THIS "
L"IS "
L"SPARTAN!";

int main()
{
	wprintf(L"%s", value);
	return 0;
}
