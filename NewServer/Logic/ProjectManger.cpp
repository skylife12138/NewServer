#include "../Prec.h"
#include "../Net/IoThread.h"
#include "../Net/MsgFilter.h"
#include "../Net/NetWorkManger.h"
#include "../Common/Common.h"
#include "../Common/Log.h"


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
	cout << "Server Begin Init ..." << endl;

	GTimer = new GlobalTimer(GetNowTime(), MainAlloc, MainFree);
	if(!GTimer)
		return false;
	RealWorldTime = GTimer->GetNowTimeStamp();
	GTimer->GetSystemTime(NowSystemTime,RealWorldTime);
	LogThread::StaticInit();
	if(!LogThread::Instance()->Start())
	{
		Error("LogThread Start Error!");
		return false;
	}
	GObjFact = new ObjectFactory;
	if (!GObjFact)
	  return false;
	
	//以下是其他初始化逻辑
	CMsgFilter::StaticInit();
	IoThread::StaticInit();
	ClientMgr::StaticInit();
	CNetPackPool::StaticInit();
	
	if(!IoThread::Instance()->Start())
	{
		Error("IoThread Start Error!");
		return false;
	}
#ifndef NET_LIB_EVENT	
	NetMgr = new NewWorkMgr();
	if(!NetMgr || !NetMgr->NetWorkInit())
	{
		Error("Net Init Error!!!");
		return false;
	}
#endif

	IsServerInitOver = true;
	Show("server start success!!!");
	return true;
}

void GProjectMgr::Realase()
{
	IoThread::Instance()->Release();

	CMsgFilter::StaticDestory();
	IoThread::StaticDestory();
	ClientMgr::StaticDestory();
	CNetPackPool::StaticDestory();

	LogThread::Instance()->Release();//必须最后析构

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