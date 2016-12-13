#include "time.h"

int tim_getTime()
{
    return ((int) millis()) % TIM_MAX_DIFFERENCE;
}

int tim_subtractTime(int time1, int time2)
{
    return (time2 - time1) >= 0 ? time2 - time1 : (time2 - time1) + TIM_MAX_DIFFERENCE;
}
