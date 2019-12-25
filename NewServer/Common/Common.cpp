#include "Common.h"

LockMeta* CreateThreadLock()
{
	return new CriticalLock;
}

void Show(const char* msg)
{
	std::cout << msg << endl;
}