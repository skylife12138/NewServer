#include "../Prec.h"
#include "../Net/IoThread.h"
#include "../Net/MsgFilter.h"
#include "../Net/NetWorkManger.h"
#include "../Common/Common.h"


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

GProjectMgr::~GProjectMgr()
{
	if(GTimer)
		delete GTimer;

	IoThread::Instance()->Stop();
}

bool GProjectMgr::Init()
{
    cout << "init Begin..." << endl;
	_proexit = false;
	_LastTickTime = NowTickCount;

	GTimer = new GlobalTimer(GetNowTime(), MainAlloc, MainFree);
	if (!GTimer)
	  return false;

	CMsgFilter::StaticInit();
	IoThread::StaticInit();
	ClientMgr::StaticInit();
	CNetPackPool::StaticInit();
	GObjFact = new ObjectFactory;

	IoThread::Instance()->Start();
#ifndef NET_LIB_EVENT	
	NetMgr = new NewWorkMgr();
	if(!NetMgr || !NetMgr->NetWorkInit())
	{
		cout << "Net Init Error!!!" << endl;
		return false;
	}
#endif

	cout << "server start success!!!" << endl;
	return true;
}

void GProjectMgr::Realase()
{
	CMsgFilter::StaticDestory();
	IoThread::StaticDestory();
	ClientMgr::StaticDestory();
	CNetPackPool::StaticDestory();

	delete GTimer;
	delete GObjFact;
	GTimer = NULL;
	GObjFact = NULL;
}

void GProjectMgr::MainLoop()
{
	if(NowTickCount - _LastTickTime > 3000)
	{
		GObjFact->ShowState();
		_LastTickTime = NowTickCount;
	}

	GTimer->Tick((int)GetNowTime());
	ClientMgr::Instance()->ParseAllClient();

#ifndef NET_LIB_EVENT
	NetMgr->HandleNetMsg();
#endif
}

bool GProjectMgr::IsExit()
{
    return false;
}