#include <stdio.h>

int main()
{
	FILE* file = fopen("test.ini", "wb");
	fclose(file);
	return 0;
}
