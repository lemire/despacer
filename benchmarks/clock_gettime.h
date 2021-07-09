#if _WIN32 || _WIN64

#include "windows.h"

#define CLOCK_REALTIME 1

int clock_gettime(int _, struct timespec *tv)
{
    return timespec_get(tv, TIME_UTC);
}

#endif
