#include"../Prec.h"
#include "../Common/GTimer.h"
GProjectMgr* GProMgr = NULL;
GlobalTimer* GTimer = NULL;
NewWorkMgr *NetMgr = NULL;

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
    cout << "init Begin..." << endl;
    _proexit = false;
	GTimer = new GlobalTimer(GetNowTime(), MainAlloc, MainFree);
	if (!GTimer)
	  return false;
	NetMgr = new NewWorkMgr();
	if(!NetMgr || !NetMgr->NetWorkInit())
	{
		cout << "Net Init Error!!!" << endl;
		return false;
	}

	cout << "server start success!!!" << endl;
	return true;
}

void GProjectMgr::MainLoop()
{
	GTimer->Tick((int)GetNowTime());
	NetMgr->HandleNetMsg();
}

bool GProjectMgr::IsExit()
{
    return false;
}