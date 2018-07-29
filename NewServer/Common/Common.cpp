#include "Common.h"

LockMeta* CreateThreadLock()
{
	return new CriticalLock;
}