#include "Common.h"

DWORD RealWorldTime = 0;
DWORD NowTickCount = 0;
class ObjectFactory* GObjFact;

LockMeta* CreateThreadLock()
{
	return new CriticalLock;
}

void Show(const char* msg)
{
	std::cout << msg << endl;
}