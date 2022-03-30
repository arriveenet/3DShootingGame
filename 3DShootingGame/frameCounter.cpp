#include "frameCounter.h"

#include <time.h>

static clock_t startTime;
static tm currentTime;
static unsigned int frameCounter;
unsigned int g_frameRate;

void frameCounterInit()
{
	errno_t err;
	time_t t = time(NULL);
	err = localtime_s(&currentTime, &t);
	startTime = clock();
}

void frameCounterUpdate()
{
	if (clock() - startTime >= 1000) {
		startTime = clock();
		g_frameRate = frameCounter;
		//printf("FPS:%d\n", FPS);
		frameCounter = 0;
	}
	frameCounter++;
}