#include "platformUtil.h"

#include <stdlib.h>
#include <stdio.h>

void showFile(const char * fileName)
{
#if defined(WIN32)
	system(fileName);
#elif defined(__linux__)
	/*Haven't tested this...*/
	char command[512];
	snprintf(command, 512, "xdg-open %s", fileName);
	system(command);
#elif defined(__APPLE__)
	/*Haven't tested this...*/
	char command[512];
	snprintf(command, 512, "open %s", fileName);
	system(command);
#endif
}
