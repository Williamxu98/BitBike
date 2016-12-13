#ifndef TIME_H
#define TIME_H

#include "definitions.h"
#include <time.h>

#define TIM_MAX_DIFFERENCE 32767

int tim_getTime();
int tim_subtractTime(int time1, int time2);

#endif
