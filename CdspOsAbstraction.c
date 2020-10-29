#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include "CdspOsAbstraction.h"
#include <unistd.h>
#include <sys/time.h>


static long startTimeMs;

void CdspOsDelay (unsigned int ms) {
	usleep (ms*1000);
}

void SetStartTime (void) {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	
	startTimeMs = currentTime.tv_sec*1000 + currentTime.tv_usec/1000;
}

uint32_t GetRunningTime (void) {
	struct timeval currentTime;
	long currentTimeMs;
	gettimeofday(&currentTime, NULL);
	
	currentTimeMs = currentTime.tv_sec*1000 + currentTime.tv_usec/1000;
	
	return (currentTimeMs - startTimeMs);
}

void CdspOsAssert (bool condition, uint32_t arg) {
	if (condition)
	{
		printf ("ASSERT!!! %d\n", arg);
	}
}
