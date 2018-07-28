#include"Prec.h"
GProjectMgr* GProMgr;

bool GProjectMgr::Init()
{
    cout << "init" << endl;
    _proexit = false;
	GTimeMgr::StaticInit();

	return true;
}

void GProjectMgr::MainLoop()
{
	GTimeMgr::Instance()->TimeTick();
}

bool GProjectMgr::IsExit()
{
    return false;
}