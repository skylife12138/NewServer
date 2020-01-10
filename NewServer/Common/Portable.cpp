#include "Portable.h"

#ifdef _WIN32
void InitTimeGetTime(){}
#else
static struct timespec starttm;
static bool InitTime = false;

void InitTimeGetTime()
{
    clock_gettime(CLOCK_MONOTONIC,&starttm);
    InitTime = true;
}

DWORD timeGetTime()
{
    if(!InitTime)
        InitTimeGetTime();
    struct timespec tm;
    clock_gettime(CLOCK_MONOTONIC,&tm);
    return DWORD((((QWORD)tm.tv_sec)*1000+tm.tv_nsec/1000000) - (((QWORD)starttm.tv_sec)*1000+starttm.tv_nsec/1000000));
}
#endif