#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <algorithm>
#include <iostream>
#include "Lock.h"
#include "../Logic/SingleTon.h"


const int TINY = 1024;
const int NORMAL = 2048;
const int BIG = 4096;

LockMeta* CreateThreadLock();

void Show(const char* msg);

#endif