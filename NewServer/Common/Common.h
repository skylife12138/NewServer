#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <algorithm>
#include <iostream>
#include "Lock.h"
#include "Portable.h"
#include "../Logic/SingleTon.h"
#include "./ObjectFactory.h"

#define TimeGetTime timeGetTime
#define USE_TCMALLOC 0

const int TINY = 1024;
const int NORMAL = 2048;
const int BIG = 4096;

const int MAX_MESSAGE_LEN = 4096 * 4;

/*全局变量*/
extern DWORD RealWorldTime;
extern DWORD NowTickCount;
extern class ObjectFactory* GObjFact;

#if not USE_TCMALLOC
inline void* operator new(size_t size)
{
    void* ret = malloc(size);
    assert(ret);
    return ret ? ret : NULL;
}

inline void operator delete(void* ptr)
{
    free(ptr);
}

inline void* operator new[](size_t size)
{
    void* ret = malloc(size);
    assert(ret);
    return ret ? ret : NULL;
}

inline void operator delete[](void* ptr)
{
    free(ptr);
}

#endif

LockMeta* CreateThreadLock();

void Show(const char* msg);

#endif