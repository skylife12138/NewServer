#include"Prec.h"
GProjectMgr* GProMgr;
GTimeMgr* GTimer;

bool GProjectMgr::Init()
{
    cout << "init" << endl;
    _proexit = false;
	GTimer = new GTimeMgr();

	return true;
}

void GProjectMgr::MainLoop()
{
	GTimer->TimeTick();
}

bool GProjectMgr::IsExit()
{
    return false;
}