#include"Prec.h"
GProjectMgr* GProMgr;
GlobalTimer* GTimer;

static void* MainAlloc(int size)
{
	return malloc(size);
}

static void MainFree(void* p, int size)
{
	free(p);
}

bool GProjectMgr::Init()
{
    cout << "init" << endl;
    _proexit = false;
	GTimer = new GlobalTimer(GetNowTime(), MainAlloc, MainFree);
	if (!GTimer)
	  return false;

	return true;
}

void GProjectMgr::MainLoop()
{
	GTimer->Tick((int)GetNowTime());
}

bool GProjectMgr::IsExit()
{
    return false;
}